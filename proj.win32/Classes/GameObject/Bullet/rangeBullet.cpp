#include "RangeBullet.hpp"
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

namespace RANGEBULLET{
    const static float STANDING_TARGET_RADIUS = 10.f;
};

using namespace cocos2d;
using namespace BULLETCREATOR;
using namespace PLANET_DEFINE;
using namespace RANGEBULLET;

CRangeBullet::CRangeBullet()
: m_Range(0.f)
, m_TargetDistance(0.f)
, m_StayTime(0.f)
, m_StayLimitTime(0.f)
, m_Movable(true)
, m_PlanetPos(m_Planet->getPosition())
{}

CRangeBullet* CRangeBullet::create()
{
    CRangeBullet* pRet = (CRangeBullet*)new(std::nothrow)CRangeBullet();
    
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

bool CRangeBullet::init()
{
    if (!CBullet::init()) return false;
    
    return true;
}

CBullet* CRangeBullet::setRange(float range)
{
    m_Range = range;
    return this;
}

CBullet* CRangeBullet::setTarget(float target)
{
    m_TargetDistance = target;
    return this;
}

CBullet* CRangeBullet::setStayLimitTime(float stayLimitTime)
{
    m_StayLimitTime = stayLimitTime;
    return this;
}

void CRangeBullet::Execute(float delta)
{
    this->sizeUP(delta);
    
    if (!m_Movable) m_StayTime += delta;
    
    if (m_StayTime > m_StayLimitTime)
    {
        m_ArriveRange = false;
        if (IsHit(m_PlanetPos, m_TargetDistance))
        {
            this->ReturnToMemoryBlock();
            return;
        }
    }
}

void CRangeBullet::sizeUP(float delta)
{
    if(!m_Movable) return;
    
    auto targetPos = getCirclePosition(getAngle(), m_Range, m_PlanetPos);
    if(IsHit(targetPos, STANDING_TARGET_RADIUS))
    {
        m_ArriveToRange = true;
    }
    this->Flee(delta);
}