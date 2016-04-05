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
#include "../../DataManager/ShooterDataManager.h"

CNormalMissile::CNormalMissile(
	sBULLET_PARAM bulletParam,
	float angle,				    //bullet 초기 각도 
	float speed)				    //bullet 초기 속도
	: CBullet(
	bulletParam,
	angle,
	speed)
	, m_bIsTargetMarkCreate(false)
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
	try{
		setItemEffect(eITEM_FLAG_giant | eITEM_FLAG_coin | eITEM_FLAG_star | eITEM_FLAG_shield);

		m_ScreenRect = Rect(-560, 0, 1280, 1280);
		setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_BulletParam._fDistance) + m_pPlanet->getPosition().x);
		setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) *  m_BulletParam._fDistance) + m_pPlanet->getPosition().y);
		setRotation(-m_fAngle);

		m_pTexture = Sprite::create(m_BulletParam._TextureName);
		m_pTexture->setAnchorPoint(Vec2(0.2f, 0.5f));
		addChild(m_pTexture);
	

		Vec2 temp((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) *  1800.f) + m_pPlanet->getPosition().x,
			(sin(CC_DEGREES_TO_RADIANS(m_fAngle)) *  1800.f) + m_pPlanet->getPosition().y);

		float distance = getPosition().distance(temp);
		float tempTime = (distance / m_fBulletSpeed);


		this->scheduleOnce([&](float delta){
			CGameScene::getGameScene()->addChild(CTargetMark::create(
				sBULLET_PARAM(
				MakeString("missile_target_%d.png", m_BulletParam._isAimingMissile + 1),//이미지 이름
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
	if (true == m_BulletParam._isAimingMissile){
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
	
	if (true == m_BulletParam._isAimingMissile){
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
		if (true == m_BulletParam._isAimingMissile){
			CObjectManager::Instance()->getM_Planet()->CrushShake(
				0.01f, 0.5f, 0.1f, 5.0f);
			AudioEngine::play2d("sounds/explosion_2.mp3", false);
		}
		else {
			CObjectManager::Instance()->getM_Planet()->CrushShake(
				0.01f, 0.2f, 0.1f, 3.0f);
			AudioEngine::play2d("sounds/explosion_1.mp3", false);
		}
		ReturnToMemoryBlock();
	}
}

void CNormalMissile::ChangeToCoin()
{
	if (!m_bIsTargetMarkCreate)
		return;

	std::string patternName;
	if (false == m_BulletParam._isAimingMissile){
		patternName = MakeString("Rocket%d_Pattern", 17);
	}
	else{
		patternName = MakeString("Rocket%d_Pattern", 18);
	}
	float distance = m_TargetVec.distance(getPosition());
	float CoinSpeed = 600.f;
	auto screwShooter = CShooterListDataManager::Instance()->getShooterInfo("pattern_Shooter");
	screwShooter->ShootWithPosition(sSHOOTER_PARAM(
		patternName
		, 0.f
		, 0.f
		, CoinSpeed
		, 0.f
		, 0.f), -getRotation(), distance);

	CGameScene::getGameScene()->addChild(CTargetMark::create(
		sBULLET_PARAM(
		MakeString("missile_target_%d.png", m_BulletParam._isAimingMissile + 1),//이미지 이름
		0.f,
		0.f,
		0.f,
		false,		// Fly Item 인지
		false),		// AimingMissile 인지
		-getRotation(),										//초기 각도
		this->getPosition(),
		CoinSpeed
	), 100);

	ReturnToMemoryBlock();
}

void CNormalMissile::ChangeToStar()
{
	if (!m_bIsTargetMarkCreate)
		return;

	eSTAR_TYPE starType;
	if (true == m_BulletParam._isAimingMissile){
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
		sBULLET_PARAM(
		MakeString("missile_target_%d.png", m_BulletParam._isAimingMissile + 1),//이미지 이름
		0.f,
		0.f,
		0.f,
		false,
		m_BulletParam._isAimingMissile),
		-getRotation(),										//초기 각도
		this->getPosition(),
		400.f
		), 100);

	ReturnToMemoryBlock();
}