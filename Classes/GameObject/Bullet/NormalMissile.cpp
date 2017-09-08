#include "NormalMissile.h"
#include "PlayCoin.h"
#include "PlayStar.h"
#include "TargetMark.h"
#include "ScoreBullet.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../BulletCreator.h"
#include "../../MyUI/ScoreUI.h"
#include "../../Particle/Particles.h"
#include "../../Scene/GameScene.h"
#include "../../MyUI/ComboScore.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/AchievementDataManager.hpp"
#include "../../DataManager/AchievementChecker/AchievementClearChecker.h"
#include "../../DataManager/GradientDataManager.h"
#include "../../DataManager/BulletPatternDataManager.h"

using namespace cocos2d;

CNormalMissile::CNormalMissile()
: m_bIsTargetMarkCreate(false)
{}

CNormalMissile* CNormalMissile::create()
{
	CNormalMissile* pRet = (CNormalMissile*)new(std::nothrow)CNormalMissile();

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

CBullet* CNormalMissile::build()
{
	CBullet::build();

	this->createParticle_Flame();
    this->setParticleName("explosion_1.png");
	return this;
}

bool CNormalMissile::init()
{
    if (!CBullet::init()) return false;
    
    this->setItemEffect(eITEM_FLAG_giant | eITEM_FLAG_coin | eITEM_FLAG_star | eITEM_FLAG_shield);

    this->setColor(CGradientDataManager::Instance()->getBulletColorByLevel(GVALUE->NOTICE_LEVEL));

    return true;
}

void CNormalMissile::Execute(float delta)
{
	m_Time += delta;
    if (m_Time >= (getDelayTime() - 0.5f))
    {
        if (!m_bIsTargetMarkCreate) {
            this->createTargetLine();
        }
    }

    if (!IsTimeUP()) return;
    this->setVisible(true);
    m_FSM->Execute(delta);
}

void CNormalMissile::CollisionWithPlanet()
{
    //		CAudioManager::Instance()->PlayEffectSound("sounds/explosion_0.mp3", false);
	m_Planet->Crushed();
    this->createExplosionEffect();
    this->ReturnToMemoryBlock();
}

void CNormalMissile::CollisionWithPlayer()
{
    //		CAudioManager::Instance()->PlayEffectSound("sounds/explosion_0.mp3", false);
    
	if (CItemManager::Instance()->isCurrentItem(eITEM_FLAG_giant)){
//        if(CObjectManager::Instance()->IsHitWithSlowPoint(this))
//            CObjectManager::Instance()->SlowMotion();
        GVALUE->GIANT_COUNT_TOTAL += 1;
        GVALUE->GIANT_COUNT_MISSILE += 1;
        GVALUE->GIANT_SCORE_TOTAL += (GVALUE->COMBO_LEVEL * 2);
        GVALUE->GIANT_SCORE_MISSILE += (GVALUE->COMBO_LEVEL * 2);
		this->createScoreCurrentPos(GVALUE->COMBO_LEVEL * 2);
		R_BezierWithRotation(Vec2(2000, 3000), Vec2(540, 1500), Vec2(900, 2000), 1.f);
	}
	else{
        GVALUE->DEAD_TYPE = PLAYER_DEAD_TYPE::NORMAL_MISSILE;
        CUserDataManager::Instance()->setUserData_NumberAdd(USERDATA_KEY::DEAD_BY_MISSILE, 1);
		m_Player->LostSomeHealth(this->getPower());
        m_Player->Crushed();
        m_Planet->Crushed();
    }
}

void CNormalMissile::CollisionWithBarrier()
{
    GVALUE->BARRIER_COUNT += 1;
	GVALUE->BARRIER_SCORE += (GVALUE->COMBO_LEVEL * 2);
	this->createScoreCurrentPos(GVALUE->COMBO_LEVEL * 2);
    this->createExplosionEffect();
	this->ReturnToMemoryBlock();
}

void CNormalMissile::ChangeToCoin()
{
	if (!m_bIsTargetMarkCreate) return;

	this->setAlive(false);
	auto patternIndex = CObjectManager::Instance()->getCharacterParam()->_index;
    auto data = CBulletPatternDataManager::Instance()->getMissilePatternByIndex(patternIndex);
	float distance = m_TargetVec.distance(getPosition());

	CObjectManager::Instance()->getBulletCreator()->CreateImmediately(data, getAngle(), distance, eITEM_FLAG_coin);
    CUserDataManager::Instance()->setUserData_NumberAdd(USERDATA_KEY::CHANGE_TO_COIN, 1);
    
    this->ReturnToMemoryBlock();
}

void CNormalMissile::ChangeToStar()
{
	if (!m_bIsTargetMarkCreate) return;

	this->setAlive(false);
	auto patternIndex = CObjectManager::Instance()->getCharacterParam()->_index;
    auto data = CBulletPatternDataManager::Instance()->getMissilePatternByIndex(patternIndex);
    float distance = m_TargetVec.distance(getPosition());
    
    CObjectManager::Instance()->getBulletCreator()->CreateImmediately(data, getAngle(), distance, eITEM_FLAG_star);
    CUserDataManager::Instance()->setUserData_NumberAdd(USERDATA_KEY::CHANGE_TO_STAR, 1);
    this->ReturnToMemoryBlock();
}

void CNormalMissile::createTargetLine()
{
	m_bIsTargetMarkCreate = true;

	auto bullet = CTargetMark::create();
	bullet->setAngle(getAngle());
	bullet->setDistance(m_Planet->getBoundingRadius() * 0.9f);
	bullet->setBullet(this);
	bullet->setIsFly(false);
	bullet->build();

    CGameScene::getZoomLayer()->addChild(bullet, ZORDER::BACKGROUND);

#if(!USE_MEMORY_POOLING)
	CObjectManager::Instance()->AddBullet(bullet);
#endif
}

void CNormalMissile::createParticle_Flame()
{
    auto color    = CGradientDataManager::Instance()->getBulletColorByLevel(GVALUE->NOTICE_LEVEL);
	auto particle = CParticle_Flame::create("particle_snow.png");
	if (particle != nullptr){
		particle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		particle->setAngle(0);
		particle->setGravity(Vec2(90, 0));
		particle->setPosition(Vec2(this->getContentSize().width * 0.9f,
                                   this->getContentSize().height * 0.5f));
		particle->setStartSize(50);
		particle->setLife(0.5f);
		particle->setLifeVar(0.15f);
		particle->setStartColor(Color4F(color.r / 255.f, color.g / 255.f, color.b / 255.f, 0.5f));
		particle->setStartColorVar(Color4F(0, 0, 0, 0));
		particle->setEndColor(Color4F(color.r / 255.f, color.g / 255.f, color.b / 255.f, 0.0f));
		particle->setEndColorVar(Color4F(0, 0, 0, 0));
		particle->setPosVar(Vec2(10, 10));
		particle->setTotalParticles(8);
		this->addChild(particle, -1);
	}
}
