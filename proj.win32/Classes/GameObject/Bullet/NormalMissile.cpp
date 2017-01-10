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
#include "../../MyUI/MultipleScore.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/ChallengeDataManager.hpp"
#include "../../DataManager/ChallengeChecker/ChallengeClearChecker.h"

using namespace cocos2d;

CNormalMissile::CNormalMissile()
: m_bIsTargetMarkCreate(false)
, m_pParticleCrash(nullptr)
, m_pParticleFlame(nullptr)
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

	return this;
}

bool CNormalMissile::init()
{
    if (!CBullet::init()) return false;
    
	this->setItemEffect(eITEM_FLAG_giant | eITEM_FLAG_coin | eITEM_FLAG_star | eITEM_FLAG_shield);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    m_ScreenRect = Rect(-visibleSize.width, 0, visibleSize.width * 3, visibleSize.height);

    return true;
}

void CNormalMissile::Execute(float delta)
{
	m_Time += delta;
	if (m_Time < getDelayTime()) return;
    if (!m_bIsTargetMarkCreate) {
        CCLOG("Delay : %f Time : %f", getDelayTime(), m_Time);
        this->createTargetLine();
    }

	m_FSM->Execute(delta);
}

void CNormalMissile::CollisionWithPlanet()
{
    //		CAudioManager::Instance()->PlayEffectSound("sounds/explosion_0.mp3", false);
    
	m_Planet->CrushShake(0.01f, 0.3f, 0.1f, 3.0f);
	
    this->createParticle_Explosion();
	this->ReturnToMemoryBlock();
}

void CNormalMissile::CollisionWithPlayer()
{
    //		CAudioManager::Instance()->PlayEffectSound("sounds/explosion_0.mp3", false);
    
	if (CItemManager::Instance()->isCurrentItem(eITEM_FLAG_giant)){
        GLOBAL->GIANT_COUNT_MISSILE += 1;
        GLOBAL->GIANT_SCORE_MISSILE += 30;
		createScoreCurrentPos(30);
		R_BezierWithRotation(Vec2(1180, 2020), Vec2(350, 900), Vec2(450, 1200), 0.5f);
	}
	else{
		m_Player->StackedRL(0.1f, 10, 10, 5);
		m_Player->LostSomeHealth(this->getPower());
		m_Planet->CrushShake(0.01f, 0.2f, 0.1f, 3.0f);

		this->ReturnToMemoryBlock();
	}
}

void CNormalMissile::CollisionWithBarrier()
{
    GLOBAL->BARRIER_COUNT += 1;
	GLOBAL->BARRIER_SCORE += 30;
	createScoreCurrentPos(30);
    this->createParticle_Explosion();
	this->ReturnToMemoryBlock();
}

void CNormalMissile::ChangeToCoinOrStar()
{
	if (!m_bIsTargetMarkCreate)
		return;

	this->setAlive(false);

	auto patternIndex = CObjectManager::Instance()->getCharacterParam()->_idx;

	float distance = m_TargetVec.distance(getPosition());

    // TODO: 패턴 안에 있는 bullet의 데이터를 셋팅해야한다.
	CObjectManager::Instance()->getBulletCreator()->CreateImmediately(patternIndex, getAngle(), distance);
    
	// 이부분 일단 작업하고 다음 리펙토링 때 autoReturnMemoryPool에 넣어야한다.
	this->scheduleOnce([this](float delta){
		this->ReturnToMemoryBlock();
	}, 0.f, "ReturnToMemoryBlock");
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

    CGameScene::getGameScene()->addChild(bullet, ZORDER::BACKGROUND);

#if(!USE_MEMORY_POOLING)
	CObjectManager::Instance()->AddBullet(bullet);
#endif
}

void CNormalMissile::createParticle_Flame()
{
    // 불꽃 파티클
    //m_pParticleFlame = CParticle_Flame::create("missileFlame.png");
    //if (m_pParticleFlame != nullptr){
    //    m_pParticleFlame->retain();
    //    m_pParticleFlame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    //    m_pParticleFlame->setAngle(-90);
    //    m_pParticleFlame->setGravity(Vec2(90, 0));
    //    m_pParticleFlame->setPosition(Vec2(this->getContentSize().width * 1.1f, this->getContentSize().height * 0.5f));
    //    m_pParticleFlame->setStartSpin(-90);
    //    m_pParticleFlame->setEndSpin(270);
    //    m_pParticleFlame->setLife(0.1f);
    //    m_pParticleFlame->setLifeVar(0.15f);
    //    
    //    this->addChild(m_pParticleFlame);
    //}

	m_pParticleFlame = CParticle_Flame::create("fire.png");
	if (m_pParticleFlame != nullptr){
		m_pParticleFlame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_pParticleFlame->setAngle(0);
		m_pParticleFlame->setGravity(Vec2(90, 0));
		m_pParticleFlame->setPosition(Vec2(this->getContentSize().width * 1.2f,	this->getContentSize().height * 0.5f));
		m_pParticleFlame->setStartSize(50);
		m_pParticleFlame->setLife(0.3f);
		m_pParticleFlame->setLifeVar(0.15f);
		m_pParticleFlame->setStartColor(Color4F(1.f, 1.f, 0.5f, 1.f));
		m_pParticleFlame->setStartColorVar(Color4F(0, 0, 0.8f, 0));
		m_pParticleFlame->setEndColor(Color4F(1.f, 1.f, 1.f, 0.4f));
		m_pParticleFlame->setEndColorVar(Color4F(0, 0, 0, 0));
		m_pParticleFlame->setPosVar(Vec2(0, 10));
		m_pParticleFlame->setTotalParticles(80);
		this->addChild(m_pParticleFlame);
	}
}

void CNormalMissile::createParticle_Explosion()
{
	m_pParticleCrash = CParticle_Explosion::create("explosion_1.png");
    if (m_pParticleCrash != nullptr){
        m_pParticleCrash->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_pParticleCrash->setAngle(-getRotation());
        m_pParticleCrash->setPosition(getPosition());
        m_pParticleCrash->setGravity(m_RotationVec);
        CGameScene::getGameScene()->addChild(m_pParticleCrash, 100);
    }
}