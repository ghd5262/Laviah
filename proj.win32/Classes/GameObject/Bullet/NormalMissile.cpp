#include "NormalMissile.h"
#include "PlayCoin.h"
#include "PlayStar.h"
#include "TargetMark.h"
#include "ScoreBullet.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../BulletCreator.hpp"
#include "../Shooter/PatternShooter.h"
#include "../../MyUI/ScoreUI.h"
#include "../../MyUI/UIManager.h"
#include "../../Particle/Particles.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/StageDataManager.h"
#include "../../MyUI/MultipleScore.h"
#include "../../DataManager/UserDataManager.h"

using namespace cocos2d;

CNormalMissile::CNormalMissile(sBULLET_PARAM bulletParam, float angle)
: CBullet(bulletParam, angle)
, m_bIsTargetMarkCreate(false)
, m_pParticleCrash(nullptr)
, m_pParticleFlame(nullptr)
{}

CNormalMissile* CNormalMissile::create(sBULLET_PARAM bulletParam, float angle)
{
	CNormalMissile* pRet = 
		(CNormalMissile*)new(std::nothrow)CNormalMissile(
		bulletParam, angle);

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

bool CNormalMissile::init()
{
    if (!CBullet::init()) return false;
    
    setItemEffect(eITEM_FLAG_giant | eITEM_FLAG_coin | eITEM_FLAG_star | eITEM_FLAG_shield);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    m_ScreenRect = Rect(-visibleSize.width, 0, visibleSize.width * 3, visibleSize.height);
    
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

    
    this->createParticle_Flame();
    this->createTargetLine();
    
    
    m_bIsTargetMarkCreate = true;
    
    
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
    //CAudioManager::Instance()->PlayEffectSound(MakeString("sounds/explosion_%d.mp3", m_BulletParam._isAimingMissile + 1), false);
    
	if (true == m_BulletParam._isAimingMissile)
		m_pPlanet->CrushShake(0.01f, 0.5f, 0.1f, 5.0f);
	else
		m_pPlanet->CrushShake(0.01f, 0.3f, 0.1f, 3.0f);
	
    this->createParticle_Explosion();
	this->ReturnToMemoryBlock();
}

void CNormalMissile::CollisionWithPlayer()
{
    //		CAudioManager::Instance()->PlayEffectSound(MakeString("sounds/explosion_%d.mp3", m_BulletParam._isAimingMissile + 1), false);
    
	if (CItemManager::Instance()->getCurrentItem() & eITEM_FLAG_giant){
		createScoreCurrentPos(50);
		R_BezierWithRotation(Vec2(1180, 2020), Vec2(350, 900), Vec2(450, 1200), 0.5f);
	}
	else{
		m_pPlayer->StackedRL(0.1f, 10, 10, 5);
		m_pPlayer->LostSomeHealth(m_BulletParam._fPower);
		if (true == m_BulletParam._isAimingMissile){
			m_pPlanet->CrushShake(
				0.01f, 0.5f, 0.1f, 5.0f);
		}
		else {
			m_pPlanet->CrushShake(
				0.01f, 0.2f, 0.1f, 3.0f);
		}

		this->ReturnToMemoryBlock();
	}
}

void CNormalMissile::CollisionWithBarrier()
{
    this->createParticle_Explosion();
	this->ReturnToMemoryBlock();
}

void CNormalMissile::ChangeToCoinOrStar()
{
	if (!m_bIsTargetMarkCreate)
		return;

	this->setAlive(false);

	std::string patternName;
	if (false == m_BulletParam._isAimingMissile){
		patternName = m_pPlayer->getCharacterParam()._normalMissilePattern;
	}
	else{
		patternName = m_pPlayer->getCharacterParam()._aimingMissilePattern;
	}
	float distance = m_TargetVec.distance(getPosition());
	float speed = 600.f;
    
    auto pattern = CBulletPatternDataManager::Instance()->getDataByName(patternName);

    CBulletCreator::Instance()->createImmediately(pattern, -getRotation(), distance);
    
	// 이부분 일단 작업하고 다음 리펙토링 때 autoReturnMemoryPool에 넣어야한다.
	this->scheduleOnce([this](float delta){
		this->ReturnToMemoryBlock();
	}, 0.f, "ReturnToMemoryBlock");
}


void CNormalMissile::createTargetLine()
{
    auto data = sBULLET_PARAM();
    data._symbol = '7';
    data._isAimingMissile = false;
    data._isFly = false;
    
    auto bullet = CBulletCreator::createBullet(data, -getRotation(), m_pPlanet->getBRadius());
    auto targetMark = dynamic_cast<CTargetMark*>(bullet);
    if(targetMark)
    {
        targetMark
        ->setBullet(this)
        ->build();
    }
}

void CNormalMissile::createParticle_Flame()
{
    // 불꽃 파티클
    m_pParticleFlame = CParticle_Flame::create("missileFlame.png");
    if (m_pParticleFlame != nullptr){
        m_pParticleFlame->retain();
        m_pParticleFlame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_pParticleFlame->setAngle(-90);
        m_pParticleFlame->setGravity(Vec2(90, 0));
        m_pParticleFlame->setPosition(Vec2(m_pTexture->getContentSize().width * 1.1f,
                                           m_pTexture->getContentSize().height * 0.5f));
        m_pParticleFlame->setStartSpin(-90);
        m_pParticleFlame->setEndSpin(270);
        m_pParticleFlame->setLife(0.1f);
        m_pParticleFlame->setLifeVar(0.15f);
        
        m_pTexture->addChild(m_pParticleFlame);
    }
}

void CNormalMissile::createParticle_Explosion()
{
    m_pParticleCrash = CParticle_Explosion::create(MakeString("explosion_%d.png",
                                                              m_BulletParam._isAimingMissile + 1));
    if (m_pParticleCrash != nullptr){
        m_pParticleCrash->retain();
        m_pParticleCrash->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_pParticleCrash->setAngle(-getRotation());
        m_pParticleCrash->setPosition(getPosition());
        m_pParticleCrash->setGravity(m_RotationVec);
        CGameScene::getGridWorld()->addChild(m_pParticleCrash, 100);
    }
}