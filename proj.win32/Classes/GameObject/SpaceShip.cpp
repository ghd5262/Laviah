#include "SpaceShip.h"
#include "Planet.h"
#include "Bullet/Bullet.h"
#include "MenuSceneObjectManager.h"
#include "../AI/States/SpaceShipStates.h"
#include "../Particle/Particles.h"

using namespace cocos2d;

CSpaceShip::CSpaceShip(sSPACESHIP_PARAM SpaceShipParam)
: m_SpaceshipParam(SpaceShipParam)
, m_Speed(0.f)
, m_ActionTime(0.f)
, m_Direction(1)
, m_Time(0.f)
, m_FSM(nullptr)
, m_Texture(nullptr)
, m_Velocity(Vec2::ZERO)
, m_Planet(CMenuSceneObjectManager::Instance()->getPlanet())
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
    
    m_CenterPos = Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.35f);
    
    
    this->schedule([=](float delta){
        m_TargetPos = Vec2(random<float>(0, 1280.f), random<float>(0, 1920.f));
    }, 5.f, "changeTargetPos");
    this->createFlameParticle();
    
	return true;
}

void CSpaceShip::FlyAround(float delta)
{
//	float radian = CC_DEGREES_TO_RADIANS(m_Direction * (m_Speed * delta));
//
//	Vec2 beforeRotation = getPosition() - m_CenterPos;
//
//	float length = beforeRotation.length();
//
//	m_RotationVec = Vec2((float)((beforeRotation.x * cos(radian)) - (beforeRotation.y * sin(radian))),
//                         (float)((beforeRotation.x * sin(radian)) + (beforeRotation.y * cos(radian))));
//
//	m_RotationVec.normalize();
//	m_RotationVec *= length;
//
//	m_TargetPos = m_CenterPos + m_RotationVec;
    
    
    
    m_Time += (m_Direction * (m_Speed * delta));
    m_TargetPos = CBullet::getCirclePosition(m_Time, m_Distance, m_CenterPos);
}

void CSpaceShip::Execute(float delta)
{
	//getFSM()->Execute(delta);
    this->seek(delta);
}

void CSpaceShip::createFlameParticle()
{
    m_ParticleFlame = CParticle_Flame::create("fire.png");
    m_ParticleFlame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_ParticleFlame->setGravity(Vec2(0, -90));
    m_ParticleFlame->setPosition(Vec2(m_Texture->getContentSize().width * 0.5f,
                                       m_Texture->getContentSize().height * -0.1f));
    m_ParticleFlame->setStartSize(100);
    m_ParticleFlame->setLife(0.8f);
    m_ParticleFlame->setLifeVar(0.15f);
    m_ParticleFlame->setStartColor(Color4F(1.f, 1.f, 0.5f, 1.f));
    m_ParticleFlame->setStartColorVar(Color4F(0, 0, 0.8f, 0));
    m_ParticleFlame->setEndColor(Color4F(1.f, 1.f, 1.f, 0.4f));
    m_ParticleFlame->setEndColorVar(Color4F(0, 0, 0, 0));
    m_ParticleFlame->setPosVar(Vec2(0, 10));
    m_ParticleFlame->setTotalParticles(80);
    m_Texture->addChild(m_ParticleFlame);

}

/* 조종행동 Seek - 오브젝트를 목표지점까지 이동 */
void CSpaceShip::seek(float delta)
{
    // 방향벡터 구하기
    Vec2 dir = m_TargetPos - getPosition();
    dir.normalize();
    
    // 속력벡터 계산
//    dir *= (m_Speed * delta);
    
    auto desiredVelocity = dir - m_Velocity;
    
    m_Velocity = desiredVelocity * (m_Speed * delta);
    
    // 현재 좌표에 적용
    //setPosition(getPosition() + dir);
    setPosition(getPosition() + m_Velocity);
    
//    Vector2D TargetVelocity = Vec2DNormalize(GoalPos - m_pVehicle->Pos()) * m_pVehicle->MaxSpeed();
    // return value는 조종힘
//     조종힘 구하는 공식 : D = T - C
//    return (TargetVelocity - m_pVehicle->Velocity());
    
    
    auto radian = atan2(m_Velocity.x, m_Velocity.y);
    auto degree = CC_RADIANS_TO_DEGREES(radian);
    this->setRotation(degree);
}
