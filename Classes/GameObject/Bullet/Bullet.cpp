#include "Bullet.h"
#include "../BulletCreator.h"
#include "../ItemManager.h"
#include "../../Task/PoolingManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Planet.h"
#include "../../GameObject/Player.h"
#include "../../AI/States/BulletStates.h"
#include "../../Scene/GameScene.h"
#include "../../MyUI/ComboScore.h"
#include "../../Particle/Particles.h"
#include "../../DataManager/AchievementDataManager.hpp"
#include "../../DataManager/AchievementChecker/AchievementClearChecker.h"
#include "../../DataManager/GradientDataManager.h"
using namespace cocos2d;

CBullet::CBullet()
: m_bIsPlayerGet(false)
, m_Time(0.f)
, m_ItemFlag(eITEM_FLAG_none)
, m_TargetVec(CObjectManager::Instance()->getPlanet()->getPosition())
, m_Player(CObjectManager::Instance()->getPlayer())
, m_Planet(CObjectManager::Instance()->getPlanet())
, m_BulletSprite(nullptr)
{
#if(!USE_MEMORY_POOLING)
    m_FSM = nullptr;
#endif
    
	// bullet이 초기화 될때마다 매번 생성하지 않는다.
	if (m_FSM == nullptr){
		m_FSM = new CStateMachine<CBullet>(this);
	}
	if (m_FSM != nullptr){
		m_FSM->ChangeState(CBulletNormal::Instance());
	}
    setCascadeOpacityEnabled(true);
    setCascadeColorEnabled(true);
}

CBullet::~CBullet(){
	if (m_FSM != nullptr)
		delete m_FSM;
}

CBullet* CBullet::build()
{
    if(m_BulletInfo._symbol == -1) return nullptr;
    
    // sprite init
    if(m_BulletInfo._spriteName != "")
    {
        m_BulletSprite = Sprite::createWithSpriteFrameName(m_BulletInfo._spriteName);
        this->setContentSize(m_BulletSprite->getContentSize());
        m_BulletSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_BulletSprite->setPosition(this->getContentSize() / 2);
        this->addChild(m_BulletSprite);
    }

	//auto debug = LayerColor::create(Color4B::MAGENTA, this->getContentSize().width, this->getContentSize().height);
	//debug->setOpacity(255 * 0.5f);
	//debug->ignoreAnchorPointForPosition(false);
	//debug->setPosition(this->getContentSize() / 2);
	//debug->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//addChild(debug);
    
    // if it is nonfly bullet
    if(!m_BulletInfo._isFly){
        this->scheduleOnce([this](float delta){
            this->R_FadeOutWithCount(5, 3.f);
        }, 5.f, MakeString("AutoRemove_%d", random<int>(1, 100)));
    }
    
    // position init
    Vec2 pos = Vec2::ZERO;
    float angle = -90.f;
    if(!CItemManager::Instance()->isCurrentItem(eITEM_FLAG::eITEM_FLAG_bonustime)){
        pos = CBullet::getCirclePosition(getAngle(), getDistance(), m_Planet->getPosition());
		this->setTargetVec(this->getPlanet()->getPosition());
        angle = -getAngle();
    }
	else{
		pos = CBullet::getSquarePosition(getAngle(), getDistance());
		this->setTargetVec(cocos2d::Vec2(pos.x, 0));
	}
	
    this->setPosition(pos);
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    // rotation init
    this->setRotation(angle);
//	this->setScale(1.5f);
//	this->setBoundingRadius(this->getBoundingRadius() * 1.5f);
        
    return this;
}

CBullet* CBullet::setBulletInfo(sBULLET_PARAM data)
{
    m_BulletInfo = data;
    return this;
}

bool CBullet::init()
{
    if(!CMover::init()) return false;
    return true;
}

void CBullet::Execute(float delta)
{
	m_Time += delta;
	if (!IsTimeUP()) return;
    
    m_FSM->Execute(delta);
    this->setVisible(true);
}

Vec2 CBullet::getCirclePosition(float angle, float distance, Vec2 center)
{
    Vec2 result = Vec2::ZERO;
    result.x = (cos(CC_DEGREES_TO_RADIANS(angle)) *  distance) + center.x;
    result.y = (sin(CC_DEGREES_TO_RADIANS(angle)) *  distance) + center.y;
    
    return result;
}

