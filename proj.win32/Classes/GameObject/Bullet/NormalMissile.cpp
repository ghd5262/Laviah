#include "NormalMissile.h"
#include "PlayCoin.h"
#include "PlayStar.h"
#include "TargetMark.h"
#include "ScoreBullet.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../Shooter/PatternShooter.h"
#include "../../MyUI/ScoreUI.h"
#include "../../MyUI/UIManager.h"
#include "../../Particle/Particles.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/StageDataManager.h"
#include "../../MyUI/MultipleScore.h"
#include "../../DataManager/UserDataManager.h"

CNormalMissile::CNormalMissile(
	sBULLET_PARAM bulletParam,
	float angle,				    //bullet 초기 각도 
	float speed)				    //bullet 초기 속도
	: CBullet(
	bulletParam,
	angle,
	speed)
	, m_bIsTargetMarkCreate(false)
	, m_pParticleCrash(nullptr)
	, m_pParticleFlame(nullptr)
{}

CNormalMissile* CNormalMissile::create(
	sBULLET_PARAM bulletParam,
	float angle,					//bullet 초기 각도 
	float speed)					//bullet 초기 속도
	
{
	CNormalMissile* pRet = 
		(CNormalMissile*)new(std::nothrow)CNormalMissile(
		bulletParam, angle, speed);

	if (pRet && pRet->init())
	{
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool CNormalMissile::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CNormalMissile::initVariable()
{
	setItemEffect(eITEM_FLAG_giant | eITEM_FLAG_coin | eITEM_FLAG_star | eITEM_FLAG_shield);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	m_ScreenRect = Rect(-visibleSize.width, 0, visibleSize.width * 3, visibleSize.height);

	setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_BulletParam._fDistance) + m_pPlanet->getPosition().x);
	setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_BulletParam._fDistance) + m_pPlanet->getPosition().y);
	setRotation(-m_fAngle);

	std::string missileTextureName = m_pPlayer->getCharacterParam()._normalMissileTextureName;
	if (m_BulletParam._isAimingMissile)
		missileTextureName = m_pPlayer->getCharacterParam()._aimingMissileTextureName;

	m_pTexture = Sprite::createWithSpriteFrameName(missileTextureName);
	if (m_pTexture != nullptr){
		m_pTexture->setAnchorPoint(Vec2(0.2f, 0.5f));
		addChild(m_pTexture);
	}

	Vec2 temp((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) *  1800.f) + m_pPlanet->getPosition().x,
		(sin(CC_DEGREES_TO_RADIANS(m_fAngle)) *  1800.f) + m_pPlanet->getPosition().y);

	float distance = getPosition().distance(temp);
	float tempTime = (distance / m_fBulletSpeed);


	// 불꽃 파티클
	m_pParticleFlame = CParticle_Flame::create("missileFlame.png");
	if (m_pParticleFlame != nullptr){
		m_pParticleFlame->retain();
		m_pParticleFlame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_pParticleFlame->setAngle(-90);
		m_pParticleFlame->setGravity(Vec2(90, 0));
		m_pParticleFlame->setPosition(Vec2(m_pTexture->getContentSize().width * 1.1f, m_pTexture->getContentSize().height * 0.5f));
		m_pParticleFlame->setStartSpin(-90);
		m_pParticleFlame->setEndSpin(270);
		m_pParticleFlame->setLife(0.1f);
		m_pParticleFlame->setLifeVar(0.15f);

		m_pTexture->addChild(m_pParticleFlame);
	}


	this->scheduleOnce([&](float delta){
		CGameScene::getGridWorld()->addChild(CTargetMark::create(
			sBULLET_PARAM(
			0.f, 0.f, 0.f,
			false,									//FlyItem 여부
			m_BulletParam._isAimingMissile),		//AimingMissile 여부
			-getRotation(),							//초기 각도
			this->getPosition(),					//미사일 좌표
			m_fBulletSpeed,							//미사일 스피드
			false,									//코인미사일여부
			this), 100);

		m_bIsTargetMarkCreate = true;

	}, tempTime, MakeString("createTargetMark_%d", distance * 100));

	m_pUIScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("StarScoreUI"));
	m_pMultipleScore = static_cast<CMultipleScore*>(CUIManager::Instance()->FindUIWithName("MultipleScoreUI"));

	return true;
}


void CNormalMissile::Rotation(float dir, float delta)
{
	// aimingMissile일 경우 화면안에 들어왔을 때에만 회전한다.
	if (true == m_BulletParam._isAimingMissile){
		if (!m_ScreenRect.containsPoint(getPosition()))
		{
			return;
		}
	}

	CBullet::Rotation(dir, delta);
}

void CNormalMissile::Execute(float delta)
{
	getFSM()->Execute(delta);
}

