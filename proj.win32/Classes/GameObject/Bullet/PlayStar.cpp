#include "PlayStar.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../MyUI/ScoreUI.h"
#include "../../MyUI/UIManager.h"

CPlayStar::CPlayStar(
	eSTAR_TYPE starType,			//star 타입 (브론즈, 실버, 골드 등)
	float boundingRadius,		    //star 충돌 범위
	float distance,					//star 초기 거리
	float angle,				    //star 초기 각도 
	float speed,				    //star 초기 속도
	bool isFly,						//fly star 인지 ground star 인지
	Vec2 createPosition/* = Vec2(0, 0)*/)

	: CBullet(
	"",
	boundingRadius,
	angle,
	speed,
	isFly)
	, m_StarType(starType)
	, m_CreatePos(createPosition)
	, m_pUIScore(nullptr)
	, m_fDistance(distance)
{}

CPlayStar* CPlayStar::create(
	eSTAR_TYPE starType,			//star 타입 (브론즈, 실버, 골드 등)
	float boundingRadius,			//star 충돌 범위
	float distance,					//star 초기 거리
	float angle,					//star 초기 각도 
	float speed,					//star 초기 속도
	bool isFly,						//fly star 인지 ground star 인지
	Vec2 createPosition)
{
	CPlayStar* pRet =
		(CPlayStar*)new(std::nothrow)CPlayStar(
		starType, boundingRadius, distance, angle, speed, isFly, createPosition);

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

bool CPlayStar::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CPlayStar::initVariable()
{
	try{
		setItemEffect(eITEM_FLAG_magnet);

		if (!m_bIsFlyItem)//{
			m_fDistance = m_pPlanet->getBRadius() + 20;

		/*if (m_CreatePos != Vec2(0.f, 0.f)){
		setPosition(m_CreatePos);
		setRotation(-m_fAngle);
		}
		}*/

		setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) * m_fDistance) + m_pPlanet->getPosition().x);
		setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) * m_fDistance) + m_pPlanet->getPosition().y);
		setRotation(-m_fAngle);

		m_pUIScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("StarScoreUI"));

		m_pTexture = Sprite::create(MakeString("star_%d.png", m_StarType));
		m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
		addChild(m_pTexture);

		m_fStarValue = CItemManager::Instance()->getValueOfStar(m_StarType);
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CPlayStar::Execute(float delta)
{
	getFSM()->Execute(delta);
}

void CPlayStar::CollisionWithPlanet()
{
	if (true == m_bIsFlyItem)
	{
		ReturnToMemoryBlock();
	}
}

void CPlayStar::CollisionWithPlayer()
{
	AudioEngine::play2d("sounds/Star_2.mp3", false);
	R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
	m_pUIScore->UpdateValue(m_fStarValue);
}
