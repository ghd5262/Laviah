#include "PlayItem.h"
#include "ItemBubble.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../Scene/GameScene.h"
#include "../ObjectManager.h"

using namespace cocos2d;

CPlayItem::CPlayItem()
{}

CPlayItem* CPlayItem::create()
{
	CPlayItem* pRet = (CPlayItem*)new(std::nothrow)CPlayItem();

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
    if (!CBullet::init()) return false;

    this->setItemEffect(eITEM_FLAG_magnet);

    return true;
}

void CPlayItem::CollisionWithPlanet()
{
	if (this->getIsFly())
	{
		this->ReturnToMemoryBlock();
	}
}

void CPlayItem::CollisionWithPlayer()
{
//	CAudioManager::Instance()->PlayEffectSound("sounds/Star_2.mp3", false);
	this->R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
//	CItemManager::Instance()->StartItemTimer(getBulletInfo()._itemType);
}
