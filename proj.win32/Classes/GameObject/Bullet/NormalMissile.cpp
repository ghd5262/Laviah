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

//    Size visibleSize = Director::getInstance()->getVisibleSize();
//    m_ScreenRect = Rect(-visibleSize.width, 0, visibleSize.width * 3, visibleSize.height);

    return true;
}

void CNormalMissile::Execute(float delta)
{
	m_Time += delta;
    if (!IsTimeUP()) return;
    if (!m_bIsTargetMarkCreate) {
        CCLOG("Delay : %f Time : %f", getDelayTime(), m_Time);
        this->createTargetLine();
    }
    this->setVisible(true);
    m_FSM->Execute(delta);
}

void CNormalMissile::CollisionWithPlanet()
{
    //		CAudioManager::Instance()->PlayEffectSound("sounds/explosion_0.mp3", false);
    
	m_Planet->CrushShake(0.01f, 0.3f, 0.1f, 3.0f);
	
    this->createCollisionParticle();
	this->ReturnToMemoryBlock();
}

void CNormalMissile::CollisionWithPlayer()
{
    //		CAudioManager::Instance()->PlayEffectSound("sounds/explosion_0.mp3", false);
    
	if (CItemManager::Instance()->isCurrentItem(eITEM_FLAG_giant)){
        GLOBAL->GIANT_COUNT_MISSILE += 1;
        GLOBAL->GIANT_SCORE_MISSILE += 30;
		this->createScoreCurrentPos(30);
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
	this->createScoreCurrentPos(30);
    this->createCollisionParticle();
	this->ReturnToMemoryBlock();
}

void CNormalMissile::ChangeToCoin()
{
	if (!m_bIsTargetMarkCreate)
		return;

	this->setAlive(false);

	auto patternIndex = CObjectManager::Instance()->getCharacterParam()->_idx;

	float distance = m_TargetVec.distance(getPosition());

    // TODO: 패턴 안에 있는 bullet의 데이터를 셋팅해야한다.
	CObjectManager::Instance()->getBulletCreator()->CreateImmediately(patternIndex, getAngle(), distance);
    
	// 이부분 일단 작업하고 다음 리펙토링 때 autoReturnMemoryPool에 넣어야한다.
//	this->scheduleOnce([this](float delta){
		this->ReturnToMemoryBlock();
//	}, 0.f, "ReturnToMemoryBlock");
}

void CNormalMissile::ChangeToStar()
{
	if (!m_bIsTargetMarkCreate)
		return;

	this->setAlive(false);

	auto patternIndex = CObjectManager::Instance()->getCharacterParam()->_idx;

	float distance = m_TargetVec.distance(getPosition());

	// TODO: 패턴 안에 있는 bullet의 데이터를 셋팅해야한다.
	CObjectManager::Instance()->getBulletCreator()->CreateImmediately(patternIndex, getAngle(), distance);

	// 이부분 일단 작업하고 다음 리펙토링 때 autoReturnMemoryPool에 넣어야한다.
//	this->scheduleOnce([this](float delta){
		this->ReturnToMemoryBlock();
//	}, 0.f, "ReturnToMemoryBlock");
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
	auto particle = CParticle_Flame::create("fire.png");
	if (particle != nullptr){
		particle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		particle->setAngle(0);
		particle->setGravity(Vec2(90, 0));
		particle->setPosition(Vec2(this->getContentSize().width * 1.2f,	this->getContentSize().height * 0.5f));
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