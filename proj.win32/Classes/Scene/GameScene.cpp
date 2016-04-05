#include "GameScene.h"
#include "../Task/PoolingManager.h"
#include "../GameObject/Planet.h"
#include "../GameObject/Player.h"
#include "../GameObject/ObjectManager.h"
#include "../GameObject/Shooter/ShooterHeaders.h"
#include "../GameObject/Bullet/Bullet.h"
#include "../MyUI/UIManager.h"
#include "../MyUI/MyButton.h"
#include "../MyUI/HealthBarUI.h"
#include "../MyUI/BonusTimeUI.h"
#include "../MyUI/ScoreUI.h"
#include "../DataManager/BulletPatternDataManager.h"
#include "../DataManager/ShooterDataManager.h"
#include "../DataManager/BulletDataManager.h"

USING_NS_CC;

CGameScene* CGameScene::m_GameScene = nullptr;
Scene* CGameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = CGameScene::create();
	// add layer as a child to scene
	scene->setAnchorPoint(Vec2(0, 0));
	layer->setAnchorPoint(Vec2(0, 0));
	scene->addChild(layer);

	return scene;
}

CGameScene::~CGameScene()
{
	CObjectManager::Instance()->RemoveAllObject();
	removeAllChildrenWithCleanup(true);
}

bool CGameScene::init()
{
	scheduleUpdate();
	if (!Layer::init())
	{
		return false;
	}

	if (!initVariable())
		return false;
	return true;
}

