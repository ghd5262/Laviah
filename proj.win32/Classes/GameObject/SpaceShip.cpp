#include "SpaceShip.h"
#include "Planet.h"
#include "Bullet/Bullet.h"
#include "MenuSceneObjectManager.h"
#include "../AI/States/SpaceShipStates.h"
#include "../Particle/Particles.h"
#include "../Scene/GameScene.h"
#include "../GameObject/ObjectManager.h"

using namespace cocos2d;

CSpaceShip::CSpaceShip(sSPACESHIP_PARAM SpaceShipParam)
: m_SpaceshipParam(SpaceShipParam)
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
		m_FSM = new CStateMachine<CSpaceShip>(this);
	}
	if (m_FSM != nullptr){
		m_FSM->ChangeState(CFlyAround::Instance());
	}

	setCascadeOpacityEnabled(true);
}

CSpaceShip::~CSpaceShip(){
	if (m_FSM != nullptr)
		delete m_FSM;
}

CSpaceShip* CSpaceShip::create(sSPACESHIP_PARAM SpaceShipParam)
{
	CSpaceShip* pRet = new(std::nothrow)CSpaceShip(SpaceShipParam);

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


bool CSpaceShip::init()
{
    if (!CGameObject::init())
		return false;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    m_Texture = Sprite::create("spaceship_0.png");
    m_Texture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(m_Texture);
    
	this->setBoundingRadius(SPACESHIP::BOUNDING_RADIUS);

    m_CenterPos = Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.35f);
	m_Velocity = Vec2(0, 1);
	m_BulletList = CObjectManager::Instance()->getBulletList();
    this->createFlameParticle();
    
	return true;
}

void CSpaceShip::Execute(float delta)
{
	getFSM()->Execute(delta);
}

void CSpaceShip::FlyAround(float delta)
{
	m_Time += (m_Direction * (m_Speed / 10 * delta));
	m_TargetPos = CBullet::getCirclePosition(m_Time, m_Distance, m_CenterPos);
	this->seek(delta);
}

void CSpaceShip::FlyAway(float delta)
{
	m_TargetPos = CBullet::getCirclePosition(m_AwayAngle, SPACESHIP::FLYAWAY_DISTANCE, m_CenterPos);
	this->arrive(delta);
}

void CSpaceShip::FlyToTouchArea(float delta)
{
	m_TargetPos = CGameScene::getGameScene()->getTouchPos();
	this->arrive(delta);
}

void CSpaceShip::rotateToHead()
{
	auto degree = atan2(m_Velocity.x, m_Velocity.y) * 180.f / PI;
	this->setRotation(degree);
}

void CSpaceShip::arrive(float delta)
{
	auto toTarget = m_TargetPos - this->getPosition();

	double dist = toTarget.length();
	auto steeringForce = Vec2::ZERO;
	if (dist > 1)
	{
		m_Arrive = false;

		const double DecelerationTweaker = 0.3;

		double speed = dist / ((double)5 * DecelerationTweaker);

		speed = std::min<double>(speed, SPACESHIP::SPEED_MAX);

		toTarget.normalize();
		Vec2 DesiredVelocity = toTarget * speed;

		steeringForce = (DesiredVelocity - m_Velocity);

		Vec2 acceleration = steeringForce / SPACESHIP::MASS;
		m_Velocity += (acceleration * delta);
		auto nextPos = this->getPosition() + (m_Velocity * delta);
		this->setPosition(nextPos);

		this->rotateToHead();
	}

	auto rect = Rect(m_TargetPos.x - SPACESHIP::ARRIVE_RADIUS,
		m_TargetPos.y - SPACESHIP::ARRIVE_RADIUS,
		m_TargetPos.x + SPACESHIP::ARRIVE_RADIUS,
		m_TargetPos.y + SPACESHIP::ARRIVE_RADIUS);

	if (rect.containsPoint(this->getPosition()))
	{
		m_Arrive = true;
	}
}


void CSpaceShip::seek(float delta)
{
	auto toTarget = m_TargetPos - this->getPosition();

	auto steeringForce = Vec2::ZERO;

	toTarget.normalize();
	Vec2 DesiredVelocity = toTarget * m_Speed;

	steeringForce = (DesiredVelocity - m_Velocity);

	Vec2 acceleration = steeringForce / SPACESHIP::MASS;
	m_Velocity += (acceleration * delta);
	auto nextPos = this->getPosition() + (m_Velocity * delta);
	this->setPosition(nextPos);

	this->rotateToHead();

	auto rect = Rect(m_TargetPos.x - SPACESHIP::ARRIVE_RADIUS,
		m_TargetPos.y - SPACESHIP::ARRIVE_RADIUS,
		m_TargetPos.x + SPACESHIP::ARRIVE_RADIUS,
		m_TargetPos.y + SPACESHIP::ARRIVE_RADIUS);

	if (rect.containsPoint(this->getPosition()))
	{
		m_Arrive = true;
	}
	
}

void CSpaceShip::createFlameParticle()
{
	m_ParticleFlame = CParticle_Flame::create("fire.png");
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

void CSpaceShip::Collision()
{
	for (auto bullet : *m_BulletList)
	{
		if (!bullet->IsAlive()) return;
		if (bullet->IsHit(this))
		{
			auto visibleSize = Director::getInstance()->getVisibleSize();
			auto targetPos = Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.96f);
			auto length = Vec2(targetPos - getPosition()).length();
			auto cPos1 = Vec2(getPosition().x - (length * 0.3f), getPosition().y - (length * 0.1f));
			auto cPos2 = Vec2(targetPos.x, targetPos.y - (length * 0.3f));
			bullet->R_BezierWithScale(targetPos, cPos1, cPos2, 1.f, 4.f);
		}
	}
}