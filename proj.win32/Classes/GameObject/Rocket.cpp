#include "Rocket.h"
#include "Planet.h"
#include "Bullet/Bullet.h"
#include "MenuSceneObjectManager.h"
#include "../AI/States/RocketStates.h"
#include "../Particle/Particles.h"
#include "../Scene/GameScene.h"
#include "../GameObject/ObjectManager.h"
#include "../GameObject/ItemManager.h"

using namespace cocos2d;

CRocket::CRocket(sROCKET_PARAM RocketParam)
: m_RocketParam(RocketParam)
, m_Speed(0.f)
, m_ActionTime(0.f)
, m_Distance(0.f)
, m_Direction(1)
, m_Time(0.f)
, m_FSM(nullptr)
, m_Texture(nullptr)
, m_Velocity(Vec2::ZERO)
, m_Arrive(false)
{
	if (m_FSM == nullptr){
		m_FSM = new CStateMachine<CRocket>(this);
	}
	if (m_FSM != nullptr){
		m_FSM->ChangeState(CFlyAround::Instance());
	}

	setCascadeOpacityEnabled(true);
}

CRocket::~CRocket(){
	if (m_FSM != nullptr)
		delete m_FSM;
}

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
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    m_FlyLimitMax = visibleSize.width * 0.9f;
    m_FlyLimitMin = visibleSize.width * 0.1f;
    
    m_Texture = Sprite::create("spaceship_0.png");
    m_Texture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(m_Texture);
    
	this->setBoundingRadius(ROCKET::BOUNDING_RADIUS);

    m_CenterPos = Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.35f);
	m_Velocity = Vec2(0, 1);
	m_BulletList = CObjectManager::Instance()->getBulletList();
    this->createFlameParticle();
    
	return true;
}

void CRocket::Execute(float delta)
{
	getFSM()->Execute(delta);
}

void CRocket::Fly(float dir, float delta)
{
    if (!(CItemManager::Instance()->isCurrentItem(eITEM_FLAG_bonustime))) return;
 
    auto oldPos = getPositionX();
    
    auto newPos = oldPos + (dir * (m_Speed * delta));
    
    if(newPos > m_FlyLimitMin && newPos < m_FlyLimitMax)
        this->setPositionX(newPos);
}

void CRocket::FlyAround(float delta)
{
	m_Time += (m_Direction * (m_Speed / 10 * delta));
	m_TargetPos = CBullet::getCirclePosition(m_Time, m_Distance, m_CenterPos);
	this->seek(delta);
}

void CRocket::FlyAway(float delta)
{
    if ((CItemManager::Instance()->isCurrentItem(eITEM_FLAG_bonustime))) return;

	m_TargetPos = CBullet::getCirclePosition(m_AwayAngle, ROCKET::FLYAWAY_DISTANCE, m_CenterPos);
	this->arrive(delta);
}

void CRocket::FlyToTouchArea(float delta)
{
	m_TargetPos = CGameScene::getGameScene()->getTouchPos();
	this->arrive(delta);
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
    
    if (rect.containsPoint(this->getPosition()))
        m_Arrive = true;
    else
        m_Arrive = false;
}

void CRocket::createFlameParticle()
{
    m_ParticleFlame = CParticle_Flame::create();
    m_ParticleFlame->setTextureName("fire.png");
    m_ParticleFlame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_ParticleFlame->setGravity(Vec2(0, -270));
    m_ParticleFlame->setPosition(Vec2(m_Texture->getContentSize().width * 0.5f,
                                      m_Texture->getContentSize().height * -0.1f));
    m_ParticleFlame->setStartSize(100);
    m_ParticleFlame->setLife(0.8f);
    m_ParticleFlame->setLifeVar(0.15f);
    m_ParticleFlame->setStartColor(Color4F(1.f, 1.f, 0.5f, 1.f));
    m_ParticleFlame->setStartColorVar(Color4F(0, 0, 0.8f, 0));
    m_ParticleFlame->setEndColor(Color4F(1.f, 1.f, 1.f, 0.4f));
    m_ParticleFlame->setEndColorVar(Color4F(0, 0, 0, 0));
    m_ParticleFlame->setPosVar(Vec2(10, 0));
    m_ParticleFlame->setTotalParticles(80);
    m_Texture->addChild(m_ParticleFlame);
}
