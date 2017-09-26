#include "PlayItem.h"
#include "ItemBubble.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../Scene/GameScene.h"
#include "../ObjectManager.h"
#include "../../DataManager/AchievementDataManager.hpp"
#include "../../DataManager/AchievementChecker/AchievementClearChecker.h"

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

void CPlayItem::CollisionWithPlayer()
{
	this->R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
    auto type = (eITEM_TYPE)(this->getSymbol() - 'A' + 1);
	CItemManager::Instance()->StartItemTimer(type);
    switch (type) {
        case eITEM_TYPE_coin:
            GVALUE->COIN_ITEM_USE += 1;
            break;
        case eITEM_TYPE_star:
            GVALUE->STAR_ITEM_USE += 1;
            break;
        case eITEM_TYPE_bonustime:
            GVALUE->BONUS_ITEM_USE += 1;
            break;
        case eITEM_TYPE_giant:
            GVALUE->GIANT_ITEM_USE += 1;
            break;
        case eITEM_TYPE_magnet:
            GVALUE->MAGNET_ITEM_USE += 1;
            break;
        case eITEM_TYPE_shield:
            CAudioManager::Instance()->PlayEffectSound("sounds/Barrier.mp3", false);
            GVALUE->BARRIER_ITEM_USE += 1;
            break;
        default: break;
    }
}
