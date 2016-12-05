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
#include "../MyUI/UIManager.h"
#include "../MyUI/MyButton.h"
#include "../MyUI/Popup.h"
#include "../MyUI/Popup/PausePopup.h"
#include "../MyUI/Popup/ResultPopup.h"
#include "../MyUI/Popup/VideoPopup.h"
#include "../MyUI/Popup/HelpPopup.h"
//#include "../MyUI/Popup/MenuPopup.h"
//#include "../MyUI/Popup/GameSceneUI.h"
#include "../AI/States/StageStates.h"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/CharacterDataManager.h"

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
	: m_CountDownLabel(nullptr)
	, m_KeyBoardSpace(false)
	, m_Count(0)
{}

CGameScene::~CGameScene()
{
	CObjectManager::Instance()->RemoveAllObject();
	removeAllChildrenWithCleanup(true); // 이부분 검토
	CPoolingManager::Instance()->DeleteAllMemory();
	this->clearData();
}

void CGameScene::update(float delta)
{
	if (m_KeyBoardSpace) CObjectManager::Instance()->RotationObject(-2.f);

	CUIManager::Instance()->Execute(delta);
	CObjectManager::Instance()->Execute(delta);
}

bool CGameScene::init()
{
	if (!Layer::init()) return false;
	
	m_GameScene = this;
	m_VisibleSize = Director::getInstance()->getVisibleSize();

	this->scheduleUpdate();
	this->clearData();
	//CAudioManager::Instance()->PlayBGM("sounds/bgm_1.mp3", true);

	m_GridWorld = NodeGrid::create();
	this->addChild(m_GridWorld, 0, 1);

	auto bulletCreator = CBulletCreator::create();
	this->addChild(bulletCreator);

	auto background = CBackGround::create();
	this->addChild(background, -1);

	int currentCharacterIdx = CUserDataManager::Instance()->getUserData_Number("USER_CUR_CHARACTER");
	sCHARACTER_PARAM currentCharacterInfo = CCharacterDataManager::Instance()->getCharacterInfoByIndex(currentCharacterIdx);
	CCharacterDataManager::Instance()->PrintCharacterInfo(currentCharacterInfo._idx);

	auto planet = CPlanet::create(currentCharacterInfo._planetTextureName);
	planet->setPosition(Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * 0.35f));
	planet->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	planet->setOriginPos(planet->getPosition());
	this->addChild(planet);

	auto player = CPlayer::create(currentCharacterInfo);
	player->setRotateSpeed(((planet->getContentSize().width / player->getContentSize().width) * BULLETCREATOR::ROTATION_SPEED));
	player->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	player->setPosition(Vec2(m_VisibleSize.width * 0.5f, planet->getPosition().y + (planet->getBoundingRadius() + 20)));
	player->setOriginPos(player->getPosition());
	player->setParticlePos(player->getPosition());
	this->addChild(player);

	CObjectManager::Instance()->setBackground(background);
	CObjectManager::Instance()->setPlayer(player);
	CObjectManager::Instance()->setPlanet(planet);
	CObjectManager::Instance()->setBulletCreator(bulletCreator);

#if(USE_MEMORY_POOLING)
	CPoolingManager::Instance()->CreateBulletList(500, 900);
#endif

//	this->initGameSceneUI();
	this->initKeyboardListener();
	//this->createTestItemButton();

	return true;
}

//void CGameScene::initGameSceneUI()
//{
//	m_CountDownLabel = Label::createWithTTF("", "fonts/malgunbd.ttf", 50);
//	m_CountDownLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//	m_CountDownLabel->setPosition(Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * 0.7f));
//	m_CountDownLabel->setVisible(false);
//	this->addChild(m_CountDownLabel);

