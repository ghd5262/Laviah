#include "NormalBullet.h"
#include "PlayCoin.h"
#include "PlayStar.h"
#include "ScoreBullet.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../BulletCreator.h"
#include "../../Particle/Particles.h"
#include "../../Scene/GameScene.h"
#include "../../MyUI/ScoreUI.h"
#include "../../MyUI/ComboScore.h"
#include "../../DataManager/ChallengeDataManager.hpp"
#include "../../DataManager/ChallengeChecker/ChallengeClearChecker.h"

using namespace cocos2d;

CNormalBullet::CNormalBullet(){}

CNormalBullet* CNormalBullet::create()
{
	CNormalBullet* pRet = (CNormalBullet*)new(std::nothrow)CNormalBullet();

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

bool CNormalBullet::init()
{
    if (!CBullet::init()) return false;
    
    this->setItemEffect(eITEM_FLAG_giant | eITEM_FLAG_coin |
                        eITEM_FLAG_star | eITEM_FLAG_shield);
    
    return true;
}

void CNormalBullet::CollisionWithPlayer()
{
	if (CItemManager::Instance()->isCurrentItem(eITEM_FLAG_giant)){
        GLOBAL->GIANT_COUNT_TOTAL += 1;
        GLOBAL->GIANT_COUNT_BULLET += 1;
        GLOBAL->GIANT_SCORE_TOTAL += 10;
        GLOBAL->GIANT_SCORE_BULLET += 10;
		createScoreCurrentPos(10);
		this->R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
	}
	else{
		m_Player->Crushed();
		CCLOG("Bullet Power %f", this->getPower());
		m_Player->LostSomeHealth(this->getPower());
        this->ReturnToMemoryBlock();
	}
}

void CNormalBullet::CollisionWithBarrier()
{
    GLOBAL->BARRIER_COUNT += 1;
	GLOBAL->BARRIER_SCORE += (GLOBAL->COMBO_LEVEL * 2);
	this->createScoreCurrentPos(GLOBAL->COMBO_LEVEL * 2);
	this->R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
}

void CNormalBullet::ChangeToCoin()
{
	float distance = m_TargetVec.distance(getPosition());
	CBulletCreator::CreateBullet('U', -getRotation(), distance, false);
    
    this->ReturnToMemoryBlock();
}

void CNormalBullet::ChangeToStar()
{
	float distance = m_TargetVec.distance(getPosition());
	CBulletCreator::CreateBullet('P', -getRotation(), distance, false);

	this->ReturnToMemoryBlock();
}