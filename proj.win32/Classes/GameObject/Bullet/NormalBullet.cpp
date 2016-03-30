#include "NormalBullet.h"
#include "PlayCoin.h"
#include "PlayStar.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../Scene/GameScene.h"

CNormalBullet::CNormalBullet(
	std::string textureName,	    //bullet 이미지
	float boundingRadius,		    //bullet 충돌 범위
	float angle,				    //bullet 초기 각도 
	float speed)				    //bullet 초기 속도
	: CBullet(
	textureName, 
	boundingRadius, 
	angle, 
	speed)
{}

CNormalBullet* CNormalBullet::create(
	std::string textureName,		//bullet 이미지
	float boundingRadius,			//bullet 충돌 범위
	float angle,					//bullet 초기 각도 
	float speed)					//bullet 초기 속도
{
	CNormalBullet* pRet = 
		(CNormalBullet*)new(std::nothrow)CNormalBullet(
		textureName, boundingRadius, angle, speed);

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

		setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) * 1000.f) + m_pPlanet->getPosition().x);
		setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) * 1000.f) + m_pPlanet->getPosition().y);
		setRotation(-m_fAngle);

		m_pTexture = Sprite::create(m_TextureName);
		m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
		addChild(m_pTexture);
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
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
		R_ScaleWithFadeOut(2.f, 0.5f, 0.5f);
	}
	else{
		ReturnToMemoryBlock();
	}
}

void CNormalBullet::ChangeToCoin()
{
	//CGameScene::getGameScene()->addChild(CPlayCoin::create(eCOIN_TYPE_bronze, 25.f, m_fAngle, m_fBulletSpeed, true, getPosition()));
	ReturnToMemoryBlock();
}

void CNormalBullet::ChangeToStar()
{
	//CGameScene::getGameScene()->addChild(CPlayStar::create(eSTAR_TYPE_bronze, 25.f, m_fAngle, m_fBulletSpeed, true, getPosition()));
	ReturnToMemoryBlock();
}