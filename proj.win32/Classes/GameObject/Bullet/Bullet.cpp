#include "Bullet.h"
#include "../ItemManager.h"
#include "../../Task/PoolingManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Planet.h"
#include "../../GameObject/Player.h"
#include "../../AI/States/BulletStates.h"
#include "../../MyUI/ScoreUI.h"
#include "../../MyUI/UIManager.h"
#include "../../Scene/GameScene.h"
#include "../../MyUI/MultipleScore.h"

using namespace cocos2d;

CBullet::CBullet()
: m_bIsPlayerGet(false)
, m_Time(0.f)
, m_RotationSpeed(100.f)
, m_ItemFlag(eITEM_FLAG_none)
, m_UIScore(nullptr)
, m_MultipleScore(nullptr)
, m_TargetVec(CObjectManager::Instance()->getPlanet()->getPosition())
, m_Player(CObjectManager::Instance()->getPlayer())
, m_Planet(CObjectManager::Instance()->getPlanet())
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
        auto sprite = Sprite::createWithSpriteFrameName(m_BulletInfo._spriteName);
        this->setContentSize(sprite->getContentSize());
        sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprite->setPosition(this->getContentSize() / 2);
        this->addChild(sprite);
    }
    
    // if it is nonfly bullet
    if(!m_BulletInfo._isFly){
        this->scheduleOnce([this](float delta){
            this->R_FadeOutWithCount(5, 3.f);
        }, 5.f, MakeString("AutoRemove_%d", random<int>(1, 100)));
    }
    
    // position init
    auto pos = CBullet::getCirclePosition(m_BulletInfo._angle,
                                          m_BulletInfo._distance,
                                          m_Planet->getPosition());
    this->setPosition(pos);
    
    // rotation init
    this->setRotation(-m_BulletInfo._angle);
    
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
    m_FSM->Execute(delta);
}

Vec2 CBullet::getCirclePosition(float angle, float distance, Vec2 center)
{
    Vec2 result = Vec2::ZERO;
    result.x = (cos(CC_DEGREES_TO_RADIANS(angle)) *  distance) + center.x;
    result.y = (sin(CC_DEGREES_TO_RADIANS(angle)) *  distance) + center.y;
    
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
	this->removeFromParent();
	this->removeAllChildren();
	this->setVisible(false);
	this->setAlive(false);
	CPoolingManager::Instance()->Bullet_ReturnToFreeMemory(this);
#endif
}


/* 회전행렬을 이용하여 오브젝트 회전 및 이동 */
void CBullet::Rotation(float dir, float delta)
{
    
    // 회전 속도와 방향을 이용하여 각도를 구하고 라디안으로 변환
    float radian = CC_DEGREES_TO_RADIANS(dir * (m_RotationSpeed * delta));
    
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
    setRotation(getRotation() - (dir *( m_RotationSpeed * delta)));
}


/* 일정 위치로 베지어 곡선을 그리며 이동한 후 커지면서 FadeOut */
void CBullet::R_BezierWithScale(Vec2 targetPos, Vec2 controlPoint_1, Vec2 controlPoint_2, float time, float scale)
{
    // 더이상 Execute 하지 않는다.
    setAlive(false);
    
    // 베지어 곡선 생성
    ccBezierConfig bezier;
    bezier.controlPoint_1 = Vec2(controlPoint_1);
    bezier.controlPoint_2 = Vec2(controlPoint_2);
    bezier.endPosition = Vec2(targetPos);
    
    // 베지어 액션 및 다른 액션 순서대로 실행
    auto bezierTo1 = BezierTo::create(time, bezier);
    this->setZOrder(101);
    auto action = Sequence::create(bezierTo1,
        Spawn::create(ScaleBy::create(0.5f, scale), FadeTo::create(2.0f, 1), NULL),
        // 두 액션이 끝난후 스케줄을 걸어 오브젝트 삭제
        CallFunc::create([&](){
        this->scheduleOnce([=](float dt){
            this->ReturnToMemoryBlock();
        }, 1.0f, "BezierWithScale");
        
    }), nullptr);
    this->runAction(action);
    
    // texture 페이드 아웃
//    auto textureAction = FadeOut::create(2.0f);
//    m_pTexture->runAction(textureAction);
}


