#include "GameScene.h"
#include "EmptyScene.h"
#include "MenuScene.h"
#include "SceneTransition.h"
#include "../Task/PoolingManager.h"
#include "../GameObject/Planet.h"
#include "../GameObject/Player.h"
#include "../GameObject/ObjectManager.h"
#include "../GameObject/ItemManager.h"
#include "../GameObject/Bullet/Bullet.h"
#include "../GameObject/BackGround.h"
#include "../GameObject/BulletCreator.h"
#include "../GameObject/Rocket.h"
#include "../GameObject/ItemRange.h"
#include "../MyUI/MyButton.h"
#include "../MyUI/MenuLayer.hpp"
#include "../MyUI/UILayer.hpp"
#include "../MyUI/BonusTimeLayer.hpp"
#include "../MyUI/MultipleScore.h"
#include "../MyUI/CountDown.hpp"
#include "../MyUI/Popup.h"
#include "../MyUI/Popup/PausePopup.h"
#include "../MyUI/Popup/ResultPopup.h"
#include "../MyUI/Popup/VideoPopup.h"
#include "../MyUI/Popup/ChallengePopup.h"
#include "../MyUI/Popup/HelpPopup.h"
#include "../MyUI/Popup/RewardPopup.h"
#include "../MyUI/Popup/Option/OptionPopup.hpp"
#include "../MyUI/Popup/WorkshopPopup.h"
#include "../MyUI/Popup/CharacterSelectPopup.h"
#include "../MyUI/Popup/GameEndPopup.hpp"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/CharacterDataManager.h"
#include "../DataManager/ChallengeDataManager.hpp"
#include "../AI/States/RocketStates.h"
#include "../SDKUtil/SDKUtil.h"
#include <array>

USING_NS_CC;

CGameScene* CGameScene::m_GameScene = nullptr;
cocos2d::NodeGrid* CGameScene::m_GridWorld = nullptr;

Scene* CGameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
#if CC_USE_PHYSICS
    scene->initWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0, -98 * 40));

#endif
	// 'layer' is an autorelease object
	auto layer = CGameScene::create();
	// add layer as a child to scene
	scene->setAnchorPoint(Vec2(0, 0));
	layer->setAnchorPoint(Vec2(0, 0));
	scene->addChild(layer);

	return scene;
}

CGameScene::CGameScene()
: m_UILayer(nullptr)
, m_MenuLayer(nullptr)
, m_BonusTimeLayer(nullptr)
, m_ScreenFade(nullptr)
, m_CountDown(nullptr)
, m_KeyBoardSpace(false){}

CGameScene::~CGameScene()
{
	CObjectManager::Instance()->RemoveAllObject();
	removeAllChildrenWithCleanup(true); // 이부분 검토
	CPoolingManager::Instance()->DeleteAllMemory();
    CAudioManager::Instance()->Clear();
}

void CGameScene::update(float delta)
{
	if (m_KeyBoardSpace) CObjectManager::Instance()->RotationObject(-2.f);

	CObjectManager::Instance()->Execute(delta);
}

bool CGameScene::init()
{
	if (!Layer::init()) return false;

	m_GameScene = this;
	m_VisibleSize = Director::getInstance()->getVisibleSize();
	m_TouchPos = m_VisibleSize / 2;
	this->scheduleUpdate();
	
	m_GridWorld = NodeGrid::create();
	this->addChild(m_GridWorld, 0, 1);
    
    this->initMemoryPool();
    this->createBulletCreator();
    this->createBackground();
    this->createPlanet();
    this->createPlayer();
    this->createRocket();
    this->createCountDown();
    this->createScreenFade();
    this->createItemRanges();
    this->createComboUI();
    this->createMenuLayer();
    this->createUILayer();
    this->initKeyboardListener();
    this->OpenGameMenuLayer();
    
	return true;
}

void CGameScene::GameExit(bool resume/* = false*/)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif
	this->GamePause();
	this->createExitPopup(resume);
}

