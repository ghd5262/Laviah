#include "PlayItem.h"
#include "ItemBubble.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../Scene/GameScene.h"
#include "../ObjectManager.h"
#include "../../DataManager/ChallengeDataManager.hpp"
#include "../../DataManager/ChallengeChecker/ChallengeClearChecker.h"

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
        this->createCollisionParticle();
		this->ReturnToMemoryBlock();
	}
}

void CPlayItem::CollisionWithPlayer()
{
//	CAudioManager::Instance()->PlayEffectSound("sounds/Star_2.mp3", false);
	this->R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
    auto type = (eITEM_TYPE)(this->getSymbol() - 'A' + 1);
	CItemManager::Instance()->StartItemTimer(type);
    switch (type) {
        case eITEM_TYPE_coin:       GLOBAL->COIN_ITEM_USE += 1;     break;
        case eITEM_TYPE_star:       GLOBAL->STAR_ITEM_USE += 1;     break;
        case eITEM_TYPE_bonustime:  GLOBAL->BONUS_ITEM_USE += 1;    break;
        case eITEM_TYPE_giant:      GLOBAL->GIANT_ITEM_USE += 1;    break;
        case eITEM_TYPE_magnet:     GLOBAL->MAGNET_ITEM_USE += 1;   break;
        case eITEM_TYPE_shield:     GLOBAL->BARRIER_ITEM_USE += 1;  break;
        default: break;
    }
}
