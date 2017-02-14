#include "StandBullet.hpp"
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

namespace STANDBULLET{
    const static float STAY_LIMIT_TIME = 1.f;
    const static float STANDING_TARGET_RADIUS = 10.f;
    const static float TARGET_POS_DISTANCE = PLANET_DEFINE::BOUNDING_RADIUS + 30;
};

using namespace cocos2d;
using namespace BULLETCREATOR;
using namespace PLANET_DEFINE;
using namespace STANDBULLET;

CStandBullet::CStandBullet()
: m_StayLimitTime(0)
, m_StandUpComplete(false)
, m_PlanetPos(m_Planet->getPosition()){}

CStandBullet* CStandBullet::create()
{
    CStandBullet* pRet = (CStandBullet*)new(std::nothrow)CStandBullet();
    
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

bool CStandBullet::init()
{
    if (!CBullet::init()) return false;
    
    this->setItemEffect(eITEM_FLAG_giant | eITEM_FLAG_coin | eITEM_FLAG_star | eITEM_FLAG_shield);
    
    return true;
}

CBullet* CStandBullet::build()
{
    CBullet::build();
    auto createDistance = BOUNDING_RADIUS * 0.7f;
    this->setPosition(getCirclePosition(getAngle(), createDistance, m_PlanetPos));
    this->setDelayTime(BULLET_STANDARD_DELAY - ((TARGET_POS_DISTANCE - createDistance) / getSpeed()));
    return this;
}

void CStandBullet::Execute(float delta)
{
    m_Time += delta;
    if (!IsTimeUP()) return;
    this->StandUp(delta);
    if (m_StandUpComplete) m_StayLimitTime += delta;
    
    if (m_StayLimitTime > STAY_LIMIT_TIME)
    {
        this->setIsFly(true);
        
        if (IsHit(m_PlanetPos, 30))
        {
            this->ReturnToMemoryBlock();
            return;
        }
    }
    this->setVisible(true);
    m_FSM->Execute(delta);
}

void CStandBullet::StandUp(float delta)
{
    if(m_StandUpComplete) return;
    
    auto targetPos = getCirclePosition(getAngle(), TARGET_POS_DISTANCE, m_PlanetPos);
    if(IsHit(targetPos, STANDING_TARGET_RADIUS))
    {
        m_StandUpComplete = true;
    }
    this->setIsFly(false);
    this->Flee(delta);
}

void CStandBullet::CollisionWithPlayer()
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

void CStandBullet::CollisionWithBarrier()
{
    GLOBAL->BARRIER_COUNT += 1;
    GLOBAL->BARRIER_SCORE += 30;
    this->createScoreCurrentPos(30);
    this->R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
}

void CStandBullet::ChangeToCoin()
{
    float distance = m_TargetVec.distance(getPosition());
	auto bullet = CBulletCreator::CreateBullet('Y', -getRotation(), distance, false);
    if (m_StandUpComplete) bullet->setIsFly(false);
    
    this->ReturnToMemoryBlock();
}

void CStandBullet::ChangeToStar()
{
	float distance = m_TargetVec.distance(getPosition());
	auto bullet = CBulletCreator::CreateBullet('T', -getRotation(), distance, false);
	if (m_StandUpComplete) bullet->setIsFly(false);

	this->ReturnToMemoryBlock();
}