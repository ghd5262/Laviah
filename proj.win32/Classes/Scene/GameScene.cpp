#include "GameScene.h"
#include "EmptyScene.h"
#include "MenuScene.h"
#include "SceneTransition.h"
#include "../Task/PoolingManager.h"
#include "../GameObject/Planet.h"
#include "../GameObject/Player.h"
#include "../GameObject/ObjectManager.h"
#include "../GameObject/ItemManager.h"
#include "../GameObject/Shooter/ShooterHeaders.h"
#include "../GameObject/Bullet/Bullet.h"
#include "../MyUI/UIManager.h"
#include "../MyUI/MyButton.h"
#include "../MyUI/HealthBarUI.h"
#include "../MyUI/BonusTimeUI.h"
#include "../MyUI/ScoreUI.h"
#include "../MyUI/MultipleScore.h"
#include "../MyUI/Popup.h"
#include "../MyUI/Popup/PausePopup.h"
#include "../MyUI/Popup/ResultPopup.h"
#include "../MyUI/Popup/VideoPopup.h"
#include "../MyUI/Popup/HelpPopup.h"
#include "../AI/States/StageStates.h"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/CharacterDataManager.h"

USING_NS_CC;

CGameScene* CGameScene::m_GameScene = nullptr;
cocos2d::NodeGrid* CGameScene::m_GridWorld = nullptr;

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
	CPoolingManager::Instance()->DeleteAllMemory();
	clearData();
}

