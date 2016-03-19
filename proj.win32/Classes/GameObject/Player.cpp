#include "Player.h"

CPlayer* CPlayer::create(
	std::string textureName,
	float boundingRadius,
	float angle,
	float rotateSpeed, 
	float maxLife)
{
	CPlayer *pRet = new(std::nothrow) CPlayer(
		textureName
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

CPlayer::CPlayer(std::string textureName, float boundingRadius, float angle, float rotateSpeed, float maxLife)
	: CGameObject(boundingRadius)
	, m_TextureName(textureName)
	, m_fAngle(angle)
	, m_fRotateSpeed(rotateSpeed)
	, m_fMaxLife(maxLife)
	, m_fLife(maxLife)
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
		m_pTexture = Sprite::create(m_TextureName);
		if (m_pTexture != nullptr){
			m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
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
}

void CPlayer::GotSomeHealth(float getHealth)
{
	if (m_fMaxLife >= (m_fLife + getHealth))
	{
		m_fLife = m_fMaxLife;
	}
	else{
		m_fLife += getHealth;
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

// callback ĳ���� ���������� ������
void CPlayer::RotationRight()
{
	m_fAngle = this->getRotation() + m_fRotateSpeed;
	m_fAngle = static_cast<int>(m_fAngle) % 360;
	this->setRotation(m_fAngle);
}

// callback ĳ���� �������� ������
void CPlayer::RotationLeft()
{
	m_fAngle = this->getRotation() - m_fRotateSpeed;
	m_fAngle = static_cast<int>(m_fAngle) % 360;
	this->setRotation(m_fAngle);
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