void CNormalMissile::CollisionWithPlanet()
{
	
	if (true == m_BulletParam._isAimingMissile){
		// 행성 흔들기
		CObjectManager::Instance()->getPlanet()->CrushShake(
			0.01f, 0.5f, 0.1f, 5.0f);
	}
	else {
		// 행성 흔들기
		CObjectManager::Instance()->getPlanet()->CrushShake(
			0.01f, 0.3f, 0.1f, 3.0f);
	}

	m_pParticleCrash = CParticle_Explosion::create(MakeString("explosion_%d.png", m_BulletParam._isAimingMissile + 1));
	if (m_pParticleCrash != nullptr){
		m_pParticleCrash->retain();
		m_pParticleCrash->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_pParticleCrash->setAngle(-getRotation());
		m_pParticleCrash->setPosition(getPosition());
		m_pParticleCrash->setGravity(m_RotationVec);
		CGameScene::getGridWorld()->addChild(m_pParticleCrash, 100);
	}

	//CAudioManager::Instance()->PlayEffectSound(MakeString("sounds/explosion_%d.mp3", m_BulletParam._isAimingMissile + 1), false);

	ReturnToMemoryBlock();
}

void CNormalMissile::CollisionWithPlayer()
{
	if (CItemManager::Instance()->getCurrentItem() & eITEM_FLAG_giant){
		createScoreCurrentPos(50);
		CAudioManager::Instance()->PlayEffectSound("sounds/explosion_2.mp3", false);
		R_BezierWithRotation(Vec2(1180, 2020), Vec2(350, 900), Vec2(450, 1200), 0.5f);
	}
	else{
		m_pPlayer->StackedRL(0.1f, 10, 10, 5);
		m_pPlayer->LostSomeHealth(m_BulletParam._fPower);
		if (true == m_BulletParam._isAimingMissile){
			CObjectManager::Instance()->getPlanet()->CrushShake(
				0.01f, 0.5f, 0.1f, 5.0f);
		}
		else {
			CObjectManager::Instance()->getPlanet()->CrushShake(
				0.01f, 0.2f, 0.1f, 3.0f);
		}

		m_pParticleCrash = CParticle_Explosion::create(MakeString("explosion_%d.png", m_BulletParam._isAimingMissile + 1));
		if (m_pParticleCrash != nullptr){
			m_pParticleCrash->retain();
			m_pParticleCrash->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_pParticleCrash->setAngle(-getRotation());
			m_pParticleCrash->setPosition(getPosition());
			m_pParticleCrash->setGravity(m_RotationVec);
			CGameScene::getGridWorld()->addChild(m_pParticleCrash, 100);
		}

		CAudioManager::Instance()->PlayEffectSound(MakeString("sounds/explosion_%d.mp3", m_BulletParam._isAimingMissile + 1), false);

		ReturnToMemoryBlock();
	}
}

void CNormalMissile::CollisionWithBarrier()
{
	m_pParticleCrash = CParticle_Explosion::create(MakeString("explosion_%d.png", m_BulletParam._isAimingMissile + 1));
	createScoreCurrentPos(50);
	if (m_pParticleCrash != nullptr){
		m_pParticleCrash->retain();
		m_pParticleCrash->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_pParticleCrash->setAngle(-getRotation());
		m_pParticleCrash->setPosition(getPosition());
		m_pParticleCrash->setGravity(m_RotationVec);
		CGameScene::getGridWorld()->addChild(m_pParticleCrash, 100);
	}
	ReturnToMemoryBlock();
}

void CNormalMissile::ChangeToCoinOrStar()
{
	if (!m_bIsTargetMarkCreate)
		return;

	setAlive(false);

	std::string patternName;
	if (false == m_BulletParam._isAimingMissile){
		patternName = m_pPlayer->getCharacterParam()._normalMissilePattern;
	}
	else{
		patternName = m_pPlayer->getCharacterParam()._aimingMissilePattern;
	}
	float distance = m_TargetVec.distance(getPosition());
	float speed = 600.f;

	auto screwShooter = CPatternShooter::create(
		sSHOOTER_PARAM(
		patternName
		, 0.f
		, 0.f
		, speed
		, -getRotation()
		, 0.f
		, 1
		, 1
		, true
		, true), distance);

	CGameScene::getGridWorld()->addChild(CTargetMark::create(
		sBULLET_PARAM(
		0.f,
		0.f,
		0.f,
		false,		// Fly Item 인지
		false),		// AimingMissile 인지
		-getRotation(),										//초기 각도
		this->getPosition(),
		speed
	), 100);


	// 이부분 일단 작업하고 다음 리펙토링 때 autoReturnMemoryPool에 넣어야한다.
	this->scheduleOnce([this](float delta){
		ReturnToMemoryBlock();
	}, 0.f, "ReturnToMemoryBlock");
}