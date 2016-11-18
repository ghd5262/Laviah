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
#include "../../MyUI/UIManager.h"
#include "../../MyUI/MultipleScore.h"
#include "../BulletCreator.h"

using namespace cocos2d;


CStickBullet::CStickBullet()
: m_HitWithPlanet(false)
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

    m_UIScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("StarScoreUI"));
    m_MultipleScore = static_cast<CMultipleScore*>(CUIManager::Instance()->FindUIWithName("MultipleScoreUI"));
    
    float bulletSpeedAccel = this->getSpeed() * 0.15f;
    
    this->schedule([=](float delta){
        
        this->setSpeed(this->getSpeed() + bulletSpeedAccel);
        
    }, 0.1f, 10, 0.3f, "AccelerationUP");
    
    return true;
}

void CStickBullet::CollisionWithPlanet()
{
    if(m_HitWithPlanet == false)
    {
        m_HitWithPlanet = true;
        
        this->setIsFly(false);
        
        scheduleOnce([=](float delta){
            
            this->setSpeed(300);
            this->setIsFly(true);
            
            scheduleOnce([=](float delta){
                
                this->ReturnToMemoryBlock();
                
            }, 0.2f, MakeString("StickBulletStayFinish_%d", random<int>(1, 100)));
            
        }, 2.f, MakeString("StickBulletStay_%d", random<int>(1, 100)));
    }
}

void CStickBullet::CollisionWithPlayer()
{
    if (CItemManager::Instance()->getCurrentItem() & eITEM_FLAG_giant){
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
    this->createScoreCurrentPos(30);
    this->R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
}

void CStickBullet::ChangeToCoinOrStar()
{
    float distance = m_TargetVec.distance(getPosition());
    char symbol = 'T';
    if (CItemManager::Instance()->getCurrentItem() & eITEM_FLAG_star) symbol = 'Y';
    
    CBulletCreator::createBullet(symbol, -getRotation(), distance, getSpeed() * 0.5f);
    
    this->ReturnToMemoryBlock();
}