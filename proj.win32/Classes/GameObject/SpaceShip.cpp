#include "SpaceShip.h"
#include "Planet.h"
#include "MenuSceneObjectManager.h"
#include "../AI/States/SpaceShipStates.h"
#include "../Particle/Particles.h"

CSpaceShip::CSpaceShip(sSPACESHIP_PARAM SpaceShipParam, float flySpeed, float distance)
	: CGameObject(0.f)
	, m_SpaceshipParam(SpaceShipParam)
	, m_fFlySpeed(flySpeed)
	, m_fDistance(distance)
	, m_Direction(1)
	, m_FSM(nullptr)
	, m_pPlanet(CMenuSceneObjectManager::Instance()->getPlanet())
{
	// bullet이 초기화 될때마다 매번 생성하지 않는다.
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
	try{
		setPositionX((cos(CC_DEGREES_TO_RADIANS(90)) *  /*m_BulletParam._fDistance*/ 300) + m_pPlanet->getPosition().x);
		setPositionY((sin(CC_DEGREES_TO_RADIANS(90)) *  /*m_BulletParam._fDistance*/ 300) + m_pPlanet->getPosition().y);
		setRotation(-90);

		auto texture = Sprite::create("whiteSquare.png");
		texture->setAnchorPoint(Vec2(0.5f, 0.5f));
		addChild(texture);

		// 불꽃 파티클
		m_pParticleFlame = CParticle_Flame::create("missileFlame.png");
		if (m_pParticleFlame != nullptr){
			m_pParticleFlame->retain();
			m_pParticleFlame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_pParticleFlame->setAngle(-90);
			m_pParticleFlame->setGravity(Vec2(90, 0));
			m_pParticleFlame->setPosition(Vec2(texture->getContentSize().width * 1.1f, texture->getContentSize().height * 0.5f));
			m_pParticleFlame->setStartSpin(-90);
			m_pParticleFlame->setStartSpin(270);
			m_pParticleFlame->setLife(0.1f);
			m_pParticleFlame->setLifeVar(0.15f);

			texture->addChild(m_pParticleFlame);
		}
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}


/* 회전행렬을 이용하여 오브젝트 회전 및 이동 */
void CSpaceShip::FlyAround(float delta)
{
	// 회전 속도와 방향을 이용하여 각도를 구하고 라디안으로 변환
	float radian = CC_DEGREES_TO_RADIANS(m_Direction * (m_fFlySpeed * delta));

	// 현재의 Direction Vector를 저장한다.
	Vec2 beforeRotation = getPosition() - m_pPlanet->getPosition();

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
	setPosition(m_pPlanet->getPosition() + m_RotationVec);

	// 오브젝트 자체도 회전
	setRotation(getRotation() - (m_Direction *(m_fFlySpeed * delta)));
}

void CSpaceShip::Execute(float delta)
{
	getFSM()->Execute(delta);
}