void CGameScene::GameStart()
{
    //    this->ScreenFade([=](){
    this->clearData();
    this->GameResume();
    m_UILayer->setVisible(true);
    m_UILayer->setDefaultCallbackToTopAgain();
    m_MenuLayer->setVisible(false);
    CObjectManager::Instance()->ZoomOut();
    CObjectManager::Instance()->getPlayer()->GameStart();
    //        CAudioManager::Instance()->PlayBGM("sounds/bgm_1.mp3", true);

    //    });
}

void CGameScene::GameResume()
{
    m_CountDown->Reset();
    this->turnUpSound();
}

void CGameScene::GamePause()
{
    CObjectManager::Instance()->setIsGamePause(true);
    this->turnDownSound();
}

void CGameScene::GameResult()
{
	this->createResultPopup();
    this->GamePause();
}

void CGameScene::GameEnd()
{
    this->createEndPopup();
//    this->GamePause();
}

void CGameScene::GameHelp()
{
	this->createHelpPopup();
}

void CGameScene::WatchVideo()
{
	this->createVideoPopup();
	this->GamePause();
}

void CGameScene::ShowChallenge()
{
	this->createChallengePopup();
	this->GamePause();
}

void CGameScene::OpenGamePausePopup()
{
    // 이미 Pause인 상태면 리턴한다.
    if(CObjectManager::Instance()->getIsGamePause())
        return;
    
    this->GamePause();
	this->createPausePopup();
}

void CGameScene::OpenGameMenuLayer()
{
    this->ScreenFade([=](){
        CObjectManager::Instance()->ZoomIn();
        CObjectManager::Instance()->getRocket()->ComebackHome();
		CObjectManager::Instance()->getRocket()->Gift();
        this->clearData();
        this->createRandomCoin();
        m_UILayer->setVisible(false);
        m_MenuLayer->setVisible(true);
        m_MenuLayer->setDefaultCallbackToTopAgain();
    });
}

void CGameScene::OpenOptionPopup(int scrollIndex/* = 0*/)
{
    this->createOptionPopup(scrollIndex);
}

void CGameScene::OpenWorkshopPopup()
{
    this->createWorkshopPopup();
}

void CGameScene::OpenCharacterSelectPopup()
{
    CObjectManager::Instance()->ZoomIn2();
    m_MenuLayer->setVisible(false);
    this->createCharacterSelectPopup();
}

void CGameScene::RandomCoin()
{
    this->createRandomCoin();
}

void CGameScene::ScreenFade(const FADE_CALLBACK& callback/* = nullptr*/)
{
    auto fadeOut = FadeTo::create(0.5f, 1);
    auto fadeIn  = FadeIn::create(0.5f);
    auto callFunc = CallFunc::create([=](){
        callback();
    });
    auto delayAction = DelayTime::create(0.3f);
    auto visibleOff = CallFunc::create([=](){ m_ScreenFade->setVisible(false); });
    auto visibleOn  = CallFunc::create([=](){ m_ScreenFade->setVisible(true);  });

    
    m_ScreenFade->runAction(Sequence::create(visibleOn,
                                             fadeIn,
                                             callFunc,
											 delayAction,
											 fadeOut,
                                             visibleOff,
                                             nullptr));
}

CPopup* CGameScene::CreateAlertPopup()
{
	return CPopup::create()
		->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
		->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
		->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setPopupPosition(m_VisibleSize / 2);
}

void CGameScene::BonusTimeStart()
{
    this->createBonusTimeLayer();
}

void CGameScene::BonusTimeEnd()
{
    this->removeBonusTimeLayer();
}

CPopup* CGameScene::Reward()
{
	return CRewardPopup::create()
		->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setPopupPosition(m_VisibleSize / 2)
		->show(this, ZORDER::POPUP);
}

void CGameScene::clearData()
{
    CAudioManager::Instance()->Clear();
    CObjectManager::Instance()->Clear();
    CItemManager::Instance()->Clear();
    this->cleanGlobalData();
	this->removeBonusTimeLayer();
}

void CGameScene::cleanGlobalData()
{
    GLOBAL->Clear();
}

void CGameScene::createPausePopup()
{
	CPausePopup::create()
		->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setPopupPosition(m_VisibleSize / 2)
        ->show(this, ZORDER::POPUP);
}

