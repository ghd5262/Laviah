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
#include "../MyUI/MyButton.h"
#include "../MyUI/MenuLayer.hpp"
#include "../MyUI/UILayer.hpp"
#include "../MyUI/MultipleScore.h"
#include "../MyUI/CountDown.hpp"
#include "../MyUI/Popup.h"
#include "../MyUI/Popup/PausePopup.h"
#include "../MyUI/Popup/ResultPopup.h"
#include "../MyUI/Popup/VideoPopup.h"
#include "../MyUI/Popup/HelpPopup.h"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/CharacterDataManager.h"

#include <array>

USING_NS_CC;
using namespace GLOBAL;

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
, m_KeyBoardSpace(false){
    std::array<std::string, 7> keyArray = {
        BONUSTIME,
        ALIENGET,
        CHALLENGECLEAR,
        TOTALSCORE,
        STARSCORE,
        COINSCORE,
        RUNSCORE
    };
    
    for(auto var : keyArray)
    {
        m_GlobalVariable.emplace(std::pair<std::string, int>(var, 0));
    }
}

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

	this->scheduleUpdate();
	//CAudioManager::Instance()->PlayBGM("sounds/bgm_1.mp3", true);

	m_GridWorld = NodeGrid::create();
	this->addChild(m_GridWorld, 0, 1);

	auto bulletCreator = CBulletCreator::create();
	this->addChild(bulletCreator);
    CObjectManager::Instance()->setBulletCreator(bulletCreator);
    

	auto background = CBackGround::create();
    this->addChild(background, ZORDER::BACKGROUND);
    CObjectManager::Instance()->setBackground(background);
    

	int currentCharacterIdx = CUserDataManager::Instance()->getUserData_Number("USER_CUR_CHARACTER");
	sCHARACTER_PARAM currentCharacterInfo = CCharacterDataManager::Instance()->getCharacterInfoByIndex(currentCharacterIdx);
	CCharacterDataManager::Instance()->PrintCharacterInfo(currentCharacterInfo._idx);

	auto planet = CPlanet::create(currentCharacterInfo._planetTextureName);
	planet->setPosition(Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * 0.35f));
	planet->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	planet->setOriginPos(planet->getPosition());
    this->addChild(planet, ZORDER::PLANET);
    CObjectManager::Instance()->setPlanet(planet);
    

	auto player = CPlayer::create(currentCharacterInfo);
	player->setRotateSpeed(((planet->getContentSize().width / player->getContentSize().width) * BULLETCREATOR::ROTATION_SPEED));
	player->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	player->setPosition(Vec2(m_VisibleSize.width * 0.5f,
                             planet->getPosition().y + (planet->getBoundingRadius() + 20)));
	player->setOriginPos(player->getPosition());
	player->setParticlePos(player->getPosition());
    this->addChild(player, ZORDER::PLAYER);
    CObjectManager::Instance()->setPlayer(player);
    

    auto multipleScoreUI = CMultipleScore::Instance();
    this->addChild(multipleScoreUI, ZORDER::PLAYER);
    

#if(USE_MEMORY_POOLING)
	CPoolingManager::Instance()->CreateBulletList(500, 900);
#endif
	this->initKeyboardListener();
    this->OpenGameMenuLayer();
    
	return true;
}

void CGameScene::GameExit()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif
	this->GamePause();
	this->createExitPopup();
}

void CGameScene::GameStart()
{
    this->clearData();
    this->createUILayer();
	CObjectManager::Instance()->getPlayer()->PlayerAlive();
}

void CGameScene::GameResume()
{
    CCountDown::create()
    ->addLastEventListner([=](Node* sender){
        CObjectManager::Instance()->setIsGamePause(false);
    })
    ->setFont(Color4B::WHITE, 50)
    ->setMaxNumber(3)
    ->setMinNumber(0)
    ->setLastContent("GO!")
    ->setInterval(0.8f)
    ->setCleanUpAtTheLast(true)
    ->setLabelPosition(Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * 0.65f))
    ->setLabelAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(this);
    
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
    this->clearData();
    this->createMenuLayer();
}

void CGameScene::clearData()
{
    CObjectManager::Instance()->Clear();
    CItemManager::Instance()->Clear();
    this->cleanGlobalData();
    this->removeUILayer();
}

void CGameScene::cleanGlobalData()
{
    std::array<std::string, 7> keyArray = {
        BONUSTIME,
        ALIENGET,
        CHALLENGECLEAR,
        TOTALSCORE,
        STARSCORE,
        COINSCORE,
        RUNSCORE
    };
    
    for(auto var : keyArray)
    {
        this->setGlobalValue(var, 0);
    }
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

void CGameScene::createExitPopup()
{
    CPopup::create()
    ->setPositiveButton([=](Node* sender){
        Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    }, "Yes")
    ->setNegativeButton([=](Node* sender){
        this->GameResume();
    }, "No")
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
    m_UILayer = CUILayer::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(this, ZORDER::POPUP);
}

void CGameScene::removeUILayer()
{
    if(m_UILayer) {
        m_UILayer->popupClose();
        m_UILayer = nullptr;
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
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, this);
}

int  CGameScene::getGlobalValue(std::string key)
{
    auto result = m_GlobalVariable.find(key);
    if(result != m_GlobalVariable.end())
        return result->second;
    return 0;
}

void CGameScene::setGlobalValue(std::string key, int value)
{
    auto result = m_GlobalVariable.find(key);
    if(result != m_GlobalVariable.end())
        result->second = value;
}

void CGameScene::addGlobalValue(std::string key, int value)
{
    auto result = m_GlobalVariable.find(key);
    if(result != m_GlobalVariable.end())
        result->second += value;
}

//void CGameScene::ResetGameScene()
//{
//	Director::getInstance()->getScheduler()->schedule([](float delta){
//
//		auto tempScene = CEmptyScene::createScene();
//		Director::getInstance()->replaceScene(TransitionFade::create(0.8f, tempScene));
//
//		Director::getInstance()->getScheduler()->schedule([](float delta){
//			auto Scene = CGameScene::createScene();
//			Director::getInstance()->replaceScene(TransitionFade::create(0.8f, Scene));
//		}, Director::getInstance(), 1.f, 0, 0.f, false, "createGameScene");
//
//	}, Director::getInstance(), 0.f, 0, 0.f, false, "createEmptyScene");
//}

//void CGameScene::BackToMenuScene()
//{
//	CAudioManager::Instance()->AllPause();
//    Director::getInstance()->getScheduler()->schedule([](float delta){
//
//        auto tempScene = CEmptyScene::createScene();
//        Director::getInstance()->replaceScene(TransitionFade::create(0.8f, tempScene));
//
//        Director::getInstance()->getScheduler()->schedule([](float delta){
//            auto Scene = CMenuScene::createScene();
//            Director::getInstance()->replaceScene(TransitionFade::create(0.8f, Scene));
//        }, Director::getInstance(), 1.f, 0, 0.f, false, "createMenuScene");
//
//    }, Director::getInstance(), 0.f, 0, 0.f, false, "createEmptyScene");
//}
