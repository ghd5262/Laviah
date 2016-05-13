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
#include "../MyUI/Popup.h"
#include "../MyUI/Popup/PausePopup.h"
#include "../MyUI/Popup/ResultPopup.h"
#include "../DataManager/BulletPatternDataManager.h"
#include "../DataManager/StageDataManager.h"
#include "../DataManager/BulletDataManager.h"
#include "../AI/States/StageStates.h"

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
		CStageDataManager::Instance();
		CBulletPatternDataManager::Instance();
		CBulletDataManager::Instance();

		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		auto background = Sprite::create("background_2.png");
		background->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.y + visibleSize.height * 0.5f));
		this->addChild(background);

		auto planet = CPlanet::create("planet.png", 170, 0.0f, 200.0f);
		planet->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.y + visibleSize.height * 0.25f));
		this->addChild(planet, 100);

		planet->setOriginPos(planet->getPosition());

		auto player = CPlayer::create("player.png", "player_big.png", 6.f, 0.0f, 400.0f, 50.f);
		player->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			planet->getPosition().y + (planet->getBRadius() + 20.f)));
		this->addChild(player, 100);

		player->setOriginPos(player->getPosition());
		player->setParticlePos(player->getPosition());

		CObjectManager::Instance()->setPlayer(player);
		CObjectManager::Instance()->setPlanet(planet);
		CPoolingManager::Instance()->CreateBulletList(1300, 800);
		CPoolingManager::Instance()->CreateShooterList(10, 800);

		InitGameSceneUI();
		GameStart();

		CAudioManager::Instance()->PlayBGM("sounds/bgm_1.mp3", true);
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CGameScene::InitGameSceneUI()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    auto closeItem = MenuItemImage::create("CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(CGameScene::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 103);

    
    // RotationObjectLeft callback 등록
	auto leftButton = CMyButton::createWithTexture(
		"leftButton_1.png",
		"leftButton_2.png",
		EXECUTE,
		std::bind(&CObjectManager::RotationObject, CObjectManager::Instance(), -1.f));

	leftButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	leftButton->setPosition(Vec2(origin.x + visibleSize.width * 0.25f,
		origin.x + visibleSize.height * 0.5f));

	this->addChild(leftButton, 102);
	if (!CUIManager::Instance()->AddUIWithName(leftButton, "LButton"))
		CCASSERT(false, "LBUTTON CAN NOT INIT");

	// RotationObjectRight callback 등록
	auto rightButton = CMyButton::createWithTexture(
		"rightButton_1.png",
		"rightButton_2.png",
		EXECUTE,
		std::bind(&CObjectManager::RotationObject, CObjectManager::Instance(), 1.f));

	rightButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	rightButton->setPosition(Vec2(origin.x + visibleSize.width * 0.75f,
		origin.x + visibleSize.height * 0.5f));

	this->addChild(rightButton, 102);
	if (!CUIManager::Instance()->AddUIWithName(rightButton, "RButton"))
		CCASSERT(false, "RBUTTON CAN NOT INIT");

	// player의 HealthCalFunc callback 등록
	auto healthBar = CHealthBarUI::create(
		std::bind(&CPlayer::HealthCalculatorInNormal, CObjectManager::Instance()->getPlayer(), std::placeholders::_1/*= 호출하는 곳의 인자를 사용한다.*/));

	healthBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	healthBar->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
		origin.x + visibleSize.height * 0.945f));
	this->addChild(healthBar, 102);
	if (!CUIManager::Instance()->AddUIWithName(healthBar, "HealthBar"))
		CCASSERT(false, "HealthBar CAN NOT INIT");

	auto bonusTime = CBonusTimeUI::create();
	bonusTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	bonusTime->setPosition(Vec2(origin.x + visibleSize.width * 0.08f,
		origin.x + visibleSize.height * 0.905f));
	this->addChild(bonusTime, 102);
	if (!CUIManager::Instance()->AddUIWithName(bonusTime, "BonusTime"))
		CCASSERT(false, "BonusTime CAN NOT INIT");

	auto starScoreUI = CScoreUI::create("fonts/Number.ttf", 25, "score.png");
	starScoreUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	starScoreUI->SetLabelAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
	starScoreUI->setPosition(Vec2(origin.x + visibleSize.width * 0.94f,
		origin.x + visibleSize.height * 0.98f));
	this->addChild(starScoreUI, 102);
	if (!CUIManager::Instance()->AddUIWithName(starScoreUI, "StarScoreUI"))
		CCASSERT(false, "StarScoreUI CAN NOT INIT");


	auto coinScoreUI = CScoreUI::create("fonts/Number.ttf", 25, "coin_2.png");
	coinScoreUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	coinScoreUI->SetLabelAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
	coinScoreUI->setPosition(Vec2(origin.x + visibleSize.width * 0.1f,
		origin.x + visibleSize.height * 0.98f));
	this->addChild(coinScoreUI, 102);
	if (!CUIManager::Instance()->AddUIWithName(coinScoreUI, "CoinScoreUI"))
		CCASSERT(false, "CoinScoreUI CAN NOT INIT");
    
    auto runScoreUI = CScoreUI::create("fonts/Number.ttf", 25, "run.png");
    runScoreUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    runScoreUI->SetLabelAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
    runScoreUI->setPosition(Vec2(origin.x + visibleSize.width * 0.94f,
        origin.x + visibleSize.height * 0.905f));
    this->addChild(runScoreUI, 102);
    if (!CUIManager::Instance()->AddUIWithName(runScoreUI, "RunScoreUI"))
        CCASSERT(false, "RunScoreUI CAN NOT INIT");

	auto CreateOneBtnPopup = CMyButton::createWithString("defaultBtn_1.png",
		"Default_1",
		25,
		Color3B::BLACK,
		END,
		[this, origin, visibleSize]()
	{
		GameEnd();
		auto popup = CPopup::createWithSpecificFormat(CResultPopup::create(), POPUPEFFECT_none);
		popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.x + visibleSize.height * 0.5f));
		popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		this->addChild(popup, 102);
	}, EFFECT_SIZEDOWN);

	CreateOneBtnPopup->setPosition(Vec2(origin.x + visibleSize.width * 0.94f,
		origin.x + visibleSize.height * 0.8f));
	CreateOneBtnPopup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(CreateOneBtnPopup, 102);

	m_PauseBtn = nullptr;
	m_PauseBtn = CMyButton::create("pauseIcon.png",
		END,
		[this, origin, visibleSize]()
	{
		GamePause();
		auto popup = CPopup::createWithSpecificFormat(CPausePopup::create(), POPUPEFFECT_none);
		popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.x + visibleSize.height * 0.5f));
		popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		this->addChild(popup, 102);
	}, EFFECT_SIZEDOWN);

	m_PauseBtn->setPosition(Vec2(origin.x + visibleSize.width * 0.08f,
		origin.x + visibleSize.height * 0.05f));
	m_PauseBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_PauseBtn->setCascadeOpacityEnabled(true);
	this->addChild(m_PauseBtn, 102);

	m_CountDownLabel = nullptr;
	m_CountDownLabel = Label::create("3", "fonts/malgunbd.ttf", 40);
	m_CountDownLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_CountDownLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
		origin.x + visibleSize.height * 0.5f));
	this->addChild(m_CountDownLabel, 102);
	m_CountDownLabel->setVisible(false);
}

void CGameScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	

	/*utils::captureScreen([this](bool succed, std::string name)
	{

		auto sp = Sprite::create(name);
		addChild(sp, 102);
		Size s = Director::getInstance()->getWinSize();
		sp->setPosition(s.width / 2, s.height / 2);

		auto fileUtiles = FileUtils::getInstance();
		auto fragmentFullPath = fileUtiles->fullPathForFilename("shader/example_Blur.fsh");
		auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
		auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		_fragSource = fragSource;
#endif
		auto _glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
		_glprogramstate->setUniformVec2("resolution", sp->getContentSize());
		_glprogramstate->setUniformFloat("blurRadius", 10.f);
		_glprogramstate->setUniformFloat("sampleNum", 5.f);
		_glprogramstate->retain();
		sp->setGLProgramState(_glprogramstate);
	}, "screenShot.png");
*/
	
	GamePause();

	auto btnYes = CMyButton::createWithString("defaultBtn_1.png",
		"Yes",
		25,
		Color3B::WHITE,
		END,
		[this](){
		Director::getInstance()->end();
	}, EFFECT_ALPHA);

	auto btnNo = CMyButton::createWithString("defaultBtn_2.png",
		"Yes",
		25,
		Color3B::WHITE,
		END,
		[this](){
		Director::getInstance()->end();
	}, EFFECT_ALPHA);

	auto popup = CPopup::createWithTwoButton("Are you sure you want to exit StarStarStar?"
		, btnNo, btnYes, 25, Color3B::BLACK);
	popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
		origin.x + visibleSize.height * 0.5f));
	popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(popup, 102);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void CGameScene::update(float delta)
{
	CUIManager::Instance()->Execute(delta);
	CObjectManager::Instance()->Execute(delta);
}

void CGameScene::GameStart()
{
	countDownLabel();
	m_PauseBtn->runAction(FadeIn::create(0.5f));
	CObjectManager::Instance()->getPlayer()->PlayerAlive();
	CObjectManager::Instance()->getFSM()->ChangeState(CGameCountDownState::Instance());
}

void CGameScene::GamePause()
{
	//오디오 소리 작게
	CAudioManager::Instance()->setBGMVolume(0.1f);
	CAudioManager::Instance()->setEffectSoundVolume(0.1f);
	CObjectManager::Instance()->setIsGamePause(true);
	CObjectManager::Instance()->setIsAbleRotation(false);
}

void CGameScene::GameResume()
{
	countDownLabel();
	//오디오 소리 크게
	CAudioManager::Instance()->setBGMVolume(1.f);
	CAudioManager::Instance()->setEffectSoundVolume(1.f);
	CObjectManager::Instance()->setIsGamePause(false);
	CObjectManager::Instance()->getFSM()->ChangeState(CGameCountDownState::Instance());
}

void CGameScene::GameEnd()
{
	m_PauseBtn->runAction(FadeTo::create(0.5f, 0));
	CObjectManager::Instance()->getPlayer()->PlayerDead();
	GamePause();
}

void CGameScene::countDownLabel()
{
	m_Count = 3;
	m_CountDownLabel->setVisible(true);
	this->schedule([this](float delta)
	{
		m_Count -= 1;
		if (m_Count > 0)
			m_CountDownLabel->setString(StringUtils::format("%d", m_Count));
		else if (m_Count == 0)
			m_CountDownLabel->setString("Go");
		else{
			m_CountDownLabel->setVisible(false);
			m_CountDownLabel->setString("3");
		}
	}, 0.7f, 4, 0.f, "countDown");
}