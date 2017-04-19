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
#include "../../DataManager/ChallengeDataManager.hpp"
#include "../../DataManager/ChallengeChecker/ChallengeClearChecker.h"

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
    CBullet::CollisionWithPlanet();
}

void CNormalMissile::CollisionWithPlayer()
{
    //		CAudioManager::Instance()->PlayEffectSound("sounds/explosion_0.mp3", false);
    
	if (CItemManager::Instance()->isCurrentItem(eITEM_FLAG_giant)){
        GLOBAL->GIANT_COUNT_TOTAL += 1;
        GLOBAL->GIANT_COUNT_MISSILE += 1;
        GLOBAL->GIANT_SCORE_TOTAL += (GLOBAL->COMBO_LEVEL * 2);
        GLOBAL->GIANT_SCORE_MISSILE += (GLOBAL->COMBO_LEVEL * 2);
		this->createScoreCurrentPos(GLOBAL->COMBO_LEVEL * 2);
		R_BezierWithRotation(Vec2(2000, 3000), Vec2(540, 1500), Vec2(900, 2000), 1.f);
	}
	else{
		m_Player->LostSomeHealth(this->getPower());
        m_Player->Crushed();
        m_Planet->Crushed();

		this->ReturnToMemoryBlock();
	}
}

void CNormalMissile::CollisionWithBarrier()
{
    GLOBAL->BARRIER_COUNT += 1;
	GLOBAL->BARRIER_SCORE += (GLOBAL->COMBO_LEVEL * 2);
	this->createScoreCurrentPos(GLOBAL->COMBO_LEVEL * 2);
    this->createCollisionParticle();
	this->ReturnToMemoryBlock();
}

void CNormalMissile::ChangeToCoin()
{
	if (!m_bIsTargetMarkCreate) return;

	this->setAlive(false);
	auto patternIndex = CObjectManager::Instance()->getCharacterParam()->_idx;
    auto data = CBulletPatternDataManager::Instance()->getMissilePatternByIndex(patternIndex);
	float distance = m_TargetVec.distance(getPosition());

	CObjectManager::Instance()->getBulletCreator()->CreateImmediately(data, getAngle(), distance, eITEM_FLAG_coin);
    
    this->ReturnToMemoryBlock();
}

void CNormalMissile::ChangeToStar()
{
	if (!m_bIsTargetMarkCreate) return;

	this->setAlive(false);
	auto patternIndex = CObjectManager::Instance()->getCharacterParam()->_idx;
    auto data = CBulletPatternDataManager::Instance()->getMissilePatternByIndex(patternIndex);
    float distance = m_TargetVec.distance(getPosition());
    
    CObjectManager::Instance()->getBulletCreator()->CreateImmediately(data, getAngle(), distance, eITEM_FLAG_star);
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
	auto particle = CParticle_Flame::create("fire.png");
	if (particle != nullptr){
		particle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		particle->setAngle(0);
		particle->setGravity(Vec2(90, 0));
		particle->setPosition(Vec2(this->getContentSize().width * 1.2f,
                                   this->getContentSize().height * 0.5f));
		particle->setStartSize(50);
		particle->setLife(0.3f);
		particle->setLifeVar(0.15f);
		particle->setStartColor(Color4F(1.f, 1.f, 0.5f, 1.f));
		particle->setStartColorVar(Color4F(0, 0, 0.8f, 0));
		particle->setEndColor(Color4F(1.f, 1.f, 1.f, 0.4f));
		particle->setEndColorVar(Color4F(0, 0, 0, 0));
		particle->setPosVar(Vec2(0, 10));
		particle->setTotalParticles(80);
		this->addChild(particle);
	}
}