bool CGameScene::initVariable()
{
	try
	{
		m_GameScene = this;
		CBulletPatternDataManager::Instance();
		CBulletDataManager::Instance();

		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		auto background = Sprite::create("background_2.png");
		background->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.y + visibleSize.height * 0.5f));
		this->addChild(background);

		auto planet = CPlanet::create("planet.png", 170, 0.0f, 2.0f);
		planet->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.y + visibleSize.height * 0.25f));
		this->addChild(planet, 100);

		planet->setOriginPos(planet->getPosition());

		auto player = CPlayer::create("player.png", "player_big.png", 6.f, 0.0f, 5.0f, 100.f);
		player->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			planet->getPosition().y + (planet->getBRadius() + 20.f)));
		this->addChild(player, 100);

		player->setOriginPos(player->getPosition());

		CObjectManager::Instance()->setM_Player(player);
		CObjectManager::Instance()->setM_Planet(planet);
		CPoolingManager::Instance()->CreateBulletList(1500, 800);
		CPoolingManager::Instance()->CreateShooterList(5, 800);



	/*	auto randomShooter = CShooterListDataManager::Instance()->getShooterInfo("random_Shooter");
		randomShooter->setShooterParam(sSHOOTER_PARAM("", 0.f, 0.f, 250.f, 0.f, 0.5f, 3));
		randomShooter->setAlive(true);

		auto randomMissileShooter = CShooterListDataManager::Instance()->getShooterInfo("randomMissile_Shooter");
		randomMissileShooter->setShooterParam(sSHOOTER_PARAM("", 0.f, 0.f, 600.f, 0.f, 10.f, 1));
		randomMissileShooter->setAlive(true);

		auto aimingMissileShooter = CShooterListDataManager::Instance()->getShooterInfo("aimingMissile_Shooter");
		aimingMissileShooter->setShooterParam(sSHOOTER_PARAM("", 0.f, 0.f, 1200.f, 0.f, 15.f, 1));
		aimingMissileShooter->setAlive(true);*/

		/*auto screwShooter = CShooterListDataManager::Instance()->getShooterInfo("screw_Shooter");
		screwShooter->setShooterParam(sSHOOTER_PARAM("screw_Shooter", "", 0.f, 10.f, 300.f, 0.f, 25.f, 1));
		screwShooter->setAlive(true);*/

		//auto letterShooter = CShooterListDataManager::Instance()->getShooterInfo("bonusLetter_Shooter");
		//letterShooter->setShooterParam(sSHOOTER_PARAM("", 0.f, 0.f, 100.f, 0.f, 30.f));
		//letterShooter->setAlive(true);


		//auto screwShooter = CPatternShooter::create(sSHOOTER_PARAM("leftScrewRocket1_Pattern", 0.f, 0.f, 900.f, -30.f, 30.f));//CShooterListDataManager::Instance()->getShooterInfo("pattern_Shooter");
		//addChild(screwShooter);
		//screwShooter->setShooterParam();
		//screwShooter->setAlive(true);

		auto screwShooter1 = CPatternShooter::create(sSHOOTER_PARAM("rightScrewRocket1_Pattern", 0.f, 0.f, 900.f, -30.f, 40.f));//CShooterListDataManager::Instance()->getShooterInfo("pattern_Shooter");
		addChild(screwShooter1);

		auto randomShooter = CRandomShooter::create(sSHOOTER_PARAM("leftScrewRocket1_Pattern", 0.f, 0.f, 250.f, -30.f, 0.5f));
		addChild(randomShooter);


		auto randomShooter1 = CPatternShooter::create(sSHOOTER_PARAM("rightScrew1_Pattern", 0.f, 0.f, 250.f, -30.f, 20.5f));
		addChild(randomShooter1);

		auto randomShooter2 = CPatternShooter::create(sSHOOTER_PARAM("temp0", 0.f, 0.f, 150.f, -30.f, 15.5f));
		addChild(randomShooter2);
		auto randomShooter3 = CPatternShooter::create(sSHOOTER_PARAM("temp1", 0.f, 0.f, 1200.f, 70.f, 15.5f));
		addChild(randomShooter3);
		auto randomShooter4 = CPatternShooter::create(sSHOOTER_PARAM("temp2", 0.f, 0.f, 900.f, -30.f, 10.5f, 2));
		addChild(randomShooter4);
		auto randomShooter5 = CPatternShooter::create(sSHOOTER_PARAM("temp3", 0.f, 0.f, 900.f, 70.f, 12.8f, 2));
		addChild(randomShooter5);

		//randomShooter->setShooterParam();
		//randomShooter->setAlive(true);

		//this->addChild(CRandomShooter::create(sSHOOTER_PARAM("", "normalShooter", 0, 0, 250.0f, 90, 0.5f, 300)));
		//RandomMissileShoot(600.f, 10.0f, 2);
		//this->addChild(CAimingMissileShooter::create(sSHOOTER_PARAM(eSHOOTER_TYPE_aimingMissile, "aimingShooter", 0.f, 0.f, 1200.0f, 90, 15.0f)));
		//DoubleScrewShoot(250.0f, 0.1f, 1, eSHOOTER_OPTION_left);
		//ScrewShoot(250.0f, 0.1f, 1, eSHOOTER_OPTION_right);
		//this->addChild(CPatternShooter::create(sSHOOTER_PARAM(eSHOOTER_TYPE_bonusTime_4, 0, 0, 250, 90.0f, 10.0f)));
		InitGameSceneUI();

		AudioEngine::play2d("sounds/bgm_1.mp3", true);
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CGameScene::InitGameSceneUI()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// RotationObjectLeft callback 등록
	auto leftButton = CMyButton::createWithTexture(
		"leftButton_1.png",
		"leftButton_2.png",
		EXECUTE,
		std::bind(&CObjectManager::RotationObject, CObjectManager::Instance(), -1));

	leftButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	leftButton->setPosition(Vec2(origin.x + visibleSize.width * 0.25f,
		origin.x + visibleSize.height * 0.5f));

	this->addChild(leftButton, 101);
	if (!CUIManager::Instance()->AddUIWithName(leftButton, "LButton"))
		CCASSERT(false, "LBUTTON CAN NOT INIT");

	// RotationObjectRight callback 등록
	auto rightButton = CMyButton::createWithTexture(
		"RightButton_1.png",
		"RightButton_2.png",
		EXECUTE,
		std::bind(&CObjectManager::RotationObject, CObjectManager::Instance(), 1));

	rightButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	rightButton->setPosition(Vec2(origin.x + visibleSize.width * 0.75f,
		origin.x + visibleSize.height * 0.5f));

	this->addChild(rightButton, 101);
	if (!CUIManager::Instance()->AddUIWithName(rightButton, "RButton"))
		CCASSERT(false, "RBUTTON CAN NOT INIT");

	// player의 HealthCalFunc callback 등록
	auto healthBar = CHealthBarUI::create(
		std::bind(&CPlayer::HealthCalculatorInNormal, CObjectManager::Instance()->getM_Player(), std::placeholders::_1/*= 호출하는 곳의 인자를 사용한다.*/));

	healthBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	healthBar->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
		origin.x + visibleSize.height * 0.95f));
	this->addChild(healthBar, 100);
	if (!CUIManager::Instance()->AddUIWithName(healthBar, "HealthBar"))
		CCASSERT(false, "HealthBar CAN NOT INIT");

	auto bonusTime = CBonusTimeUI::create();
	bonusTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	bonusTime->setPosition(Vec2(origin.x + visibleSize.width * 0.08f,
		origin.x + visibleSize.height * 0.915f));
	this->addChild(bonusTime, 100);
	if (!CUIManager::Instance()->AddUIWithName(bonusTime, "BonusTime"))
		CCASSERT(false, "BonusTime CAN NOT INIT");

	auto starScoreUI = CScoreUI::create("fonts/Number.ttf", 25, "star_2.png");
	starScoreUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	starScoreUI->SetLabelAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
	starScoreUI->setPosition(Vec2(origin.x + visibleSize.width * 0.94f,
		origin.x + visibleSize.height * 0.98f));
	this->addChild(starScoreUI, 100);
	if (!CUIManager::Instance()->AddUIWithName(starScoreUI, "StarScoreUI"))
		CCASSERT(false, "StarScoreUI CAN NOT INIT");


	auto coinScoreUI = CScoreUI::create("fonts/Number.ttf", 25, "coin_2.png");
	coinScoreUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	coinScoreUI->SetLabelAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
	coinScoreUI->setPosition(Vec2(origin.x + visibleSize.width * 0.1f,
		origin.x + visibleSize.height * 0.98f));
	this->addChild(coinScoreUI, 100);
	if (!CUIManager::Instance()->AddUIWithName(coinScoreUI, "CoinScoreUI"))
		CCASSERT(false, "CoinScoreUI CAN NOT INIT");


}

void CGameScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void CGameScene::update(float delta)
{
	CUIManager::Instance()->Execute(delta);
	CObjectManager::Instance()->Execute(delta);
}