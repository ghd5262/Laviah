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
#include "../../MyUI/UIManager.h"
#include "../../MyUI/MultipleScore.h"

using namespace cocos2d;

CNormalBullet::CNormalBullet()
: m_pParticleCrash(nullptr){}

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
    
    m_MultipleScore = static_cast<CMultipleScore*>(CUIManager::Instance()->FindUIWithName("MultipleScoreUI"));
    m_UIScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("StarScoreUI"));
    
    return true;
}

void CNormalBullet::CollisionWithPlanet()
{
    this->setParticle();
	this->ReturnToMemoryBlock();
}

void CNormalBullet::CollisionWithPlayer()
{
	if (CItemManager::Instance()->getCurrentItem() & eITEM_FLAG_giant){
		createScoreCurrentPos(30);
		this->R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
	}
	else{
		m_Player->StackedRL(0.1f, 10, 10, 5);
		CCLOG("Bullet Power %f", this->getPower());
		m_Player->LostSomeHealth(this->getPower());
        this->ReturnToMemoryBlock();
	}
}

void CNormalBullet::CollisionWithBarrier()
{
	this->createScoreCurrentPos(30);
	this->R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
}

void CNormalBullet::ChangeToCoinOrStar()
{
	float distance = m_TargetVec.distance(getPosition());
    char symbol = 0;
    
    if (CItemManager::Instance()->getCurrentItem() & eITEM_FLAG_star) symbol = 'P';
    else symbol = 'U';
    
    CBulletCreator::createBullet(symbol, -getRotation(), distance);
    
    this->ReturnToMemoryBlock();
}

void CNormalBullet::setParticle()
{
    m_pParticleCrash = CParticle_Explosion::create(m_Player->getCharacterParam()._normalBulletTextureName);
    if (m_pParticleCrash != nullptr){
        m_pParticleCrash->retain();
        m_pParticleCrash->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_pParticleCrash->setAngle(-getRotation());
        m_pParticleCrash->setPosition(getPosition());
        m_pParticleCrash->setGravity(m_RotationVec);
        m_pParticleCrash->setSpeed(100);
        m_pParticleCrash->setSpeedVar(50);
        CGameScene::getGridWorld()->addChild(m_pParticleCrash, 100);
    }
}