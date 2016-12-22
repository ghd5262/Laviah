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

using namespace cocos2d;

CRocket::CRocket(sROCKET_PARAM RocketParam)
: m_RocketParam(RocketParam)
, m_Speed(0.f)
, m_Distance(0.f)
, m_Direction(1)
, m_Time(0.f)
, m_FSM(nullptr)
, m_Texture(nullptr)
, m_Velocity(Vec2::ZERO)
, m_PlayerPos(Vec2::ZERO)
, m_Arrive(false)
, m_ArriveCallback(nullptr)
{}

CRocket::~CRocket(){}

CRocket* CRocket::create(sROCKET_PARAM RocketParam)
{
	CRocket* pRet = new(std::nothrow)CRocket(RocketParam);

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
    
    m_Texture = Sprite::create("spaceship_1.png");
	this->setContentSize(m_Texture->getContentSize());
    m_Texture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_Texture->setPosition(this->getContentSize() / 2);
    addChild(m_Texture);
    
	this->setBoundingRadius(ROCKET::BOUNDING_RADIUS);

    auto player = CObjectManager::Instance()->getPlayer();
    m_PlayerPos = player->getOriginPos();
    m_CenterPos = Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.35f);
	m_Velocity = Vec2(0, 1);
	m_BulletList = CObjectManager::Instance()->getBulletList();
    this->createFlameParticle();
    
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
	m_TargetPos = CBullet::getCirclePosition(m_AwayAngle, ROCKET::FLYAWAY_DISTANCE, m_CenterPos);
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

void CRocket::CollisionCheckAtHome()
{
    for (auto bullet : *m_BulletList)
    {
        if (!bullet->IsAlive()) return;
        if (bullet->IsHit(this))
        {
            auto visibleSize = Director::getInstance()->getVisibleSize();
            auto targetPos = Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.96f);
            auto length = Vec2(targetPos - bullet->getPosition()).length();
            auto cPos1 = Vec2(bullet->getPosition().x - (length * 0.3f),
                              bullet->getPosition().y - 50.f);
            auto cPos2 = Vec2(targetPos.x, targetPos.y - (length * 0.3f));
            auto time = std::max<float>(0.5f, (length / visibleSize.height) * 1.3f);
            
            bullet->R_UpAndBezier(targetPos, cPos1, cPos2, time, 4.f);
        }
    }
}

void CRocket::BonusTimeBegan()
{
    this->ChangeState(CFlyToTarget::Instance());
    this->setPosition(Vec2(-50, -50));
    this->setTargetPos(Vec2(m_PlayerPos.x, m_PlayerPos.y));
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
        this->setSpeed(ROCKET::SPEED);
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

		speed = std::min<double>(speed, ROCKET::SPEED_MAX);

		toTarget.normalize();
        Vec2 desiredVelocity = toTarget * speed;
        
        auto steeringForce = Vec2::ZERO;
        steeringForce = (desiredVelocity - m_Velocity);

		Vec2 acceleration = steeringForce / ROCKET::MASS;
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

	Vec2 acceleration = steeringForce / ROCKET::MASS;
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
    auto rect = Rect(m_TargetPos.x - ROCKET::ARRIVE_RADIUS,
                     m_TargetPos.y - ROCKET::ARRIVE_RADIUS,
                     m_TargetPos.x + ROCKET::ARRIVE_RADIUS,
                     m_TargetPos.y + ROCKET::ARRIVE_RADIUS);
    
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
