#include "PlayStar.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../MyUI/ScoreUI.h"
#include "../../MyUI/MultipleScore.h"
#include "../../Particle/Particles.h"
#include "../../Scene/GameScene.h"

using namespace cocos2d;

CPlayStar::CPlayStar()
: m_pParticleCrash(nullptr)
{}

CPlayStar* CPlayStar::create()
{
	CPlayStar* pRet = (CPlayStar*)new(std::nothrow)CPlayStar();

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

bool CPlayStar::init()
{
	if (!CBullet::init()) return false;

    this->setItemEffect(eITEM_FLAG_magnet);
    return true;
}

void CPlayStar::CollisionWithPlanet()
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
            CGameScene::getGameScene()->addChild(m_pParticleCrash, ZORDER::PLAYER);
		}
        
        this->ReturnToMemoryBlock();
	}
}

void CPlayStar::CollisionWithPlayer()
{
//	CAudioManager::Instance()->PlayEffectSound("sounds/Star_2.mp3", false);
	this->R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);

	auto value = CItemManager::Instance()->getValueOfStar((eSTAR_TYPE)(this->getSymbol() - 'P' + 1));
    CMultipleScore::Instance()->AddScore(value);
	GLOBAL->STAR_COUNT += 1;
}
