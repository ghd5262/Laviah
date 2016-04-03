#include "PlayCoin.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../MyUI/ScoreUI.h"
#include "../../MyUI/UIManager.h"

CPlayCoin::CPlayCoin(
	eCOIN_TYPE coinType,			//coin 타입 (브론즈, 실버, 골드 등)
	float boundingRadius,		    //coin 충돌 범위
	float distance,					//coin 초기 거리
	float angle,				    //coin 초기 각도 
	float speed,				    //coin 초기 속도
	bool isFly,						//fly coin 인지 ground coin 인지
	Vec2 createPosition /*= Vec2(0, 0)*/)
	
	: CBullet(
	"",
	boundingRadius,
	angle,
	speed,
	isFly)
	, m_CoinType(coinType)
	, m_CreatePos(createPosition)
	, m_pUIScore(nullptr)
	, m_fDistance(distance)
{}

CPlayCoin* CPlayCoin::create(
	eCOIN_TYPE coinType,			//coin 타입 (브론즈, 실버, 골드 등)
	float boundingRadius,			//coin 충돌 범위
	float distance,					//coin 초기 거리
	float angle,					//coin 초기 각도 
	float speed,					//coin 초기 속도
	bool isFly,						//fly coin 인지 ground coin 인지
	Vec2 createPosition)
{
	CPlayCoin* pRet =
		(CPlayCoin*)new(std::nothrow)CPlayCoin(
		coinType, boundingRadius, distance, angle, speed, isFly, createPosition);

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

bool CPlayCoin::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CPlayCoin::initVariable()
{
	try{
		setItemEffect(eITEM_FLAG_magnet);

		if (!m_bIsFlyItem)
			m_fDistance = m_pPlanet->getBRadius() + 20;

		setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) * m_fDistance) + m_pPlanet->getPosition().x);
		setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) * m_fDistance) + m_pPlanet->getPosition().y);
		setRotation(-m_fAngle);
		

		m_pUIScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("CoinScoreUI"));

		m_pTexture = Sprite::create(MakeString("coin_%d.png", m_CoinType));
		m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
		addChild(m_pTexture);

		m_fCoinValue = CItemManager::Instance()->getValueOfCoin(m_CoinType);
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CPlayCoin::Execute(float delta)
{
	getFSM()->Execute(delta);
}

void CPlayCoin::CollisionWithPlanet()
{
	if (true == m_bIsFlyItem)
	{
		ReturnToMemoryBlock();
	}
}

void CPlayCoin::CollisionWithPlayer()
{
	AudioEngine::play2d("sounds/Star_2.mp3", false);
	R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
	m_pUIScore->UpdateValue(m_fCoinValue);
}