void CGameScene::clearData()
{
	m_GameScene = nullptr;
	m_GridWorld = nullptr;
	m_PauseBtn = nullptr;
	m_CountDownLabel = nullptr;
	m_KeyBoardL = false;
	m_KeyBoardR = false;
	m_Count = 0;
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
		clearData();
		m_GameScene = this;
		m_KeyBoardL = false;
		m_KeyBoardR = false;

		CObjectManager::Instance()->Clear();
		CUIManager::Instance()->Clear();
		CAudioManager::Instance()->Clear();
		CItemManager::Instance()->Clear();

		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		m_GridWorld = NodeGrid::create();
		addChild(m_GridWorld, 0, 1);

		int currentCharacterIdx = CUserDataManager::Instance()->getUserData_Number("USER_CUR_CHARACTER");
		sCHARACTER_PARAM currentCharacterInfo = CCharacterDataManager::Instance()->getCharacterInfoByIndex(currentCharacterIdx);

		auto background = Sprite::create("background_2.png");
		background->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.y + visibleSize.height * 0.5f));
		m_GridWorld->addChild(background);

		auto planet = CPlanet::create(currentCharacterInfo._planetTextureName, 180, 0.0f, 100.f);
		planet->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.y + visibleSize.height * 0.35f));
		m_GridWorld->addChild(planet, 100);

		planet->setOriginPos(planet->getPosition());

		CCharacterDataManager::Instance()->PrintCharacterInfo(currentCharacterInfo._idx);
		auto player = CPlayer::create(currentCharacterInfo, 6.f, 0.0f, 400.0f);

		player->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			planet->getPosition().y + (planet->getBRadius() + 20)));
		m_GridWorld->addChild(player, 100);

		player->setOriginPos(player->getPosition());
		player->setParticlePos(player->getPosition());

		CObjectManager::Instance()->setPlayer(player);
		CObjectManager::Instance()->setPlanet(planet);
		CPoolingManager::Instance()->CreateBulletList(1000, 900);
		CPoolingManager::Instance()->CreateShooterList(30, 800);

		EventListenerKeyboard * pListener = EventListenerKeyboard::create();
		pListener->onKeyPressed = [this](EventKeyboard::KeyCode code, Event* pEvent)
		{
			switch (code)
			{
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			{
				m_KeyBoardL = true;
			}break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			{
				m_KeyBoardR = true;
			}break;
			}
		};

		pListener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* pEvent)
		{
			switch (code)
			{
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			{
				m_KeyBoardL = false;
			}break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			{
				m_KeyBoardR = false;
			}break;
			}
		};

		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, this);

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

    
//    auto closeItem = MenuItemImage::create("CloseNormal.png",
//                                           "CloseSelected.png",
//                                           CC_CALLBACK_1(CGameScene::menuCloseCallback, this));
//    
//    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
//                                origin.y + closeItem->getContentSize().height/2));
//    
//    // create menu, it's an autorelease object
//    auto menu = Menu::create(closeItem, NULL);
//    menu->setPosition(Vec2::ZERO);
//	m_GridWorld->addChild(menu, 103);

    
    // RotationObjectLeft callback 등록
	auto leftButton = CMyButton::createWithTexture(
		"leftButton_1.png",
		"leftButton_2.png",
		EXECUTE,
		std::bind(&CObjectManager::RotationObject, CObjectManager::Instance(), -1.f));

	leftButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	leftButton->setPosition(Vec2(origin.x + visibleSize.width * 0.25f,
		origin.x + visibleSize.height * 0.5f));

	m_GridWorld->addChild(leftButton, 102);

	// RotationObjectRight callback 등록
	auto rightButton = CMyButton::createWithTexture(
		"rightButton_1.png",
		"rightButton_2.png",
		EXECUTE,
		std::bind(&CObjectManager::RotationObject, CObjectManager::Instance(), 1.f));

	rightButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	rightButton->setPosition(Vec2(origin.x + visibleSize.width * 0.75f,
		origin.x + visibleSize.height * 0.5f));

	m_GridWorld->addChild(rightButton, 102);

	// player의 HealthCalFunc callback 등록 
	auto healthBar = CHealthBarUI::create(
		std::bind(&CPlayer::HealthCalculatorInNormal, CObjectManager::Instance()->getPlayer(), std::placeholders::_1/*= 호출하는 곳의 인자를 사용한다.*/));

	healthBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	healthBar->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
		origin.x + visibleSize.height * 0.945f));
	m_GridWorld->addChild(healthBar, 102);
	if (!CUIManager::Instance()->AddUIWithName(healthBar, "HealthBar"))
		CCASSERT(false, "HealthBar CAN NOT INIT");

	auto bonusTime = CBonusTimeUI::create();
	bonusTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	bonusTime->setPosition(Vec2(origin.x + visibleSize.width * 0.06f,
		origin.x + visibleSize.height * 0.905f));
	m_GridWorld->addChild(bonusTime, 102);
	if (!CUIManager::Instance()->AddUIWithName(bonusTime, "BonusTime"))
		CCASSERT(false, "BonusTime CAN NOT INIT");

	auto starScoreUI = CScoreUI::create("fonts/Number.ttf", 38, "score.png");
	starScoreUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	starScoreUI->SetLabelAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
	starScoreUI->setPosition(Vec2(origin.x + visibleSize.width * 0.96f,
		origin.x + visibleSize.height * 0.98f));
	m_GridWorld->addChild(starScoreUI, 102);
	if (!CUIManager::Instance()->AddUIWithName(starScoreUI, "StarScoreUI"))
		CCASSERT(false, "StarScoreUI CAN NOT INIT");

	auto multipleScoreUI = CMultipleScore::create();
	m_GridWorld->addChild(multipleScoreUI); // referenceCount를 위하여 addChild
	multipleScoreUI->setVisible(false);
	if (!CUIManager::Instance()->AddUIWithName(multipleScoreUI, "MultipleScoreUI"))
		CCASSERT(false, "MultipleScoreUI CAN NOT INIT");

	auto coinScoreUI = CScoreUI::create("fonts/Number.ttf", 38, "coinIcon_2.png");
	coinScoreUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	coinScoreUI->SetLabelAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
	coinScoreUI->setPosition(Vec2(origin.x + visibleSize.width * 0.1f,
		origin.x + visibleSize.height * 0.98f));
	m_GridWorld->addChild(coinScoreUI, 102);
	if (!CUIManager::Instance()->AddUIWithName(coinScoreUI, "CoinScoreUI"))
		CCASSERT(false, "CoinScoreUI CAN NOT INIT");
    
	auto runScoreUI = CScoreUI::create("fonts/Number.ttf", 38, "run.png");
    runScoreUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    runScoreUI->SetLabelAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
    runScoreUI->setPosition(Vec2(origin.x + visibleSize.width * 0.96f,
        origin.x + visibleSize.height * 0.905f));
	m_GridWorld->addChild(runScoreUI, 102);
    if (!CUIManager::Instance()->AddUIWithName(runScoreUI, "RunScoreUI"))
        CCASSERT(false, "RunScoreUI CAN NOT INIT");

	auto bonusTimeCount = CScoreUI::create("fonts/Number.ttf", 25, "run.png");
	m_GridWorld->addChild(bonusTimeCount, 102);// referenceCount를 위하여 addChild
	bonusTimeCount->setVisible(false);
	if (!CUIManager::Instance()->AddUIWithName(bonusTimeCount, "BonusTimeCount"))
		CCASSERT(false, "BonusTimeCount CAN NOT INIT");

	auto alienGetCount = CScoreUI::create("fonts/Number.ttf", 25, "run.png");
	m_GridWorld->addChild(alienGetCount, 102);// referenceCount를 위하여 addChild
	alienGetCount->setVisible(false);
	if (!CUIManager::Instance()->AddUIWithName(alienGetCount, "AlienGetCount"))
		CCASSERT(false, "AlienGetCount CAN NOT INIT");

	auto challengeClearCount = CScoreUI::create("fonts/Number.ttf", 25, "run.png");
	m_GridWorld->addChild(challengeClearCount, 102);// referenceCount를 위하여 addChild
	challengeClearCount->setVisible(false);
	if (!CUIManager::Instance()->AddUIWithName(challengeClearCount, "ChallengeClearCount"))
		CCASSERT(false, "ChallengeClearCount CAN NOT INIT");

	auto totalScore = CScoreUI::create("fonts/Number.ttf", 25, "run.png");
	m_GridWorld->addChild(totalScore, 102);// referenceCount를 위하여 addChild
	totalScore->setVisible(false);
	if (!CUIManager::Instance()->AddUIWithName(totalScore, "TotalScore"))
		CCASSERT(false, "TotalScore CAN NOT INIT");
    
    auto bestScore = CScoreUI::create("fonts/Number.ttf", 25, "run.png");
    m_GridWorld->addChild(bestScore, 102);// referenceCount를 위하여 addChild
    bestScore->setVisible(false);
    if (!CUIManager::Instance()->AddUIWithName(bestScore, "BestScore"))
        CCASSERT(false, "BestScore CAN NOT INIT");
    
	m_PauseBtn = nullptr;
	m_PauseBtn = CMyButton::create("pauseIcon.png",
		END,
		[this, origin, visibleSize]()
	{
		OpenGamePausePopup();
	}, EFFECT_SIZEDOWN);

	m_PauseBtn->setPosition(Vec2(origin.x + visibleSize.width * 0.08f,
		origin.x + visibleSize.height * 0.05f));
	m_PauseBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_PauseBtn->setCascadeOpacityEnabled(true);
	m_GridWorld->addChild(m_PauseBtn, 102);
    
    
    auto giantModeBtn = CMyButton::create("bonusLetter_0.png",
                                   END,
                                   [this, origin, visibleSize]()
                                   {
                                       CItemManager::Instance()->StartItemTimer(eITEM_TYPE_giant);
                                   }, EFFECT_SIZEDOWN);
    
    giantModeBtn->setPosition(Vec2(origin.x + visibleSize.width * 0.08f,
                                 origin.x + visibleSize.height * 0.5f));
    giantModeBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    giantModeBtn->setCascadeOpacityEnabled(true);
    m_GridWorld->addChild(giantModeBtn, 102);
    
    
    

