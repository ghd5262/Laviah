#include "PlayCoin.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../MyUI/ScoreUI.h"
#include "../../Particle/Particles.h"
#include "../../Scene/GameScene.h"

using namespace cocos2d;

CPlayCoin::CPlayCoin()
: m_pParticleCrash(nullptr){}

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

void CPlayCoin::CollisionWithPlanet()
{
	if (this->getIsFly())
	{
		m_pParticleCrash = CParticle_Explosion::create("coin_5.png");
		if (m_pParticleCrash != nullptr){
			m_pParticleCrash->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_pParticleCrash->setAngle(-getRotation());
			m_pParticleCrash->setPosition(getPosition());
			m_pParticleCrash->setGravity(m_RotationVec);
			m_pParticleCrash->setSpeed(100);
			m_pParticleCrash->setSpeedVar(50);
			CGameScene::getGameScene()->addChild(m_pParticleCrash, 100);
		}
        
        this->ReturnToMemoryBlock();
	}
}

void CPlayCoin::CollisionWithPlayer()
{
//	CAudioManager::Instance()->PlayEffectSound("sounds/Star_2.mp3", false);
	this->R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
	auto value = CItemManager::Instance()->getValueOfCoin((eCOIN_TYPE)(this->getSymbol() - 'U' + 1));
    GLOBAL::COINSCORE += value;
}
