#include "StickBullet.h"
#include "PlayCoin.h"
#include "PlayStar.h"
#include "ScoreBullet.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../Scene/GameScene.h"
#include "../../MyUI/ScoreUI.h"
#include "../../MyUI/MultipleScore.h"
#include "../BulletCreator.h"
#include "../../DataManager/ChallengeDataManager.hpp"
#include "../../DataManager/ChallengeChecker/ChallengeClearChecker.h"

using namespace cocos2d;

namespace STICKBULLET{
	const static float STAY_LIMIT_TIME = 1.0f;
};

CStickBullet::CStickBullet()
	: m_StayLimitTime(0)
	, m_HitWithPlanet(false)
{}

CStickBullet* CStickBullet::create()
{
    CStickBullet* pRet = (CStickBullet*)new(std::nothrow)CStickBullet();
    
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

bool CStickBullet::init()
{
    if (!CBullet::init()) return false;

    this->setItemEffect(eITEM_FLAG_giant | eITEM_FLAG_coin | eITEM_FLAG_star | eITEM_FLAG_shield);
    
    return true;
}

void CStickBullet::Execute(float delta)
{
	m_Time += delta;
    if (!IsTimeUP()) return;
    if (m_HitWithPlanet) m_StayLimitTime += delta;

	if (m_StayLimitTime > STICKBULLET::STAY_LIMIT_TIME)
	{
		this->setIsFly(true);

		if (IsHit(m_Planet->getPosition(), m_Planet->getBoundingRadius() * 0.3f))
		{
			this->ReturnToMemoryBlock();
			return;
		}
	}
    this->setVisible(true);
    m_FSM->Execute(delta);
}

void CStickBullet::CollisionWithPlanet()
{
	if (m_HitWithPlanet == false)
	{
		m_HitWithPlanet = true;
		this->setIsFly(false);
		this->setSpeed(300.f);
	}
}

void CStickBullet::CollisionWithPlayer()
{
    if (CItemManager::Instance()->isCurrentItem(eITEM_FLAG_giant)){
        GLOBAL->GIANT_COUNT_STICK += 1;
        GLOBAL->GIANT_SCORE_STICK += 30;
        this->createScoreCurrentPos(30);
        this->R_BezierWithRotation(Vec2(1180, 2020), Vec2(350, 900), Vec2(450, 1200), 0.5f);
    }
    else{
        m_Player->StackedRL(0.1f, 10, 10, 5);
        m_Player->LostSomeHealth(this->getPower());
        this->ReturnToMemoryBlock();
    }
}

void CStickBullet::CollisionWithBarrier()
{
    GLOBAL->BARRIER_COUNT += 1;
    GLOBAL->BARRIER_SCORE += 30;
    this->createScoreCurrentPos(30);
    this->R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
}

void CStickBullet::ChangeToCoin()
{
	float distance = m_TargetVec.distance(getPosition());
	auto bullet = CBulletCreator::CreateBullet('Y', -getRotation(), distance, false);
	if (m_HitWithPlanet) bullet->setIsFly(false);

	this->ReturnToMemoryBlock();
}

void CStickBullet::ChangeToStar()
{
	float distance = m_TargetVec.distance(getPosition());
	auto bullet = CBulletCreator::CreateBullet('T', -getRotation(), distance, false);
	if (m_HitWithPlanet) bullet->setIsFly(false);

	this->ReturnToMemoryBlock();
}