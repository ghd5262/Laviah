#include "Player.h"
#include "../AI/States/PlayerStates.h"

CPlayer* CPlayer::create(
	std::string normalTextureName,
	std::string giantTextureName,
	float boundingRadius,
	float angle,
	float rotateSpeed, 
	float maxLife)
{
	CPlayer *pRet = new(std::nothrow) CPlayer(
		normalTextureName
		, giantTextureName
		, boundingRadius
		, angle
		, rotateSpeed
		, maxLife);
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

CPlayer::CPlayer(
	std::string normalTextureName,
	std::string giantTextureName,
	float boundingRadius,
	float angle,
	float rotateSpeed,
	float maxLife)

	: CGameObject(boundingRadius)
	, m_NormalTextureName(normalTextureName)
	, m_GiantTextureName(giantTextureName)
	, m_fAngle(angle)
	, m_fRotateSpeed(rotateSpeed)
	, m_fMaxLife(maxLife)
	, m_fLife(maxLife)
	, m_fMagnetLimitRadius(200.f)
{
}

bool CPlayer::init()
{
	//this->DrawDebugBinding();
	if (!initVariable())
		return false;
	return true;
}

bool CPlayer::initVariable()
{
	try{
		m_FSM = new CStateMachine<CPlayer>(this);
		if (m_FSM != nullptr){
			m_FSM->ChangeState(CPlayerNormal::Instance());
		}

		m_pTexture = Sprite::create(m_NormalTextureName);
		if (m_pTexture != nullptr){
			m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
			m_pTexture->setScale(0.5f);
			addChild(m_pTexture);
		}
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CPlayer::Execute(float delta)
{
	m_FSM->Execute(delta);
}

void CPlayer::GotSomeHealth(float health)
{
	if (m_fMaxLife >= (m_fLife + health))
	{
		m_fLife = m_fMaxLife;
	}
	else{
		m_fLife += health;
	}
}

void CPlayer::LostSomeHealth(float loseHealth)
{
	if (0.f < (m_fLife - loseHealth))
	{
		m_fLife -= loseHealth;
	}
	else{
		m_fLife = 0.f;
	}
}

// Dir -1 == Left, 1 == Right
void CPlayer::Rotation(int dir)
{
	m_fAngle = this->getRotation() + (dir * m_fRotateSpeed);
	m_fAngle = static_cast<int>(m_fAngle) % 360;
	this->setRotation(m_fAngle);
}

void CPlayer::GiantMode()
{
	auto action = Sequence::create(
		ScaleTo::create(0.5f, 3.0f),
		CallFunc::create([&](){
		this->m_pTexture->setTexture(m_GiantTextureName);
		this->setBRadius(60.f);
	}), nullptr);
	this->runAction(action);
}

void CPlayer::NormalMode()
{
	auto action = Sequence::create(
		ScaleTo::create(0.5f, 1.0f),
		CallFunc::create([&](){
		this->m_pTexture->setTexture(m_NormalTextureName);
		this->setBRadius(6.f);
	}), nullptr);
	this->runAction(action);
}

// callback ��ҿ� ����Ǵ� ����� ����Լ�
float CPlayer::HealthCalculatorInNormal(float delta)
{
	// 5.0f == ���� ������ ������ �� ���� ������ ������� ��
	// ���Ŀ� �� ȿ���� ���� ��� �Բ� ����ؾ��Ѵ�.
	LostSomeHealth(5.0f * delta);
	return (m_fLife / m_fMaxLife) * 100;
}

// callback ���ʽ� Ÿ�ӿ� ����Ǵ� ����� ����Լ�
float CPlayer::HealthCalculatorInBonusTime(float delta)
{
	// 5.0f == ���� ������ ������ �� ���� ������ ������� ��
	// ���Ŀ� �� ȿ���� ���� ��� �Բ� ����ؾ��Ѵ�.
	// ���ʽ� Ÿ���̱� ������ �������� �پ���.
	LostSomeHealth(2.5f);
	return (m_fLife / m_fMaxLife) * 100;
}