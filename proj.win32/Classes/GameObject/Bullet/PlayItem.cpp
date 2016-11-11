#include "PlayItem.h"
#include "ItemBubble.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../Scene/GameScene.h"
#include "../ObjectManager.h"

CPlayItem::CPlayItem(
	sBULLET_PARAM bulletParam,
	float angle,				    //item 초기 각도 
	float speed)				    //item 초기 속도)
	: CBullet(
	bulletParam,
	angle,
	speed)
{}

CPlayItem* CPlayItem::create(
	sBULLET_PARAM bulletParam,
	float angle,					//item 초기 각도 
	float speed)					//item 초기 속도

{
	CPlayItem* pRet = 
		(CPlayItem*)new(std::nothrow)CPlayItem(
		bulletParam, angle, speed);

	if (pRet && pRet->init())
	{
#if(!USE_MEMORY_POOLING)
        pRet->autorelease();
#endif
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
	setItemEffect(eITEM_FLAG_magnet);
	if (!m_BulletParam._isFly){
		m_BulletParam._fDistance = m_pPlanet->getBRadius() + 20;

		this->scheduleOnce([this](float delta)
		{
			this->R_FadeOutWithCount(5, 3.f);
		}, 5.f, MakeString("AutoRemove_%d", random<int>(1, 100)));
	}
	setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_BulletParam._fDistance) + m_pPlanet->getPosition().x);
	setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_BulletParam._fDistance) + m_pPlanet->getPosition().y);
	setRotation(-m_fAngle);

	m_pTexture = Sprite::create(MakeString("playItem_%d.png", m_BulletParam._itemType));
	if (m_pTexture != nullptr){
		m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
		addChild(m_pTexture);
	}

	if (m_BulletParam._isFly){
        
        auto test = CItemBubble::create(
                            sBULLET_PARAM( 0.f, 0.f, 0.f, false,		//FlyItem 여부
                                        m_BulletParam._isAimingMissile),//AimingMissile 여부
                                        MakeString("itemBubbleIcon_%d.png", m_BulletParam._itemType).c_str(),
                                        -getRotation(),					//초기 각도
                                        this);
        
		CGameScene::getGridWorld()->addChild(test, 100);
        
#if(!USE_MEMORY_POOLING)
        CObjectManager::Instance()->AddBullet(test);
#endif
	}
  
	return true;
}

void CPlayItem::Execute(float delta)
{
	getFSM()->Execute(delta);
}

void CPlayItem::CollisionWithPlanet()
{
	if (true == m_BulletParam._isFly)
	{
		ReturnToMemoryBlock();
	}
}

void CPlayItem::CollisionWithPlayer()
{
//	CAudioManager::Instance()->PlayEffectSound("sounds/Star_2.mp3", false);
	R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
	CItemManager::Instance()->StartItemTimer(m_BulletParam._itemType);
}
