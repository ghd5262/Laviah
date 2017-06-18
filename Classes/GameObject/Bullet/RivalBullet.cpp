#include "RivalBullet.hpp"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../ItemManager.h"
#include "../../Scene/GameScene.h"
#include "../../MyUI/UrlSprite.hpp"
#include "../../Facebook/FacebookManager.hpp"

using namespace cocos2d;

namespace RIVALBULLET{
    const static float STAY_LIMIT_TIME = 5.0f;
};

CRivalBullet::CRivalBullet()
: m_StayLimitTime(0)
, m_HitWithPlanet(false){}

CRivalBullet* CRivalBullet::create()
{
    CRivalBullet* pRet = (CRivalBullet*)new(std::nothrow)CRivalBullet();
    
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

bool CRivalBullet::init()
{
    if (!CBullet::init()) return false;
    
    this->setItemEffect(eITEM_FLAG_none);
    
    return true;
}

void CRivalBullet::Execute(float delta)
{
    m_Time += delta;
    if (!IsTimeUP()) return;
    if (m_HitWithPlanet) m_StayLimitTime += delta;
    
    if (m_StayLimitTime > RIVALBULLET::STAY_LIMIT_TIME)
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

void CRivalBullet::CollisionWithPlanet()
{
    if (m_HitWithPlanet == false)
    {
        m_HitWithPlanet = true;
        this->setIsFly(false);
        this->setSpeed(300.f);
    }
}

void CRivalBullet::CollisionWithPlayer()
{
    this->createScoreCurrentPos(100);
    this->R_BezierWithRotation(Vec2(2000, 3000), Vec2(540, 1500), Vec2(900, 2000), 1.f);
    CObjectManager::Instance()->ShowCapturingAction();
}