void CGameScene::createVideoPopup()
{
	CVideoPopup::create()
		->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setPopupPosition(m_VisibleSize / 2)
		->show(this, ZORDER::POPUP);
}

void CGameScene::createChallengePopup()
{
	CChallengePopup::create()
		->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setPopupPosition(m_VisibleSize / 2)
		->show(this, ZORDER::POPUP);
}

void CGameScene::createResultPopup()
{
	CResultPopup::create()
		->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setPopupPosition(m_VisibleSize / 2)
		->show(this, ZORDER::POPUP);
}

void CGameScene::createEndPopup()
{
    CGameEndPopup::create()
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(this, ZORDER::POPUP);
}

void CGameScene::createHelpPopup()
{
	CHelpPopup::create()
		->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setPopupPosition(m_VisibleSize / 2)
		->show(this, ZORDER::POPUP);
}

void CGameScene::createExitPopup(bool resume)
{
	CSDKUtil::Instance()->AutoSave();

	this->CreateAlertPopup()
		->setPositiveButton([=](Node* sender){
		Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
	}, TRANSLATE("BUTTON_YES"))
		->setNegativeButton([=](Node* sender){
		if (resume) this->GameResume();
	}, TRANSLATE("BUTTON_NO"))
		->setDefaultCallback([=](Node* sender){
		if (resume) this->GameResume();

		auto popup = dynamic_cast<CPopup*>(sender);
		popup->popupClose();
	})
		->setMessage(TRANSLATE("GAME_EXIT_CHECK"))
		->show(this, ZORDER::POPUP);
}

void CGameScene::createOptionPopup(int index)
{
    COptionPopup::create()
    ->setInitialScrollIndex(index)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(this, ZORDER::POPUP);
}

void CGameScene::createBonusTimeLayer()
{
    m_BonusTimeLayer = CBonusTimeLayer::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(this, ZORDER::POPUP);
}

void CGameScene::createWorkshopPopup()
{
    CWorkshopPopup::create()
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(this, ZORDER::POPUP);
}


void CGameScene::createCharacterSelectPopup()
{
    CCharacterSelectPopup::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(this, ZORDER::POPUP);
}

void CGameScene::removeBonusTimeLayer()
{
    if(m_BonusTimeLayer) {
        m_BonusTimeLayer->popupClose();
        m_BonusTimeLayer = nullptr;
    }
}

void CGameScene::turnDownSound()
{
    auto userBGMVolume      = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BGM_VOLUME);
    auto userEffectVolume   = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::EFFECT_VOLUME);
    
    CAudioManager::Instance()->setBGMVolume((userBGMVolume / 100.f) * 0.1f);
    CAudioManager::Instance()->setEffectSoundVolume((userEffectVolume / 100.f) * 0.1f);
}

void CGameScene::turnUpSound()
{
    auto userBGMVolume      = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BGM_VOLUME);
    auto userEffectVolume   = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::EFFECT_VOLUME);
    
    CAudioManager::Instance()->setBGMVolume((userBGMVolume / 100.f));
    CAudioManager::Instance()->setEffectSoundVolume((userEffectVolume / 100.f));
}

void CGameScene::initKeyboardListener()
{
	EventListenerKeyboard * pListener = EventListenerKeyboard::create();
	pListener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* pEvent)
	{
        if(code == EventKeyboard::KeyCode::KEY_SPACE) m_KeyBoardSpace = true;
	};

	pListener->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* pEvent)
	{
        if(code == EventKeyboard::KeyCode::KEY_SPACE) m_KeyBoardSpace = false;
        if(code == EventKeyboard::KeyCode::KEY_BACK)  CPopup::DefaultCallback();
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, this);
}

void CGameScene::createRandomCoin()
{
    auto data = CBulletPatternDataManager::Instance()->getRandomConstellationPatternByLevel(1, true);
    CBulletCreator::CreateConstellation(data);
}


// The following items are initialized only once.
void CGameScene::createBulletCreator()
{
    auto bulletCreator = CBulletCreator::create();
    this->addChild(bulletCreator);
    CObjectManager::Instance()->setBulletCreator(bulletCreator);
}

