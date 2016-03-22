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

// callback 평소에 적용되는 생명력 계산함수
float CPlayer::HealthCalculatorInNormal(float delta)
{
	// 5.0f == 가장 레벨이 낮을때 한 번에 빠지는 생명력의 양
	// 이후에 펫 효과나 버프 등과 함께 계산해야한다.
	LostSomeHealth(5.0f * delta);
	return (m_fLife / m_fMaxLife) * 100;
}

// callback 보너스 타임에 적용되는 생명력 계산함수
float CPlayer::HealthCalculatorInBonusTime(float delta)
{
	// 5.0f == 가장 레벨이 낮을때 한 번에 빠지는 생명력의 양
	// 이후에 펫 효과나 버프 등과 함께 계산해야한다.
	// 보너스 타임이기 때문에 더느리게 줄어든다.
	LostSomeHealth(2.5f);
	return (m_fLife / m_fMaxLife) * 100;
}