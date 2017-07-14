#include "PlayCoin.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../MyUI/ScoreUI.h"
#include "../../MyUI/ComboScore.h"
#include "../../Particle/Particles.h"
#include "../../Scene/GameScene.h"

using namespace cocos2d;

CPlayCoin::CPlayCoin(){}

CPlayCoin* CPlayCoin::create()
{
	CPlayCoin* pRet = (CPlayCoin*)new(std::nothrow)CPlayCoin();

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

bool CPlayCoin::init()
{
    if (!CBullet::init()) return false;
    
    this->setItemEffect(eITEM_FLAG_magnet);
    this->setColor(COLOR::GOLD);
    return true;
}

void CPlayCoin::CollisionWithPlayer()
{
//	CAudioManager::Instance()->PlayEffectSound("sounds/Star_2.mp3", false);
    this->R_UpAndBezier();
    auto value = CItemManager::Instance()->getValueOfCoin((eCOIN_TYPE)(this->getSymbol() - 'U' + 1));
    CComboScore::Instance()->AddCombo();
    GVALUE->COIN_SCORE += value;
	GVALUE->COIN_COUNT += 1;
}
