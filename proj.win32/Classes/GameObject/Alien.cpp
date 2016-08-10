#include "Alien.h"
#include "Planet.h"
#include "MenuSceneObjectManager.h"
#include "../Task/PoolingManager.h"
#include "../AI/States/AlienStates.h"


CAlien::CAlien(sALIEN_PARAM alienParam, float walkingSpeed, float distance)
	: CMover(0.0f)
	, m_AlienParam(alienParam)
	, m_fWalkingSpeed(walkingSpeed)
	, m_fDistance(distance)
	, m_Direction(1)
	, m_FSM(nullptr)
	, m_pPlanet(CMenuSceneObjectManager::Instance()->getPlanet())
    , m_Texture(nullptr)
{
	// bullet이 초기화 될때마다 매번 생성하지 않는다.
	if (m_FSM == nullptr){
		m_FSM = new CStateMachine<CAlien>(this);
	}
	if (m_FSM != nullptr){
		m_FSM->ChangeState(CWanderingState::Instance());
	}

	setCascadeOpacityEnabled(true);
}

CAlien::~CAlien(){
	if (m_FSM != nullptr)
		delete m_FSM;
}

CAlien* CAlien::create(sALIEN_PARAM alienParam,
	float walkingSpeed,
	float distance)
{
	CAlien* pRet =
		(CAlien*)new(std::nothrow)CAlien(alienParam, walkingSpeed, distance);

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


bool CAlien::init()
{
	if (!initVariable())
		return false;
	return true;
}


bool CAlien::initVariable()
{
	try{
		setPositionX((cos(CC_DEGREES_TO_RADIANS(90)) *  m_pPlanet->getBRadius()) + m_pPlanet->getPosition().x);
		setPositionY((sin(CC_DEGREES_TO_RADIANS(90)) *  m_pPlanet->getBRadius()) + m_pPlanet->getPosition().y);

		m_Texture = Sprite::create("player0.png");
		m_Texture->setAnchorPoint(Vec2(0.5f, 0.5f));
		addChild(m_Texture);
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}


/* 회전행렬을 이용하여 오브젝트 회전 및 이동 */
void CAlien::Walk(float delta)
{
    // 회전 속도와 방향을 이용하여 각도를 구하고 라디안으로 변환
	float radian = CC_DEGREES_TO_RADIANS(m_Direction * (m_fWalkingSpeed * delta));

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
//	setRotation(getRotation() - (m_Direction *(m_fWalkingSpeed * delta)));
    
    
    auto Angle = m_Texture->getRotation() - (m_Direction * (m_fWalkingSpeed * 10) * delta);
    m_Texture->setRotation(Angle);
}

void CAlien::ReturnToMemoryBlock()
{
	/*removeFromParent 의 이유 :
	이유는 모든 CMover의 파생 객체들은 메모리 블럭에서 메모리를 할당 받는다.
	그로인해 실행 중 addChild시 같은 메모리를 여러번 addChild할 수 있다.
	때문에 메모리 블럭으로 되돌릴때에는 부모관계를 제거하여야한다.
	또 ReferenceCount를 1 낮춰야 하는 이유도 있다.*/
	this->removeFromParent();
	this->removeAllChildren();
	this->setVisible(false);
	this->setAlive(false);
	CPoolingManager::Instance()->Alien_ReturnToFreeMemory(this);
}

void* CAlien::operator new (size_t size, const std::nothrow_t)
{
	// PoolingManager에서 메모리를 할당 받는다.
	return CPoolingManager::Instance()->AlienNew();
}

void CAlien::Execute(float delta)
{
	getFSM()->Execute(delta);
}

