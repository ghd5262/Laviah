#include "PlayStar.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../MyUI/ScoreUI.h"
#include "../../MyUI/UIManager.h"
#include "../../MyUI/MultipleScore.h"
#include "../../Particle/Particles.h"
#include "../../Scene/GameScene.h"

CPlayStar::CPlayStar(
	sBULLET_PARAM bulletParam,
	float angle,				    //star 초기 각도 
	float speed,				    //star 초기 속도
	Vec2 createPosition/* = Vec2(0, 0)*/)

	: CBullet(
	bulletParam,
	angle,
	speed)
	, m_CreatePos(createPosition)
	, m_pParticleCrash(nullptr)
{}

CPlayStar* CPlayStar::create(
	sBULLET_PARAM bulletParam,
	float angle,					//star 초기 각도 
	float speed,					//star 초기 속도
	Vec2 createPosition)
{
	CPlayStar* pRet =
		(CPlayStar*)new(std::nothrow)CPlayStar(
		bulletParam, angle, speed, createPosition);

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

bool CPlayStar::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CPlayStar::initVariable()
{
	setItemEffect(eITEM_FLAG_magnet);

	if (!m_BulletParam._isFly){
		m_BulletParam._fDistance = m_pPlanet->getBRadius() + 20;

		this->scheduleOnce([this](float delta)
		{
			this->R_FadeOutWithCount(5, 3.f);
		}, 5.f, MakeString("AutoRemove_%d", random<int>(1, 100)));
	}
	setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) * m_BulletParam._fDistance) + m_pPlanet->getPosition().x);
	setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) * m_BulletParam._fDistance) + m_pPlanet->getPosition().y);
	setRotation(-m_fAngle);

	m_pUIScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("StarScoreUI"));

	m_pTexture = Sprite::create(MakeString("star_%d.png", m_BulletParam._starType));
	m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
	addChild(m_pTexture);

	m_fStarValue = CItemManager::Instance()->getValueOfStar(m_BulletParam._starType);

	m_pMultipleScore = static_cast<CMultipleScore*>(CUIManager::Instance()->FindUIWithName("MultipleScoreUI"));

	return true;
}

void CPlayStar::Execute(float delta)
{
	getFSM()->Execute(delta);
}

void CPlayStar::CollisionWithPlanet()
{
	if (true == m_BulletParam._isFly)
	{
		ReturnToMemoryBlock();

		m_pParticleCrash = CParticle_Explosion::create("coin_5.png");
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
}

void CPlayStar::CollisionWithPlayer()
{
	CAudioManager::Instance()->PlayEffectSound("sounds/Star_2.mp3", false);
	R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
	m_pMultipleScore->AddScore(m_fStarValue);
}