//	auto gridTest = CMyButton::create("pauseIcon.png",
//		END,
//		[this]()
//	{
//		m_GridWorld->runAction(CSplitCircle::create(10, 9));
//	}, EFFECT_SIZEDOWN);
//
//	gridTest->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
//		origin.x + visibleSize.height * 0.05f));
//	gridTest->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//	gridTest->setCascadeOpacityEnabled(true);
//	m_GridWorld->addChild(gridTest, 102);

	m_CountDownLabel = nullptr;
	m_CountDownLabel = Label::createWithTTF("", "fonts/malgunbd.ttf", 50);
	m_CountDownLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_CountDownLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
		origin.x + visibleSize.height * 0.7f));
	m_GridWorld->addChild(m_CountDownLabel, 102);
	m_CountDownLabel->setVisible(false);
}

void CGameScene::GameExit()
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

	auto btnYes = CMyButton::createWithLayerColor(Size(430, 150),
		Color4B(0, 0, 0, 255 * 0.8f),
		"Yes",
		40,
		Color3B::WHITE,
		END,
		[this](){
            Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
#endif
	}, EFFECT_ALPHA);

	auto btnNo = CMyButton::createWithLayerColor(Size(430, 150),
		Color4B(255, 48, 48, 255 * 0.8f),
		"No",
		40,
		Color3B::WHITE,
		END,
		[this](){
            GameResume();
	}, EFFECT_ALPHA);

	auto popup = CPopup::createWithTwoButton("Are you sure you want to exit StarStarStar?"
		, btnNo, btnYes, 40, Color3B::BLACK);
	popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
		origin.x + visibleSize.height * 0.5f));
	popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(popup, 102);
}

void CGameScene::update(float delta)
{
	
	if (m_KeyBoardL)
		CObjectManager::Instance()->RotationObject(-1.f);
	if (m_KeyBoardR)
		CObjectManager::Instance()->RotationObject(1.f);
	

	CUIManager::Instance()->Execute(delta);
	CObjectManager::Instance()->Execute(delta);
}

void CGameScene::GameStart()
{
    CountDown(3, "GO!", [](){
        CObjectManager::Instance()->getFSM()->ChangeState(CNormalStageState::Instance());
    });
 
	CAudioManager::Instance()->setBGMVolume(1.f);
	CAudioManager::Instance()->setEffectSoundVolume(1.f);

	m_PauseBtn->runAction(FadeIn::create(0.5f));
	CObjectManager::Instance()->getPlayer()->PlayerAlive();
	CObjectManager::Instance()->setIsGamePause(false);
	CObjectManager::Instance()->getFSM()->ChangeState(CGameCountDownState::Instance());
}

