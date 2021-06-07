#include "Rocket.h"
#include "Planet.h"
#include "Bullet/Bullet.h"
#include "MenuSceneObjectManager.h"
#include "../AI/States/RocketStates.h"
#include "../Particle/Particles.h"
#include "../Scene/GameScene.h"
#include "../GameObject/ObjectManager.h"
#include "../GameObject/ItemManager.h"
#include "../GameObject/Player.h"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/RocketDataManager.hpp"
#include "../MyUI/MyButton.h"
#include "../MyUI/Popup/RewardPopup.h"
#include "../DataManager/AchievementRewarder/AchievementRewarder.hpp"
using namespace cocos2d;

CRocket::CRocket()
: m_Speed(0.f)
, m_Distance(0.f)
, m_Direction(1)
, m_Time(0.f)
, m_FSM(nullptr)
, m_Texture(nullptr)
, m_Velocity(Vec2::ZERO)
, m_PlayerPos(Vec2::ZERO)
, m_Arrive(false)
, m_ArriveCallback(nullptr)
//, m_Gift(nullptr)
, m_Player(CObjectManager::Instance()->getPlayer())
{}

CRocket::~CRocket(){}

CRocket* CRocket::create()
{
	CRocket* pRet = new(std::nothrow)CRocket();

	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}


bool CRocket::init()
{
    if (!CGameObject::init())
		return false;
    
    setCascadeOpacityEnabled(true);

    m_FSM = std::shared_ptr<CStateMachine<CRocket>>(new CStateMachine<CRocket>(this),
                                                    [=](CStateMachine<CRocket>* fsm){
                                                        delete fsm;
                                                    });
    this->ChangeState(CFlyAround::Instance());
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    m_FlyLimitMax = visibleSize.width * 0.9f;
    m_FlyLimitMin = visibleSize.width * 0.1f;
    
    m_RocketParam = CRocketDataManager::Instance()->getCurRocket();
    m_Texture = Sprite::createWithSpriteFrameName(m_RocketParam->_textureName);
    if (m_Texture != nullptr){
        this->setContentSize(m_Texture->getContentSize());
        m_Texture->setPosition(this->getContentSize() / 2);
        m_Texture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        addChild(m_Texture);
    }
    
	this->setBoundingRadius(ROCKET_DEFINE::BOUNDING_RADIUS);

//	m_Gift = CMyButton::create()
//		->addEventListener([=](Node* sender){
//		auto popup = CGameScene::getGameScene()->Reward();
//		auto rewardPopup = dynamic_cast<CRewardPopup*>(popup);
//		rewardPopup->AddRewardToList(ACHIEVEMENT_REWARD_KEY::REWARD_COIN_RANDOM, 50);
//        rewardPopup->AddRewardToList(ACHIEVEMENT_REWARD_KEY::REWARD_CHARACTER_RANDOM, 50);
//		rewardPopup->setExitCallback([=](){
////			sender->setVisible(false);
////            this->ChangeState(CFlyAway::Instance());
//		});
//	})
//		->setButtonNormalImage("rewardIcon_s.png")
//		->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
//		->setButtonPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.65f))
//		->show(this);
//	m_Gift->setColor(COLOR::GOLD);
//    m_Gift->setVisible(false);
    
    m_CenterPos = Vec2(visibleSize / 2);
	m_Velocity = Vec2(0, 1);
	m_BulletList = CObjectManager::Instance()->getBulletList();
    this->createFlameParticle();
    this->setScale(0.65f);
    
	return true;
}

void CRocket::Execute(float delta)
{
	m_FSM->Execute(delta);
}

void CRocket::Fly(float speed)
{
    if (!(CItemManager::Instance()->isCurrentItem(eITEM_FLAG_bonustime))) return;
 
    auto oldPos = getPositionX();
    
    auto newPos = oldPos + speed;
    
    if(newPos > m_FlyLimitMin && newPos < m_FlyLimitMax)
        this->setPositionX(newPos);
}

void CRocket::FlyAround(float delta)
{
	m_Time += (m_Direction * (m_Speed / 9 * delta));
	m_TargetPos = CBullet::getCirclePosition(m_Time, m_Distance, m_CenterPos);
	this->seek(delta);
}

void CRocket::FlyAway(float delta)
{
	m_TargetPos = CBullet::getCirclePosition(m_AwayAngle, ROCKET_DEFINE::FLYAWAY_DISTANCE, m_CenterPos);
	this->arrive(delta);
}

void CRocket::FlyToTouchArea(float delta)
{
	m_TargetPos = CGameScene::getGameScene()->getTouchPos();
	this->arrive(delta);
}

void CRocket::FlyToTarget(float delta)
{
    if(!m_Arrive) this->arrive(delta);
}

void CRocket::ComebackHome()
{
    auto centerPos = Director::getInstance()->getVisibleSize() / 2;
    this->setTargetPos(CBullet::getCirclePosition(random<int>(0, 360), this->getDistance(), centerPos));
    this->ChangeState(CFlyToTouchArea::Instance());
}