//	CMenuPopup::create()
//		->setPopupPosition(m_VisibleSize / 2)
//		->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
//		->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
//		->show(this);

	// player의 HealthCalFunc callback 등록 
	//auto healthBar = CHealthBarUI::create(
	//	std::bind(&CPlayer::HealthCalculatorInNormal, CObjectManager::Instance()->getPlayer(), std::placeholders::_1/*= 호출하는 곳의 인자를 사용한다.*/));
	//healthBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//healthBar->setPosition(Vec2(origin.x + m_VisibleSize.width * 0.5f,
	//	origin.x + m_VisibleSize.height * 0.945f));
	//this->addChild(healthBar);
	//if (!CUIManager::Instance()->AddUIWithName(healthBar, "HealthBar"))
	//	CCASSERT(false, "HealthBar CAN NOT INIT");
	//	auto gridTest = CMyButton::create("pauseIcon.png",
	//		END,
	//		[this]()
	//	{
	//		this->runAction(CSplitCircle::create(10, 9));
	//	}, EFFECT_SIZEDOWN);
	//
	//	gridTest->setPosition(Vec2(origin.x + m_VisibleSize.width * 0.5f,
	//		origin.x + m_VisibleSize.height * 0.05f));
	//	gridTest->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//	gridTest->setCascadeOpacityEnabled(true);
	//	this->addChild(gridTest);
//}

void CGameScene::GameExit()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif
//	this->gamePause();
	this->createExitPopup();
}

void CGameScene::GameStart()
{
	this->GameResume();
	CObjectManager::Instance()->getFSM()->ChangeState(CGameCountDownState::Instance());
	CObjectManager::Instance()->getPlayer()->PlayerAlive();
}

void CGameScene::GameResume()
{
//    this->CountDown(3, "GO!", [](){
//        CObjectManager::Instance()->getFSM()->ChangeState(CNormalStageState::Instance());
//    });

	CObjectManager::Instance()->setIsGamePause(false);
    this->turnUpSound();
}

void CGameScene::GamePause()
{
    auto objectMng = CObjectManager::Instance();
    if(objectMng->getPlayer()->getIsDead() || objectMng->getIsGamePause()) return;

    CObjectManager::Instance()->setIsGamePause(true);
    this->createPausePopup();
    this->turnDownSound();
}

void CGameScene::ShowResult()
{
//	this->gamePause();
    
	this->createResultPopup();
//	this->offPauseButton();
}

void CGameScene::GameHelp()
{
	this->createHelpPopup();
}

//void CGameScene::CountDownCancel()
//{
//    this->unschedule("countDown");
//    m_CountDownLabel->setVisible(false);
//}

//void CGameScene::CountDown(int count, std::string finMent/* = "0"*/, const std::function<void(void)> &func/* = nullptr*/)
//{
//    this->CountDownCancel();
//	CObjectManager::Instance()->getFSM()->ChangeState(CGameCountDownState::Instance());
//    m_Count = count;
//	m_CountDownLabel->setVisible(true);
//    m_CountDownLabel->setString(StringUtils::format("%d", m_Count));
//	this->schedule([=](float delta)
//	{
//        m_Count -= 1;
//		if (m_Count > 0)
//			m_CountDownLabel->setString(StringUtils::format("%d", m_Count));
//        else if (m_Count == 0){
//			m_CountDownLabel->setString(finMent.c_str());
//        }
//		else{
//            m_CountDownLabel->setVisible(false);
//            if(func != nullptr)
//                func();
//		}
//	}, 1.f, count, 0.f, "countDown");
//}

void CGameScene::ResetGameScene()
{
	//Director::getInstance()->getScheduler()->schedule([](float delta){
	//	
	//	auto tempScene = CEmptyScene::createScene();
	//	Director::getInstance()->replaceScene(TransitionFade::create(0.8f, tempScene));

	//	Director::getInstance()->getScheduler()->schedule([](float delta){
	//		auto Scene = CGameScene::createScene();
	//		Director::getInstance()->replaceScene(TransitionFade::create(0.8f, Scene));
	//	}, Director::getInstance(), 1.f, 0, 0.f, false, "createGameScene");

	//}, Director::getInstance(), 0.f, 0, 0.f, false, "createEmptyScene");
}

void CGameScene::WatchVideo()
{
	this->createVideoPopup();
//	this->offPauseButton();
//	this->gamePause();
}

void CGameScene::OpenGamePausePopup()
{
    // 이미 Pause인 상태면 리턴한다.
    if(CObjectManager::Instance()->getPlayer()->getIsDead()
    || CObjectManager::Instance()->getIsGamePause())
        return;
    
//    this->gamePause();
	this->createPausePopup();
}