void CGameScene::createBackground()
{
    auto background = CBackGround::create();
    this->addChild(background, ZORDER::BACKGROUND);
    CObjectManager::Instance()->setBackground(background);
}

void CGameScene::createPlanet()
{
    CObjectManager::Instance()->ChangeCharacter();

    auto planet = CPlanet::create();
    planet->setPosition(PLANET_DEFINE::ZOOMOUT_POS);
    planet->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(planet, ZORDER::PLANET);
    CObjectManager::Instance()->setPlanet(planet);
}

void CGameScene::createPlayer()
{
    auto player = CPlayer::create();
    player->setPosition(PLAYER_DEFINE::ZOOMOUT_POS);
    player->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(player, ZORDER::PLAYER);
    CObjectManager::Instance()->setPlayer(player);
}

void CGameScene::createRocket()
{
    auto rocket = CRocket::create(sROCKET_PARAM());
    rocket->setSpeed(ROCKET_DEFINE::SPEED);
    rocket->setDistance(ROCKET_DEFINE::FLYAROUND_DISTANCE);
    rocket->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rocket->setPosition(CBullet::getSquarePosition(random<int>(0, 360), ROCKET_DEFINE::FLYAWAY_DISTANCE));
    rocket->setTargetPos(CBullet::getSquarePosition(random<int>(0, 360), rocket->getDistance()));
    rocket->ChangeState(CFlyToTouchArea::Instance());
    this->addChild(rocket, ZORDER::PLAYER);
    CObjectManager::Instance()->setRocket(rocket);
}

void CGameScene::createCountDown()
{
    m_CountDown = CCountDown::create()
    ->addLastEventListner([=](Node* sender){
        CObjectManager::Instance()->setIsGamePause(false);
        CObjectManager::Instance()->SpeedControl(0.5f, BULLETCREATOR::ROTATION_SPEED);
    })
    ->setFont(Color4B::WHITE, 65)
    ->setMaxNumber(3)
    ->setMinNumber(0)
    ->setLastContent(TRANSLATE("COUNTDOWN_LAST"))
    ->setInterval(0.8f)
    ->setLabelPosition(Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * 0.6f))
    ->setLabelAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(this, ZORDER::POPUP);
    m_CountDown->Pause();
    
    CTranslateManager::Instance()->addTranslateEventListener([=](){
        m_CountDown->setLastContent(TRANSLATE("COUNTDOWN_LAST"));
    });
}

void CGameScene::createScreenFade()
{
    m_ScreenFade = LayerColor::create(Color4B::BLACK, m_VisibleSize.width, m_VisibleSize.height);
    m_ScreenFade->setIgnoreAnchorPointForPosition(false);
    m_ScreenFade->setPosition(m_VisibleSize / 2);
    m_ScreenFade->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(m_ScreenFade, ZORDER::SCREENFADE);
}

void CGameScene::createItemRanges()
{
    auto createRange = [=](std::string textureName){
        auto range = CItemRange::create()
        ->setTextureName(textureName)
        ->show(this, ZORDER::PLAYER);
        range->setPosition(PLAYER_DEFINE::ZOOMOUT_POS);
        range->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        return range;
    };
    
    CObjectManager::Instance()->setBarrierItemRange(createRange("barrier2.png"));
    CObjectManager::Instance()->setStarItemRange(createRange("barrier3.png"));
    CObjectManager::Instance()->setCoinItemRange(createRange("barrier3.png"));
}

void CGameScene::createComboUI()
{
    auto multiscore = CMultipleScore::Instance();
    this->addChild(multiscore, ZORDER::BACKGROUND);
}

void CGameScene::createMenuLayer()
{
    m_MenuLayer = CMenuLayer::Instance()
    ->setBackgroundVisible(false)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(this, ZORDER::POPUP);
}

void CGameScene::createUILayer()
{
    m_UILayer = CUILayer::Instance()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(this, ZORDER::POPUP);
}

void CGameScene::initMemoryPool()
{
#if(USE_MEMORY_POOLING)
    CPoolingManager::Instance()->CreateBulletList(600, 900);
#endif
}
