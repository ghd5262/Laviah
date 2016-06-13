#include "NormalBullet.h"
#include "PlayCoin.h"
#include "PlayStar.h"
#include "ScoreBullet.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
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
	try{
		setItemEffect(eITEM_FLAG_giant | eITEM_FLAG_coin | eITEM_FLAG_star | eITEM_FLAG_shield);

		setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_BulletParam._fDistance) + m_pPlanet->getPosition().x);
		setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_BulletParam._fDistance) + m_pPlanet->getPosition().y);
		setRotation(-m_fAngle);

		m_pTexture = Sprite::create(m_BulletParam._TextureName);
		m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
		addChild(m_pTexture);

		m_pMultipleScore = static_cast<CMultipleScore*>(CUIManager::Instance()->FindUIWithName("MultipleScoreUI"));

		m_pUIScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("StarScoreUI"));
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CNormalBullet::Execute(float delta)
{
	getFSM()->Execute(delta);
}

void CNormalBullet::CollisionWithPlanet()
{
	ReturnToMemoryBlock();
}

void CNormalBullet::CollisionWithPlayer()
{
	if (CItemManager::Instance()->getCurrentItem() & eITEM_FLAG_giant){
		createScoreCurrentPos(30);
		R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
	}
	else{
		m_pPlayer->StackedRL(0.1f, 10, 10, 5);
		m_pPlayer->LostSomeHealth(m_BulletParam._fPower);
		CAudioManager::Instance()->PlayEffectSound("sounds/hit.mp3", false);
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
			MakeString("star_%d.png", static_cast<int>(m_BulletParam._symbol - '0')),
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
			MakeString("coin_%d.png", static_cast<int>(m_BulletParam._symbol - '0')),
			25.f, distance, 0.f, true, false,
			static_cast<eCOIN_TYPE>(m_BulletParam._symbol - '0')),
			-getRotation(),
			m_fBulletSpeed,
			getPosition()));
	}

	ReturnToMemoryBlock();
}