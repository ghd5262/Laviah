#include "SpaceShip.h"
#include "Planet.h"
#include "MenuSceneObjectManager.h"
#include "../AI/States/SpaceShipStates.h"
#include "../Particle/Particles.h"

using namespace cocos2d;

CSpaceShip::CSpaceShip(sSPACESHIP_PARAM SpaceShipParam, float flySpeed, float distance)
	: CGameObject(0.f)
	, m_SpaceshipParam(SpaceShipParam)
	, m_fFlySpeed(flySpeed)
	, m_fDistance(distance)
	, m_Direction(1)
	, m_FSM(nullptr)
	, m_pPlanet(CMenuSceneObjectManager::Instance()->getPlanet())
{
	if (m_FSM == nullptr){
		m_FSM = new CStateMachine<CSpaceShip>(this);
	}
	if (m_FSM != nullptr){
		m_FSM->ChangeState(CSpaceShipFlyingState::Instance());
	}

	setCascadeOpacityEnabled(true);
}

CSpaceShip::~CSpaceShip(){
	if (m_FSM != nullptr)
		delete m_FSM;
}

CSpaceShip* CSpaceShip::create(sSPACESHIP_PARAM SpaceShipParam,
	float flySpeed,
	float distance)
{
	CSpaceShip* pRet =
		(CSpaceShip*)new(std::nothrow)CSpaceShip(SpaceShipParam, flySpeed, distance);

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
	if (!initVariable())
		return false;
	return true;
}


bool CSpaceShip::initVariable()
{

	setPositionX((cos(CC_DEGREES_TO_RADIANS(90)) *  /*m_BulletParam._fDistance*/ m_fDistance) + m_pPlanet->getPosition().x);
	setPositionY((sin(CC_DEGREES_TO_RADIANS(90)) *  /*m_BulletParam._fDistance*/ m_fDistance) + m_pPlanet->getPosition().y);

	auto texture = Sprite::create("spaceship_0.png");
	texture->setAnchorPoint(Vec2(0.5f, 0.5f));
	addChild(texture);

	m_pParticleFlame = CParticle_Flame::create("fire.png");
	if (m_pParticleFlame != nullptr){
		m_pParticleFlame->retain();
		m_pParticleFlame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_pParticleFlame->setGravity(Vec2(90, 0));
		m_pParticleFlame->setPosition(Vec2(texture->getContentSize().width * 1.1f, texture->getContentSize().height * 0.5f));
		m_pParticleFlame->setStartSize(100);
		m_pParticleFlame->setLife(0.8f);
		m_pParticleFlame->setLifeVar(0.15f);
		m_pParticleFlame->setStartColor(Color4F(1.f, 1.f, 0.5f, 1.f));
		m_pParticleFlame->setStartColorVar(Color4F(0, 0, 0.8f, 0));
		m_pParticleFlame->setEndColor(Color4F(1.f, 1.f, 1.f, 0.4f));
		m_pParticleFlame->setEndColorVar(Color4F(0, 0, 0, 0));
		m_pParticleFlame->setPosVar(Vec2(0, 10));
		m_pParticleFlame->setTotalParticles(80);
		texture->addChild(m_pParticleFlame);
	}

	return true;
}


void CSpaceShip::FlyAround(float delta)
{
	float radian = CC_DEGREES_TO_RADIANS(m_Direction * (m_fFlySpeed * delta));

	Vec2 beforeRotation = getPosition() - m_pPlanet->getPosition();

	float length = beforeRotation.length();

	m_RotationVec = Vec2((float)((beforeRotation.x * cos(radian)) - (beforeRotation.y * sin(radian))),
		(float)((beforeRotation.x * sin(radian)) + (beforeRotation.y * cos(radian))));

	m_RotationVec.normalize();
	m_RotationVec *= length;

	setPosition(m_pPlanet->getPosition() + m_RotationVec);

	setRotation(getRotation() - (m_Direction *(m_fFlySpeed * delta)));
}

void CSpaceShip::Execute(float delta)
{
	getFSM()->Execute(delta);
}

