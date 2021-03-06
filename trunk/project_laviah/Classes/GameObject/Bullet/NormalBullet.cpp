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
#include "../../DataManager/AchievementDataManager.hpp"
#include "../../DataManager/AchievementChecker/AchievementClearChecker.h"
#include "../../DataManager/GradientDataManager.h"

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
    
//    this->setColor(CStageDataManager::getCurrentBulletColor());
    
    return true;
}

void CNormalBullet::Execute(float delta)
{
    CBullet::Execute(delta);
    this->bulletColor();
}

void CNormalBullet::CollisionWithPlayer()
{
	if (CItemManager::Instance()->isCurrentItem(eITEM_FLAG_giant)){
        GVALUE->GIANT_COUNT_TOTAL += 1;
        GVALUE->GIANT_COUNT_BULLET += 1;
        GVALUE->GIANT_SCORE_TOTAL += GVALUE->COMBO_LEVEL;
        GVALUE->GIANT_SCORE_BULLET += GVALUE->COMBO_LEVEL;
		createScoreCurrentPos(GVALUE->COMBO_LEVEL);
		this->R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
	}
	else{
        GVALUE->DEAD_TYPE = PLAYER_DEAD_TYPE::NORMAL_BULLET;
		m_Player->Crushed();
		CCLOG("Bullet Power %f", this->getPower());
		m_Player->LostSomeHealth(this);
	}
}

void CNormalBullet::CollisionWithBarrier()
{
    GVALUE->BARRIER_COUNT += 1;
	GVALUE->BARRIER_SCORE += (GVALUE->COMBO_LEVEL * 2);
	this->createScoreCurrentPos(GVALUE->COMBO_LEVEL * 2);
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