Vec2 CBullet::getSquarePosition(float angle, float distance)
{
    Vec2 result = Vec2::ZERO;
    auto visibleSize = Director::getInstance()->getVisibleSize();
	result.x = (angle * (visibleSize.width / 360));
    result.y = distance;
    
    return result;
}

#if(USE_MEMORY_POOLING)
/* poolingManager에서 FreeMemory Block을 하나 가져옴 */
void* CBullet::operator new(size_t size, const std::nothrow_t)
{
    // PoolingManager에서 메모리를 할당 받는다.
    return CPoolingManager::Instance()->BulletNew();
}
#endif

/* 오브젝트의 메모리를 FreeMemoryBlock으로 변환 == 오브젝트 삭제 */
void CBullet::ReturnToMemoryBlock()
{
	/*removeFromParent 의 이유 : 
	이유는 모든 CMover의 파생 객체들은 메모리 블럭에서 메모리를 할당 받는다.
	그로인해 실행 중 addChild시 같은 메모리를 여러번 addChild할 수 있다.
	때문에 메모리 블럭으로 되돌릴때에는 부모관계를 제거하여야한다.
	또 ReferenceCount를 1 낮춰야 하는 이유도 있다.*/
#if(!USE_MEMORY_POOLING)
    CObjectManager::Instance()->removeBulletFromList(this);
    this->setAlive(false);
    this->removeFromParent();
#else
    this->setVisible(false);
    this->setReturnToMemoryBlock(true);
#endif
}


/* 회전행렬을 이용하여 오브젝트 회전 및 이동 */
void CBullet::Rotation(float speed)
{
    
    // 회전 속도와 방향을 이용하여 각도를 구하고 라디안으로 변환
    float radian = CC_DEGREES_TO_RADIANS(speed);
    
    // 현재의 Direction Vector를 저장한다.
    Vec2 beforeRotation = getPosition() - m_Planet->getPosition();
    
    // 거리도 저장
    float length = beforeRotation.length();
    
    /* 회전행렬을 구함
     * rotate x = ((x_ * cos(angle)) - (y_ * sin(angle)))
     * rotate y = ((x_ * sin(angle)) + (y_ * cos(angle))) */
    m_RotationVec = Vec2((float)((beforeRotation.x * cos(radian)) - (beforeRotation.y * sin(radian))),
                         (float)((beforeRotation.x * sin(radian)) + (beforeRotation.y * cos(radian))));
    
    // 노말라이즈
    m_RotationVec.normalize();
    m_RotationVec *= length;
    
    // 기존의 좌표에 새로운 좌표를 더해준다.
    setPosition(m_Planet->getPosition() + m_RotationVec);
    
    // 오브젝트 자체도 회전
    setAngle(-(getRotation() - speed));
    
//	setAngle(-(getRotation() - (dir *(m_RotationSpeed * delta))));
    setRotation(-getAngle());
}

void CBullet::R_UpAndBezier()
{
    // do not execute
    this->setAlive(false);
    
    // zorder
    this->setLocalZOrder(ZORDER::POPUP);
    
    // create action
    auto upActin      = MoveTo::create(0.6f, Vec2(getPositionX(), getPositionY() + 150.f));
    auto sineAction   = EaseExponentialOut::create(upActin);
    auto delayAction  = DelayTime::create(0.5f);
    
    auto action = Sequence::create(sineAction,
                                   delayAction,
                                   CallFunc::create([=](){
        
        // create bezier action config
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto targetPos   = Vec2(visibleSize.width * 0.06f, visibleSize.height * 0.925f);
        targetPos        = CGameScene::getZoomLayer()->convertToNodeSpace(targetPos);
        auto length      = Vec2(targetPos - this->getPosition()).length();
        auto cPos1       = Vec2(this->getPosition().x - (length * 0.3f),
                                this->getPosition().y - 100.f);
        auto cPos2       = Vec2(targetPos.x, targetPos.y - (length * 0.3f));
        auto time        = std::max<float>(0.5f, (length / visibleSize.height) * 1.3f);
        auto scale       = 3.f;
        
        // create bezier action
        ccBezierConfig bezier;
        bezier.controlPoint_1 = Vec2(cPos1);
        bezier.controlPoint_2 = Vec2(cPos2);
        bezier.endPosition    = Vec2(targetPos);
        
        auto bezierAction = BezierTo::create(time, bezier);
        auto exponential  = EaseSineIn::create(bezierAction);
        auto scaleAction  = ScaleTo::create(0.4f, scale);
        auto fadeAction   = FadeTo::create(0.4f, 1);
        auto spawn        = Spawn::create(scaleAction, fadeAction, nullptr);
        
        this->runAction(Sequence::create(exponential,
                                         spawn,
                                         CallFunc::create([=](){
            this->ReturnToMemoryBlock();
        }), nullptr));
        
    }), nullptr);
    
    this->runAction(action);
}