void CRocket::CollisionCheckAtHome()
{
    for (auto bullet : *m_BulletList)
    {
        if (!bullet->IsAlive()) continue;

        if (bullet->IsHit(this))
        {
			auto value = CItemManager::Instance()->getValueOfCoin((eCOIN_TYPE)(bullet->getSymbol() - 'U' + 1));
			CUserDataManager::Instance()->CoinUpdate(value);
            bullet->R_UpAndBezier();
        }
    }
}

void CRocket::BonusTimeBegan()
{
    this->ChangeState(CFlyToTarget::Instance());
    this->setPosition(Vec2(-50, -50));
    this->setTargetPos(PLAYER_DEFINE::POSITION);
    this->setVelocity(Vec2(800, 300));
    this->setSpeed(800.f);
    
    setArriveCallback([=](cocos2d::Node* sender){
        CObjectManager::Instance()->getPlayer()->TakeOnRocket();
        
        this->scheduleOnce([=](float delta){
            this->setTargetPos(Vec2(_director->getVisibleSize().width * 0.5f,
                                    _director->getVisibleSize().height + 500));
            this->setVelocity(Vec2(0, 1500));
        }, 1.f, "DELAY");
    });
}

void CRocket::BonusTimeEnd()
{
    this->setTargetPos(Vec2(getPositionX(), _director->getVisibleSize().height + 500));
    this->setSpeed(800.f);
    
    // return to game
    this->scheduleOnce([=](float delta){
//        this->ChangeState(CFlyAway::Instance());
        this->setSpeed(ROCKET_DEFINE::SPEED);
    }, 1.f, "BONUSTIMEEND");
}

void CRocket::arrive(float delta)
{
	auto toTarget = m_TargetPos - this->getPosition();

	double dist = toTarget.length();
	if (dist > 1)
	{
		const double DecelerationTweaker = 0.3;
		double speed = dist / ((double)5 * DecelerationTweaker);

		speed = std::min<double>(speed, ROCKET_DEFINE::SPEED_MAX);

		toTarget.normalize();
        Vec2 desiredVelocity = toTarget * speed;
        
        auto steeringForce = Vec2::ZERO;
        steeringForce = (desiredVelocity - m_Velocity);

		Vec2 acceleration = steeringForce / ROCKET_DEFINE::MASS;
		m_Velocity += (acceleration * delta);
		auto nextPos = this->getPosition() + (m_Velocity * delta);
		this->setPosition(nextPos);
		this->rotateToHead();
	}
    this->arriveCheck();
}

void CRocket::seek(float delta)
{
	auto toTarget = m_TargetPos - this->getPosition();

	toTarget.normalize();
	Vec2 desiredVelocity = toTarget * m_Speed;
    
    auto steeringForce = Vec2::ZERO;
    steeringForce = (desiredVelocity - m_Velocity);

	Vec2 acceleration = steeringForce / ROCKET_DEFINE::MASS;
	m_Velocity += (acceleration * delta);
	auto nextPos = this->getPosition() + (m_Velocity * delta);
    
	this->setPosition(nextPos);
	this->rotateToHead();
    this->arriveCheck();
}

void CRocket::rotateToHead()
{
    auto degree = atan2(m_Velocity.x, m_Velocity.y) * 180.f / PI;
    this->setRotation(degree);
}

void CRocket::arriveCheck()
{
    auto rect = Rect(m_TargetPos.x - ROCKET_DEFINE::ARRIVE_RADIUS,
                     m_TargetPos.y - ROCKET_DEFINE::ARRIVE_RADIUS,
                     m_TargetPos.x + ROCKET_DEFINE::ARRIVE_RADIUS,
                     m_TargetPos.y + ROCKET_DEFINE::ARRIVE_RADIUS);
    
	if (!m_Arrive && rect.containsPoint(this->getPosition())){
		m_Arrive = true;
		if (m_ArriveCallback)
		{
			this->retain();
			m_ArriveCallback(this);
			m_ArriveCallback = nullptr;
			this->release();
		}
	}
    else
        m_Arrive = false;
}

void CRocket::createFlameParticle()
{
	m_ParticleFlame = CParticle_Flame::create("fire.png");
    m_ParticleFlame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_ParticleFlame->setGravity(Vec2(90, 0));
	m_ParticleFlame->setRotation(90);
    m_ParticleFlame->setPosition(Vec2(this->getContentSize().width * 0.5f,
                                      this->getContentSize().height * -0.1f));
    m_ParticleFlame->setStartSize(100);
    m_ParticleFlame->setLife(0.8f);
    m_ParticleFlame->setLifeVar(0.15f);
    m_ParticleFlame->setStartColor(Color4F(1.f, 1.f, 0.5f, 1.f));
    m_ParticleFlame->setStartColorVar(Color4F(0, 0, 0.8f, 0));
    m_ParticleFlame->setEndColor(Color4F(1.f, 1.f, 1.f, 0.4f));
    m_ParticleFlame->setEndColorVar(Color4F(0, 0, 0, 0));
    m_ParticleFlame->setPosVar(Vec2(10, 0));
    m_ParticleFlame->setTotalParticles(80);
	this->addChild(m_ParticleFlame);
}

//void CRocket::Gift()
//{
//	m_Gift->setVisible(true);
//}
