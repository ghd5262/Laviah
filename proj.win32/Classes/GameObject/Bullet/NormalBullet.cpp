#include "NormalBullet.h"
#include "PlayCoin.h"
#include "PlayStar.h"
#include "ScoreBullet.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../Particle/Particles.h"
#include "../../Scene/GameScene.h"
#include "../../MyUI/ScoreUI.h"
#include "../../MyUI/UIManager.h"
#include "../../MyUI/MultipleScore.h"

CNormalBullet::CNormalBullet(
	sBULLET_PARAM bulletParam,
	float angle,				    //bullet 초기 각도 
	float speed)				    //bullet 초기 속도
	: CBullet(
	bulletParam,
	angle, 
	speed)
	, m_pParticleCrash(nullptr)
{}

CNormalBullet* CNormalBullet::create(
	sBULLET_PARAM bulletParam,
	float angle,					//bullet 초기 각도 
	float speed)					//bullet 초기 속도
{
	CNormalBullet* pRet = 
		(CNormalBullet*)new(std::nothrow)CNormalBullet(
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

bool CNormalBullet::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CNormalBullet::initVariable()
{
	setItemEffect(eITEM_FLAG_giant | eITEM_FLAG_coin | eITEM_FLAG_star | eITEM_FLAG_shield);

	setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_BulletParam._fDistance) + m_pPlanet->getPosition().x);
	setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_BulletParam._fDistance) + m_pPlanet->getPosition().y);
	setRotation(-m_fAngle);

	m_pTexture = Sprite::createWithSpriteFrameName(m_pPlayer->getCharacterParam()._normalBulletTextureName);
	if (m_pTexture != nullptr){
		m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
		addChild(m_pTexture);
	}
	m_pMultipleScore = static_cast<CMultipleScore*>(CUIManager::Instance()->FindUIWithName("MultipleScoreUI"));

	m_pUIScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("StarScoreUI"));

	return true;
}

void CNormalBullet::Execute(float delta)
{
	getFSM()->Execute(delta);
}

void CNormalBullet::CollisionWithPlanet()
{
	ReturnToMemoryBlock();

	m_pParticleCrash = CParticle_Explosion::create(m_pPlayer->getCharacterParam()._normalBulletTextureName);
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

void CNormalBullet::CollisionWithPlayer()
{
	if (CItemManager::Instance()->getCurrentItem() & eITEM_FLAG_giant){
		createScoreCurrentPos(30);
		R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
	}
	else{
		m_pPlayer->StackedRL(0.1f, 10, 10, 5);
		CCLOG("Bullet Power %f", m_BulletParam._fPower);
		m_pPlayer->LostSomeHealth(m_BulletParam._fPower);
        ReturnToMemoryBlock();
	}
}

void CNormalBullet::CollisionWithBarrier()
{
	createScoreCurrentPos(30);
	R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
}

void CNormalBullet::ChangeToCoinOrStar()
{
	float distance = m_TargetVec.distance(getPosition());

	if (CItemManager::Instance()->getCurrentItem() & eITEM_FLAG_star){
		float distance = m_TargetVec.distance(getPosition());
		CGameScene::getGridWorld()->addChild(CPlayStar::create(
			sBULLET_PARAM(
			25.f, distance, 0.f, true, false,
			eCOIN_TYPE_none,
			static_cast<eSTAR_TYPE>(m_BulletParam._symbol - '0')),
			-getRotation(),
			m_fBulletSpeed,
			getPosition()));
	}
	else{
		CGameScene::getGridWorld()->addChild(CPlayCoin::create(
			sBULLET_PARAM(
			25.f, distance, 0.f, true, false,
			static_cast<eCOIN_TYPE>(m_BulletParam._symbol - '0')),
			-getRotation(),
			m_fBulletSpeed,
			getPosition()));
	}

	ReturnToMemoryBlock();
}