/* 일정 위치로 베지어 곡선을 그리며 이동한 후 커지면서 FadeOut */
void CBullet::R_BezierWithScale(Vec2 targetPos, Vec2 controlPoint_1, Vec2 controlPoint_2, float time, float scale)
{
    // do not execute
    this->setAlive(false);
    
    // zorder
    this->setLocalZOrder(ZORDER::POPUP);
    
    // create action
    ccBezierConfig bezier;
    bezier.controlPoint_1 = Vec2(controlPoint_1);
    bezier.controlPoint_2 = Vec2(controlPoint_2);
    bezier.endPosition    = Vec2(targetPos);
    auto bezierAction = BezierTo::create(time, bezier);
    auto scaleAction  = ScaleTo::create(0.4f, scale);
    auto fadeAction   = FadeTo::create(0.4f, 1);
    auto spawn = Spawn::create(scaleAction, fadeAction, nullptr);
    
    auto action = Sequence::create(bezierAction,
                                   spawn,
                                   CallFunc::create([=](){
        
        this->ReturnToMemoryBlock();
        
    }), nullptr);
    
    this->runAction(action);
}


void CBullet::R_BezierWithRotation(Vec2 targetPos, Vec2 controlPoint_1, Vec2 controlPoint_2, float time)
{
    // do not execute
    setAlive(false);

    // zorder
    this->setLocalZOrder(ZORDER::POPUP);
    
	// 이전의 Action들을 cancel
	this->stopAllActions();

	ccBezierConfig bezier;
	bezier.controlPoint_1 = Vec2(controlPoint_1);
	bezier.controlPoint_2 = Vec2(controlPoint_2);
	bezier.endPosition    = Vec2(targetPos);
	auto bezierAction = BezierTo::create(time, bezier);
    auto rotateAction = RotateBy::create(1.f, 720.f);
    auto spawn = Spawn::create(rotateAction, bezierAction, nullptr);
	auto action = Sequence::create(spawn,
                                   CallFunc::create([=](){

        this->ReturnToMemoryBlock();
        
	}), nullptr);
    
	this->runAction(action);
}


void CBullet::R_ScaleWithFadeOut(float scale, float scaleTime, float fadeOutTime)
{
    // do not execute
    this->setAlive(false);
    
	this->stopAllActions();

    auto fadeAction = FadeTo::create(fadeOutTime, 1);
    auto scaleAction = ScaleTo::create(scaleTime, scale);
    auto spawn = Spawn::create(fadeAction, scaleAction, nullptr);
    
	auto action = Sequence::create(spawn,
                                   CallFunc::create([=](){
        this->ReturnToMemoryBlock();
	}), nullptr);
    
	this->runAction(action);
}


void CBullet::R_FadeOutWithCount(int repeat, float removeTime)
{
	this->scheduleOnce([=](float dt){
		this->ReturnToMemoryBlock();
	}, removeTime + 0.1f, MakeString("FadeOutWithCount_%d", random<int>(1, 100)));

	auto textureAction1 = FadeTo::create((removeTime / repeat) - 0.1f, 1);
	auto textureAction2 = FadeTo::create(0.1f, 255);
	auto repeatAction = Repeat::create(Sequence::create(textureAction2, textureAction1, nullptr), repeat);
	this->runAction(repeatAction);
}


/* 조종행동 Seek - 오브젝트를 목표지점까지 이동 */
void CBullet::Seek(float delta)
{
    // 방향벡터 구하기
    Vec2 dir = m_TargetVec - getPosition();
    dir.normalize();
    
    // 속력벡터 계산
    dir *= (getSpeed() * delta);
    
    // 현재 좌표에 적용
    setPosition(getPosition() + dir);
}

