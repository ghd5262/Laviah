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
	
//CAudioManager::Instance()->PlayBGM("sounds/bgm_1.mp3", true);
	m_GridWorld = NodeGrid::create();
	this->addChild(m_GridWorld, 0, 1);
    
    m_ScreenFade = LayerColor::create(Color4B::BLACK, m_VisibleSize.width, m_VisibleSize.height);
    m_ScreenFade->setIgnoreAnchorPointForPosition(false);
    m_ScreenFade->setPosition(m_VisibleSize / 2);
    m_ScreenFade->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(m_ScreenFade, ZORDER::SCREENFADE);

	auto bulletCreator = CBulletCreator::create();
	this->addChild(bulletCreator);
    CObjectManager::Instance()->setBulletCreator(bulletCreator);
    
	auto background = CBackGround::create();
    this->addChild(background, ZORDER::BACKGROUND);
    CObjectManager::Instance()->setBackground(background);
    
	int currentCharacterIdx = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::CHARACTER);
	auto currentCharacterInfo = CCharacterDataManager::Instance()->getCharacterByIndex(currentCharacterIdx);
	CCharacterDataManager::Instance()->PrintCharacterInfo(currentCharacterInfo->_idx);
	CObjectManager::Instance()->setCharacterParam(currentCharacterInfo);

	auto planet = CPlanet::create();
	planet->setPosition(Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * 0.35f));
	planet->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	planet->setOriginPos(planet->getPosition());
    this->addChild(planet, ZORDER::PLANET);
    CObjectManager::Instance()->setPlanet(planet);

	auto player = CPlayer::create();
	player->setRotateSpeed(((planet->getContentSize().width / player->getContentSize().width) * BULLETCREATOR::ROTATION_SPEED));
	player->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	player->setPosition(Vec2(m_VisibleSize.width * 0.5f,
                             planet->getPosition().y + (planet->getBoundingRadius() + 20)));
	player->setOriginPos(player->getPosition());
	player->setParticlePos(player->getPosition());
    this->addChild(player, ZORDER::PLAYER);
    CObjectManager::Instance()->setPlayer(player);

	auto createRange = [=](std::string textureName){
		auto range = CItemRange::create()
			->setTextureName(textureName)
			->show(this);
		range->setPosition(player->getPosition());
		range->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		return range;
	};

	CObjectManager::Instance()->setBarrierItemRange(createRange("barrier2.png"));
	CObjectManager::Instance()->setStarItemRange(createRange("barrier3.png"));
	CObjectManager::Instance()->setCoinItemRange(createRange("barrier3.png"));

    auto rocket = CRocket::create(sROCKET_PARAM());
    rocket->setSpeed(ROCKET::SPEED);
    rocket->setDistance(ROCKET::FLYAROUND_DISTANCE);
	rocket->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	rocket->setPosition(CBullet::getCirclePosition(random<int>(0, 360), ROCKET::FLYAWAY_DISTANCE, planet->getPosition()));
   	rocket->setTargetPos(CBullet::getCirclePosition(random<int>(0, 360), rocket->getDistance(), planet->getPosition()));
	rocket->ChangeState(CFlyToTouchArea::Instance());
	this->addChild(rocket, ZORDER::PLAYER);
	CObjectManager::Instance()->setRocket(rocket);

    auto multipleScoreUI = CMultipleScore::Instance();
    this->addChild(multipleScoreUI, ZORDER::PLAYER);
    
    m_CountDown = CCountDown::create()
    ->addLastEventListner([=](Node* sender){
        CObjectManager::Instance()->setIsGamePause(false);
    })
    ->setFont(Color4B::WHITE, 60)
    ->setMaxNumber(3)
    ->setMinNumber(0)
    ->setLastContent("GO!")
    ->setInterval(0.8f)
    ->setLabelPosition(Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * 0.65f))
    ->setLabelAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(this);
    m_CountDown->Pause();

#if(USE_MEMORY_POOLING)
	CPoolingManager::Instance()->CreateBulletList(600, 900);
#endif
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
    this->ScreenFade([=](){
        this->clearData();
        this->GameResume();
		m_UILayer->setVisible(true);
//        this->createUILayer();
        CObjectManager::Instance()->getPlayer()->PlayerAlive();
    });
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
        auto centerPos = Director::getInstance()->getVisibleSize() / 2;
        auto rocket = CObjectManager::Instance()->getRocket();
        rocket->setTargetPos(CBullet::getCirclePosition(random<int>(0, 360), rocket->getDistance(), centerPos));
        rocket->ChangeState(CFlyToTouchArea::Instance());
        
        this->clearData();
        this->createMenuLayer();
        this->createRandomCoin();
    });
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
    CObjectManager::Instance()->Clear();
    CItemManager::Instance()->Clear();
    this->cleanGlobalData();
//    this->removeUILayer();
	this->removeBonusTimeLayer();
	m_UILayer->setVisible(false);
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

    CPopup::create()
    ->setPositiveButton([=](Node* sender){
        Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    }, "Yes")
    ->setNegativeButton([=](Node* sender){
        if(resume) this->GameResume();
    }, "No")
    ->setDefaultCallback([=](Node* sender){
        if(resume) this->GameResume();
        
		auto popup = dynamic_cast<CPopup*>(sender);
		popup->popupClose();
    })
    ->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setMessage("Are you sure you want to exit StarStarStar?")
    ->setMessageFont(Color3B::BLACK, 40)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(this, ZORDER::POPUP);
}

void CGameScene::createMenuLayer()
{
    CMenuLayer::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
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

void CGameScene::createBonusTimeLayer()
{
    m_BonusTimeLayer = CBonusTimeLayer::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(this, ZORDER::POPUP);
}

//void CGameScene::removeUILayer()
//{
//    if(m_UILayer) {
//        m_UILayer->popupClose();
//        m_UILayer = nullptr;
//    }
//}

void CGameScene::removeBonusTimeLayer()
{
    if(m_BonusTimeLayer) {
        m_BonusTimeLayer->popupClose();
        m_BonusTimeLayer = nullptr;
    }
}

void CGameScene::turnDownSound()
{
    CAudioManager::Instance()->setBGMVolume(0.1f);
    CAudioManager::Instance()->setEffectSoundVolume(0.1f);
}

void CGameScene::turnUpSound()
{
    CAudioManager::Instance()->setBGMVolume(1.f);
    CAudioManager::Instance()->setEffectSoundVolume(1.f);
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
