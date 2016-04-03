#include "NormalMissile.h"
#include "PlayCoin.h"
#include "PlayStar.h"
#include "TargetMark.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../Shooter/PatternShooter.h"
#include "../../Scene/GameScene.h"


CNormalMissile::CNormalMissile(
	std::string textureName,	    //bullet 이미지
	float boundingRadius,		    //bullet 충돌 범위
	float angle,				    //bullet 초기 각도 
	float speed,				    //bullet 초기 속도
	bool isAiming/* = false*/)		//조준미사일인지 여부				
	: CBullet(textureName,
	boundingRadius,
	angle,
	speed)
	, m_bIsAimingMissile(isAiming)
{}

CNormalMissile* CNormalMissile::create(
	std::string textureName,		//bullet 이미지
	float boundingRadius,			//bullet 충돌 범위
	float angle,					//bullet 초기 각도 
	float speed,					//bullet 초기 속도
	bool isAiming/* = false*/)		//조준미사일인지 여부
{
	CNormalMissile* pRet = 
		(CNormalMissile*)new(std::nothrow)CNormalMissile(
		textureName, boundingRadius, angle, speed, isAiming);

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
	try{
		setItemEffect(eITEM_FLAG_giant | eITEM_FLAG_coin | eITEM_FLAG_star | eITEM_FLAG_shield);

		m_ScreenRect = Rect(0, 0, 720, 1280);
		setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) * 2500.f) + m_pPlanet->getPosition().x);
		setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) * 2500.f) + m_pPlanet->getPosition().y);
		setRotation(-m_fAngle);

		m_pTexture = Sprite::create(m_TextureName);
		m_pTexture->setAnchorPoint(Vec2(0.2f, 0.5f));
		addChild(m_pTexture);
	
		CGameScene::getGameScene()->addChild(CTargetMark::create(
			MakeString("missile_target_%d.png", m_bIsAimingMissile + 1),//이미지 이름
			m_fAngle,													//초기 각도
			this->getPosition(),
			this->getBulletSpeed(),
			m_bIsAimingMissile,
			this), 100);
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}


void CNormalMissile::Rotation(int dir)
{
	// aimingMissile일 경우 화면안에 들어왔을 때에만 회전한다.
	if (true == m_bIsAimingMissile){
		if (!m_ScreenRect.containsPoint(getPosition()))
		{
			return;
		}
	}

	CBullet::Rotation(dir);
}

void CNormalMissile::Execute(float delta)
{
	getFSM()->Execute(delta);
}

void CNormalMissile::CollisionWithPlanet()
{
	
	if (true == m_bIsAimingMissile){
		CObjectManager::Instance()->getM_Planet()->CrushShake(
			0.01f, 0.5f, 0.1f, 5.0f);
		AudioEngine::play2d("sounds/explosion_2.mp3", false);
	}
	else {
		CObjectManager::Instance()->getM_Planet()->CrushShake(
			0.01f, 0.3f, 0.1f, 3.0f);
		AudioEngine::play2d("sounds/explosion_1.mp3", false);
	}
	ReturnToMemoryBlock();
}

void CNormalMissile::CollisionWithPlayer()
{
	if (CItemManager::Instance()->getCurrentItem() & eITEM_FLAG_giant){
		AudioEngine::play2d("sounds/explosion_2.mp3", false);
		R_BezierWithRotation(Vec2(920, 1580), Vec2(350, 900), Vec2(450, 1200), 0.5f);
	}
	else{
		if (true == m_bIsAimingMissile){
			CObjectManager::Instance()->getM_Planet()->CrushShake(
				0.01f, 0.5f, 0.1f, 5.0f);
			AudioEngine::play2d("sounds/explosion_2.mp3", false);
		}
		else {
			CObjectManager::Instance()->getM_Planet()->CrushShake(
				0.01f, 0.3f, 0.1f, 3.0f);
			AudioEngine::play2d("sounds/explosion_1.mp3", false);
		}
		ReturnToMemoryBlock();
	}
}

void CNormalMissile::ChangeToCoin()
{
	std::string patternName;
	if (true == m_bIsAimingMissile){
		patternName = MakeString("Rocket%d_Pattern", 1);
	}
	else{
		patternName = MakeString("Rocket%d_Pattern", 1);
	}
	float distance = m_TargetVec.distance(getPosition());

	auto CPatternShooter = CPatternShooter::create(
		sSHOOTER_PARAM(
		"pattern_Shooter"
		, patternName
		, 0.f
		, 0.f
		, 400.f
		, 0.f
		, 200.f));

	CGameScene::getGameScene()->addChild(CPatternShooter);
	CPatternShooter->ShootWithPosition(patternName, -getRotation(), distance);

	CGameScene::getGameScene()->addChild(CTargetMark::create(
		MakeString("missile_target_%d.png", m_bIsAimingMissile + 1),//이미지 이름
		-getRotation(),										//초기 각도
		this->getPosition(),
		400.f,
		false), 100);

	ReturnToMemoryBlock();
}

void CNormalMissile::ChangeToStar()
{
	eSTAR_TYPE starType;
	if (true == m_bIsAimingMissile){
		starType = eSTAR_TYPE_bigGold;
	}
	else{
		starType = eSTAR_TYPE_bigSilver;
	}
	float distance = m_TargetVec.distance(getPosition());
	this->setBulletSpeed(250.f);

	//auto CPatternShooter = CPatternShooter::create(
	//	sSHOOTER_PARAM(
	//	eSHOOTER_TYPE_screwBullet
	//	, starType
	//	, 0.f
	//	, 0.f
	//	, 500.f
	//	, 0.f
	//	, 200.f));

	//CGameScene::getGameScene()->addChild(CPatternShooter);
	//CPatternShooter->ShootWithPosition(-getRotation(), distance);

	CGameScene::getGameScene()->addChild(CTargetMark::create(
		MakeString("missile_target_%d.png", m_bIsAimingMissile + 1),//이미지 이름
		-getRotation(),										//초기 각도
		this->getPosition(),
		this->getBulletSpeed(),
		false), 100);

	ReturnToMemoryBlock();
}