void CBullet::Flee(float delta)
{
    // 방향벡터 구하기
    Vec2 dir = getPosition() - m_TargetVec;
    dir.normalize();
    
    // 속력벡터 계산
    dir *= (getSpeed() * delta);
    
    // 현재 좌표에 적용
    setPosition(getPosition() + dir);
}

void CBullet::createScoreCurrentPos(int score)
{
    auto scoreBullet = CScoreBullet::create(score);
    scoreBullet->setPosition(getPosition());
    scoreBullet->setAnchorPoint(Vec2::ZERO);
    CGameScene::getZoomLayer()->addChild(scoreBullet);
    CComboScore::Instance()->AddCombo();
    GVALUE->STAR_SCORE += score;
}

void CBullet::CollisionWithPlanet()
{
    if (this->getIsFly()){
        this->R_ScaleWithFadeOut(0.1f, 0.15f, 0.15f);
    }
}

//void CBullet::createCollisionParticle()
//{
//    auto particle = CParticle_Explosion::create(m_BulletInfo._particleName);
//    if (particle != nullptr){
//        particle->retain();
//        particle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//        particle->setStartSpin(getRotation());
//        particle->setEndSpin(getRotation());
//        particle->setPosition(getPosition());
//        particle->setGravity(m_RotationVec);
//        particle->setSpeed(100);
//        particle->setSpeedVar(50);
//        CGameScene::getZoomLayer()->addChild(particle, ZORDER::BULLET);
//    }
//}

void CBullet::createExplosionEffect()
{
    auto createParticle = [=](Node* node){
        auto angle = random<float>(-getRotation() - 30, -getRotation() + 30);
        auto distance = random<float>(30, 80);
        auto size = random<float>(1.4f, 1.9f);
        auto pos  = CBullet::getCirclePosition(angle, distance, getPosition());
        auto move = MoveTo::create(0.5f, pos);
        auto fade = FadeTo::create(0.5f, 0);
        auto scale = ScaleTo::create(0.5f, size);
        auto spawn = Spawn::create(move, fade, scale, nullptr);
        auto seq  = Sequence::create(spawn, RemoveSelf::create(), nullptr);
        node->runAction(seq);
    };
    
    for(int i = 0; i < 5; i++)
    {
        auto sprite = Sprite::create("particle_snow.png");
        sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprite->setPosition(Vec2(getPosition()));
        sprite->setScale(0.7);
        sprite->setRotation(-getRotation());
        sprite->setColor(CGradientDataManager::Instance()->getBulletColorByLevel(GVALUE->NOTICE_LEVEL));

        CGameScene::getZoomLayer()->addChild(sprite, ZORDER::POPUP);
        
        createParticle(sprite);
    }
}

void CBullet::setBoundingRadius (float data)
{
    CGameObject::setBoundingRadius(data);
    m_BulletInfo._boundingRadius = data;
}

void CBullet::setSpeed          (float data) { m_BulletInfo._speed            = data; }
void CBullet::setAngle          (float data) { m_BulletInfo._angle            = data; }
void CBullet::setDistance       (float data) { m_BulletInfo._distance         = data; }
void CBullet::setDelayTime		(float data) { m_BulletInfo._delayTime		  = data; }
void CBullet::setPower          (float data) { m_BulletInfo._power            = data; }
void CBullet::setSymbol         (float data) { m_BulletInfo._symbol           = data; }
void CBullet::setIsFly          (float data) { m_BulletInfo._isFly            = data; }
void CBullet::setParticleName   (std::string data) { m_BulletInfo._particleName = data; }

sBULLET_PARAM CBullet::getInfo()   const { return m_BulletInfo;                 }
float CBullet::getBoundingRadius() const { return m_BulletInfo._boundingRadius; }
float CBullet::getSpeed()          const { return m_BulletInfo._speed;          }
float CBullet::getAngle()          const { return m_BulletInfo._angle;          }
float CBullet::getDistance()       const { return m_BulletInfo._distance;		}
float CBullet::getDelayTime()      const { return m_BulletInfo._delayTime;		}
float CBullet::getPower()          const { return m_BulletInfo._power;          }
char  CBullet::getSymbol()         const { return m_BulletInfo._symbol;         }
bool  CBullet::getIsFly()          const { return m_BulletInfo._isFly;          }
int   CBullet::getPatternIndex()   const { return m_BulletInfo._patternIdx;     }
