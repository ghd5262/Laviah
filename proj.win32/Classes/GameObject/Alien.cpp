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
	// bullet�� �ʱ�ȭ �ɶ����� �Ź� �������� �ʴ´�.
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

		m_Texture = Sprite::createWithSpriteFrameName("player0.png");
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


/* ȸ������� �̿��Ͽ� ������Ʈ ȸ�� �� �̵� */
void CAlien::Walk(float delta)
{
	// ȸ�� �ӵ��� ������ �̿��Ͽ� ������ ���ϰ� �������� ��ȯ
	float radian = CC_DEGREES_TO_RADIANS(m_Direction * (m_fWalkingSpeed * delta));

	// ������ Direction Vector�� �����Ѵ�.
	Vec2 beforeRotation = getPosition() - m_pPlanet->getPosition();

	// �Ÿ��� ����
	float length = beforeRotation.length();

	/* ȸ������� ����
	* rotate x = ((x_ * cos(angle)) - (y_ * sin(angle)))
	* rotate y = ((x_ * sin(angle)) + (y_ * cos(angle))) */
	m_RotationVec = Vec2((float)((beforeRotation.x * cos(radian)) - (beforeRotation.y * sin(radian))),
		(float)((beforeRotation.x * sin(radian)) + (beforeRotation.y * cos(radian))));

	// �븻������
	m_RotationVec.normalize();
	m_RotationVec *= length;

	// ������ ��ǥ�� ���ο� ��ǥ�� �����ش�.
	setPosition(m_pPlanet->getPosition() + m_RotationVec);

	// ������Ʈ ��ü�� ȸ��
	//	setRotation(getRotation() - (m_Direction *(m_fWalkingSpeed * delta)));


	auto Angle = m_Texture->getRotation() - (m_Direction * (m_fWalkingSpeed * 10) * delta);
	m_Texture->setRotation(Angle);
}

void CAlien::ReturnToMemoryBlock()
{
	/*removeFromParent �� ���� :
	������ ��� CMover�� �Ļ� ��ü���� �޸� ������ �޸𸮸� �Ҵ� �޴´�.
	�׷����� ���� �� addChild�� ���� �޸𸮸� ������ addChild�� �� �ִ�.
	������ �޸� ������ �ǵ��������� �θ���踦 �����Ͽ����Ѵ�.
	�� ReferenceCount�� 1 ����� �ϴ� ������ �ִ�.*/
	this->removeFromParent();
	this->removeAllChildren();
	this->setVisible(false);
	this->setAlive(false);
	CPoolingManager::Instance()->Alien_ReturnToFreeMemory(this);
}

void* CAlien::operator new (size_t size, const std::nothrow_t)
{
	// PoolingManager���� �޸𸮸� �Ҵ� �޴´�.
	return CPoolingManager::Instance()->AlienNew();
}

void CAlien::Execute(float delta)
{
	getFSM()->Execute(delta);
}