void CBullet::R_BezierWithRotation(Vec2 targetPos, Vec2 controlPoint_1, Vec2 controlPoint_2, float time)
{
	// 더이상 Execute 하지 않는다.
	setAlive(false);

	// 이전의 Action들을 cancel
	this->stopAllActions();

	ccBezierConfig bezier;
	bezier.controlPoint_1 = Vec2(controlPoint_1);
	bezier.controlPoint_2 = Vec2(controlPoint_2);
	bezier.endPosition = Vec2(targetPos);

	auto bezierTo1 = BezierTo::create(time, bezier);
	auto action = Sequence::create(
        Spawn::create(RotateBy::create(0.5f, 720.f), bezierTo1, NULL),
		CallFunc::create([&](){

		this->scheduleOnce([=](float dt){
			this->ReturnToMemoryBlock();
		}, 1.0f, "BezierWithRotation");
	}), nullptr);
	this->runAction(action);
//	auto textureAction = RotateBy::create(0.5f, 720.f);
//	m_pTexture->runAction(textureAction);
}


void CBullet::R_ScaleWithFadeOut(float scale, float scaleTime, float fadeOutTime)
{
	// 더이상 Execute 하지 않는다.
	setAlive(false);

	// 이전의 Action들을 cancel
	this->stopAllActions();
//	this->m_pTexture->stopAllActions(); 

	auto action = Sequence::create(
        Spawn::create(FadeTo::create(fadeOutTime, 1), ScaleBy::create(scaleTime, scale), NULL),
		CallFunc::create([&](){

		this->scheduleOnce([=](float dt){
			this->ReturnToMemoryBlock();
		}, 1.0f, MakeString("ScaleWithFadeOut_%d", random<int>(1, 100)));
	}), nullptr);
	this->runAction(action);
//	auto textureAction = FadeOut::create(fadeOutTime);
//	this->runAction(textureAction);
}


void CBullet::R_FadeOutWithCount(int intervalCount, float removeTime)
{
	this->scheduleOnce([=](float dt){
		this->ReturnToMemoryBlock();
	}, removeTime + 0.1f, MakeString("FadeOutWithCount_%d", random<int>(1, 100)));

	auto textureAction1 = FadeTo::create((removeTime / intervalCount) - 0.1f, 1);
	auto textureAction2 = FadeTo::create(0.1f, 255);
	auto repeatAction = Repeat::create(Sequence::create(textureAction2, textureAction1, nullptr), intervalCount);
	this->runAction(repeatAction);
}


void CBullet::StackedRL(float duration, float stackSize, int stackCount)
{
	this->runAction(
		Repeat::create(
		Sequence::create(
		MoveBy::create(duration / stackCount, Vec2(stackSize, 0)),
		MoveBy::create(duration / stackCount, Vec2(-stackSize, 0)),
		nullptr), stackCount));
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


void CBullet::createScoreCurrentPos(int score)
{
	if (m_MultipleScore != nullptr){
		m_MultipleScore->AddScore(score);
		auto scoreBullet = CScoreBullet::create(score);
		scoreBullet->setPosition(getPosition());
		scoreBullet->setAnchorPoint(Vec2::ZERO);
		CGameScene::getGridWorld()->addChild(scoreBullet);
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
void CBullet::setPower          (float data) { m_BulletInfo._power            = data; }
void CBullet::setSymbol         (float data) { m_BulletInfo._symbol           = data; }
void CBullet::setIsFly          (float data) { m_BulletInfo._isFly            = data; }
void CBullet::setIsAiming       (float data) { m_BulletInfo._isAiming         = data; }

sBULLET_PARAM CBullet::getInfo()   const { return m_BulletInfo;                 }
float CBullet::getBoundingRadius() const { return m_BulletInfo._boundingRadius; }
float CBullet::getSpeed()          const { return m_BulletInfo._speed;          }
float CBullet::getAngle()          const { return m_BulletInfo._angle;          }
float CBullet::getDistance()       const { return m_BulletInfo._distance;       }
float CBullet::getPower()          const { return m_BulletInfo._power;          }
char  CBullet::getSymbol()         const { return m_BulletInfo._symbol;         }
bool  CBullet::getIsFly()          const { return m_BulletInfo._isFly;          }
bool  CBullet::getIsAiming()       const { return m_BulletInfo._isAiming;       }