void CGameScene::BackToMenuScene()
{
	/*CAudioManager::Instance()->AllPause();
    Director::getInstance()->getScheduler()->schedule([](float delta){
        
        auto tempScene = CEmptyScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.8f, tempScene));
        
        Director::getInstance()->getScheduler()->schedule([](float delta){
            auto Scene = CMenuScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.8f, Scene));
        }, Director::getInstance(), 1.f, 0, 0.f, false, "createMenuScene");
        
    }, Director::getInstance(), 0.f, 0, 0.f, false, "createEmptyScene");*/
}

//void CGameScene::gamePause()
//{
//	//오디오 소리 작게
//	CAudioManager::Instance()->setBGMVolume(0.1f);
//	CAudioManager::Instance()->setEffectSoundVolume(0.1f);
//	CObjectManager::Instance()->setIsGamePause(true);
//	CObjectManager::Instance()->setIsAbleRotation(false);
//	this->offPauseButton();
//}

void CGameScene::clearData()
{
	BONUSTIME = 0;
	ALIENGET = 0;
	CHALLENGECLEAR = 0;
	TOTALSCORE = 0;
	CObjectManager::Instance()->Clear();
	CUIManager::Instance()->Clear();
	CAudioManager::Instance()->Clear();
	CItemManager::Instance()->Clear();
}

void CGameScene::createPausePopup()
{
	CPausePopup::create()
		->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setPopupPosition(m_VisibleSize / 2)
		->show(this);
}

void CGameScene::createVideoPopup()
{
	CVideoPopup::create()
		->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setPopupPosition(m_VisibleSize / 2)
		->show(this);
}

void CGameScene::createResultPopup()
{
	CResultPopup::create()
		->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setPopupPosition(m_VisibleSize / 2)
		->show(this);
}

void CGameScene::createHelpPopup()
{
	CHelpPopup::create()
		->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setPopupPosition(m_VisibleSize / 2)
		->show(this);
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
    ->show(this);
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

//void CGameScene::onPauseButton()
//{
//	//m_PauseBtn->runAction(FadeIn::create(0.5f));
//}
//
//void CGameScene::offPauseButton()
//{
//	//m_PauseBtn->runAction(FadeTo::create(0.5f, 0));
//}
//
//void CGameScene::createTestItemButton()
//{
//	auto createItemTest = [=](eITEM_TYPE type, Vec2 pos){
//		CMyButton::create()
//			->addEventListener([=](Node* sender){
//			CItemManager::Instance()->StartItemTimer(type);
//		})
//			->setButtonNormalImage(StringUtils::format("playItem_%d.png", type))
//			->setButtonPosition(pos)
//			->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
//			->show(this);
//	};
//
//	Vec2 itemPosArray[] = {
//		Vec2(m_VisibleSize.width * 0.08f, m_VisibleSize.height * 0.5f),
//		Vec2(m_VisibleSize.width * 0.08f, m_VisibleSize.height * 0.475f),
//		Vec2(m_VisibleSize.width * 0.08f, m_VisibleSize.height * 0.45f),
//		Vec2(m_VisibleSize.width * 0.08f, m_VisibleSize.height * 0.425f),
//		Vec2(m_VisibleSize.width * 0.08f, m_VisibleSize.height * 0.4f),
//		Vec2(m_VisibleSize.width * 0.08f, m_VisibleSize.height * 0.375f)
//	};
//
//	for (int idx = 0; idx < 6; idx++)
//	{
//		createItemTest((eITEM_TYPE)(eITEM_TYPE::eITEM_TYPE_health + idx), itemPosArray[idx]);
//	}
//}

void CGameScene::initKeyboardListener()
{
	EventListenerKeyboard * pListener = EventListenerKeyboard::create();
	pListener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* pEvent)
	{
		switch (code)
		{
			case EventKeyboard::KeyCode::KEY_SPACE: m_KeyBoardSpace = true; break;
		}
	};

	pListener->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* pEvent)
	{
		switch (code)
		{
			case EventKeyboard::KeyCode::KEY_SPACE: m_KeyBoardSpace = false; break;
		}
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, this);
}