#include "PlayCoin.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../MyUI/ScoreUI.h"
#include "../../MyUI/MultipleScore.h"
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
      
    return true;
}

void CPlayCoin::CollisionWithPlayer()
{
	CAudioManager::Instance()->PlayEffectSound("sounds/Star_2.mp3", false);

    auto visibleSize = Director::getInstance()->getVisibleSize();
	auto targetPos = Vec2(visibleSize.width * 0.055f, visibleSize.height * 0.925f);
    auto length = Vec2(targetPos - this->getPosition()).length();
    auto cPos1 = Vec2(this->getPosition().x - (length * 0.3f),
                      this->getPosition().y - 50.f);
    auto cPos2 = Vec2(targetPos.x, targetPos.y - (length * 0.3f));
    auto time = std::max<float>(0.5f, (length / visibleSize.height) * 1.3f);
    
    this->R_UpAndBezier(targetPos, cPos1, cPos2, time, 4.f);
    auto value = CItemManager::Instance()->getValueOfCoin((eCOIN_TYPE)(this->getSymbol() - 'U' + 1));
    CMultipleScore::Instance()->AddScore(value);
    GLOBAL->COIN_SCORE += value;
	GLOBAL->COIN_COUNT += 1;
}
