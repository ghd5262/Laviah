#include "PlayItem.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"

CPlayItem::CPlayItem(
	std::string textureName,	    //item 이미지
	float boundingRadius,		    //item 충돌 범위
	float angle,				    //item 초기 각도 
	float speed,				    //item 초기 속도
	int itemType,					//item 타입
	bool isFly)						//fly item 인지 ground item 인지

	: CBullet(
	textureName,
	boundingRadius,
	angle,
	speed,
	isFly)
	, m_ItemType(static_cast<eITEM_TYPE>(itemType))	// eITEM_TYPE에서 eITEM_FLAG 로 변환
{}

CPlayItem* CPlayItem::create(
	std::string textureName,		//item 이미지
	float boundingRadius,			//item 충돌 범위
	float angle,					//item 초기 각도 
	float speed,					//item 초기 속도
	int itemType,					//item 타입
	bool isFly)						//fly item 인지 ground item 인지
{
	CPlayItem* pRet = 
		(CPlayItem*)new(std::nothrow)CPlayItem(
		textureName, boundingRadius, angle, speed, itemType, isFly);

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

bool CPlayItem::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CPlayItem::initVariable()
{
	try{
		setItemEffect(eITEM_FLAG_magnet);
		float distance = 1000.f;
		if (!m_bIsFlyItem)
			distance = m_pPlanet->getBRadius() + 20;
		
		setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) * distance) + m_pPlanet->getPosition().x);
		setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) * distance) + m_pPlanet->getPosition().y);
		setRotation(-m_fAngle);

		m_pTexture = Sprite::create(m_TextureName);
		m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
		addChild(m_pTexture);
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CPlayItem::Execute(float delta)
{
	getFSM()->Execute(delta);
}

void CPlayItem::CollisionWithPlanet()
{
	if (true == m_bIsFlyItem)
	{
		ReturnToMemoryBlock();
	}
}

void CPlayItem::CollisionWithPlayer()
{
	AudioEngine::play2d("sounds/Star_2.mp3", false);
	R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
	CItemManager::Instance()->StartItemTimer(m_ItemType);
}