void CGameScene::GamePause()
{
	//오디오 소리 작게
	CAudioManager::Instance()->setBGMVolume(0.1f);

    m_PauseBtn->runAction(FadeTo::create(0.5f, 0));
    
	CAudioManager::Instance()->setEffectSoundVolume(0.1f);
	CObjectManager::Instance()->setIsGamePause(true);
	CObjectManager::Instance()->setIsAbleRotation(false);
}

void CGameScene::GameResume()
{
    CountDown(3, "GO!", [](){
        CObjectManager::Instance()->getFSM()->ChangeState(CNormalStageState::Instance());
    });

	//오디오 소리 크게
	CAudioManager::Instance()->setBGMVolume(1.f);

	CAudioManager::Instance()->setEffectSoundVolume(1.f);
	CObjectManager::Instance()->setIsGamePause(false);
    
    m_PauseBtn->runAction(FadeIn::create(0.5f));
}

void CGameScene::GameEnd()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto popup = CPopup::createWithSpecificFormat(CResultPopup::create(), POPUPEFFECT_none);
	popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
		origin.x + visibleSize.height * 0.5f));
	popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_GridWorld->addChild(popup, 102);

	m_PauseBtn->runAction(FadeTo::create(0.5f, 0));
	GamePause();
}

void CGameScene::GameHelp()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto popup = CPopup::createWithSpecificFormat(CHelpPopup::create(), POPUPEFFECT_none);
    popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
                            origin.x + visibleSize.height * 0.5f));
    popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_GridWorld->addChild(popup, 102);
}

void CGameScene::CountDownCancel()
{
    this->unschedule("countDown");
    m_CountDownLabel->setVisible(false);
}

void CGameScene::CountDown(int count, std::string finMent/* = "0"*/, const std::function<void(void)> &func/* = nullptr*/)
{
    CountDownCancel();
	CObjectManager::Instance()->getFSM()->ChangeState(CGameCountDownState::Instance());
    m_Count = count;
	m_CountDownLabel->setVisible(true);
    m_CountDownLabel->setString(StringUtils::format("%d", m_Count));
	this->schedule([this, count, finMent, func](float delta)
	{
        m_Count -= 1;
		if (m_Count > 0)
			m_CountDownLabel->setString(StringUtils::format("%d", m_Count));
        else if (m_Count == 0){
			m_CountDownLabel->setString(finMent.c_str());
        }
		else{
            m_CountDownLabel->setVisible(false);
            if(func != nullptr)
                func();
		}
	}, 1.f, count, 0.f, "countDown");
}

void CGameScene::resetGameScene()
{
	Director::getInstance()->getScheduler()->schedule([](float delta){


		auto tempScene = CEmptyScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.8f, tempScene));

		Director::getInstance()->getScheduler()->schedule([](float delta){
			auto Scene = CGameScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.8f, Scene));
		}, Director::getInstance(), 1.f, 0, 0.f, false, "createGameScene");

	}, Director::getInstance(), 0.f, 0, 0.f, false, "createEmptyScene");

}

void CGameScene::watchVideo()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto popup = CPopup::createWithSpecificFormat(CVideoPopup::create(), POPUPEFFECT_none);
	popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
		origin.x + visibleSize.height * 0.5f));
	popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_GridWorld->addChild(popup, 102);

	m_PauseBtn->runAction(FadeTo::create(0.5f, 0));
	GamePause();
}

void CGameScene::OpenGamePausePopup()
{
    // 이미 Pause인 상태면 리턴한다.
    if(CObjectManager::Instance()->getPlayer()->getIsDead()
    || CObjectManager::Instance()->getIsGamePause())
        return;
    
    GamePause();
 
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto popup = CPopup::createWithSpecificFormat(CPausePopup::create(), POPUPEFFECT_none);
    popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
                            origin.x + visibleSize.height * 0.5f));
    popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_GridWorld->addChild(popup, 102);
}

void CGameScene::backToMenuScene()
{
	CAudioManager::Instance()->AllPause();

    Director::getInstance()->getScheduler()->schedule([](float delta){
        
        auto tempScene = CEmptyScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.8f, tempScene));
        
        Director::getInstance()->getScheduler()->schedule([](float delta){
            auto Scene = CMenuScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.8f, Scene));
        }, Director::getInstance(), 1.f, 0, 0.f, false, "createMenuScene");
        
    }, Director::getInstance(), 0.f, 0, 0.f, false, "createEmptyScene");
}
