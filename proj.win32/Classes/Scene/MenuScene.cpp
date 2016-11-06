#include "MenuScene.h"
#include "GameScene.h"
#include "EmptyScene.h"
#include "../GameObject/Planet.h"
#include "../GameObject/Alien.h"
#include "../GameObject/SpaceShip.h"
#include "../MyUI/UIManager.h"
#include "../MyUI/MyButton.h"
#include "../Task/PoolingManager.h"
#include "../GameObject/MenuSceneObjectManager.h"
#include "../DataManager/BulletPatternDataManager.h"
#include "../DataManager/StageDataManager.h"
#include "../DataManager/BulletDataManager.h"
#include "../DataManager/WorkshopItemDataManager.h"
#include "../DataManager/UserDataManager.h"
#include "../MyUI/Popup/CharacterSelectPopup.h"
#include "../MyUI/Popup/WorkshopPopup.h"
#include "../MyUI/Popup/GachaPopup.h"
#include "../SDKUtil/SDKUtil.h"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/CharacterDataManager.h"
#include "../DataManager/GradientDataManager.h"

USING_NS_CC;
using namespace cocos2d;
using namespace cocos2d::ui;

CMenuScene* CMenuScene::m_MenuScene = nullptr;

Scene* CMenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = CMenuScene::create();
	// add layer as a child to scene
	scene->setAnchorPoint(Vec2(0, 0));
	layer->setAnchorPoint(Vec2(0, 0));
	scene->addChild(layer);

	return scene;
}

CMenuScene::~CMenuScene()
{
    CMenuSceneObjectManager::Instance()->RemoveAllObject();
    removeAllChildrenWithCleanup(true); // 이부분 검토
    CPoolingManager::Instance()->DeleteAllMemory();

	clearData();
}

void CMenuScene::clearData()
{
	m_MenuScene = nullptr;
}

bool CMenuScene::init()
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

bool CMenuScene::initVariable()
{
	try
	{
		clearData();
		m_MenuScene = this;

        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        CStageDataManager::Instance();
        CBulletPatternDataManager::Instance();
        CBulletDataManager::Instance();
		CWorkshopItemDataManager::Instance();
		CCharacterDataManager::Instance();
        CGradientDataManager::Instance();
		
		//int currentCharacterIdx = CUserDataManager::Instance()->getUserData_Number("USER_CUR_CHARACTER");
		//sCHARACTER_PARAM currentCharacterInfo = CCharacterDataManager::Instance()->getCharacterInfoByIndex(currentCharacterIdx);

//		auto planet = CPlanet::create(currentCharacterInfo._planetTextureName, 290.f, 0.0f, 200.0f);
//        if(planet != nullptr){
//            planet->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f));
//            planet->setScale(1.5f);
//            planet->runAction(RepeatForever::create(RotateBy::create(90, 360)));
//            this->addChild(planet);
//            CMenuSceneObjectManager::Instance()->setPlanet(planet);
//        }
//        auto spaceship = CSpaceShip::create(sSPACESHIP_PARAM(), 40, 430);
//        if(spaceship != nullptr)
//        {
//            this->addChild(spaceship);
//            CMenuSceneObjectManager::Instance()->setSpaceShip(spaceship);
//        }
//        
//        CPoolingManager::Instance()->CreateAlienList(100, 800);
//       
//        for(int i = 0 ;i < 10 ;i++ ){
//            auto alien = CAlien::create(sALIEN_PARAM(), random<int>(10, 30), 200);
//            auto button = CMyButton::createWithLayerColor(Size(80, 80), Color4B(255, 255, 255, 0), "", 0, Color3B::WHITE, eMYBUTTON_STATE::END, [alien](){
//                
//            });
//            alien->addChild(button);
//            this->addChild(alien);
//            
//        }
        
        InitMenuSceneUI();
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

static const Color4B COLOR_BUTTON_RED = Color4B(255, 48, 48, 255 * 0.8f);

void CMenuScene::InitMenuSceneUI()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto createTestButton = [=](const std::function<void(Node*)> &callback, std::string name, Vec2 pos){
		CMyButton::create()
			->addEventListener(callback)
			->setLayer(LayerColor::create(COLOR_BUTTON_RED, 430, 150))
			->setContents(name)
			->setFont(Color3B::WHITE, 40)
			->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->setButtonPosition(pos)
			->show(this);
	};

	auto createWidgetPopup = [=](CPopup* widget){
		widget->setPopupPosition(visibleSize / 2)
			->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->show(this);
	};

	auto createOneButtonPopup = [=](std::string message){
		CPopup::create()
			->setPositiveButton([=](Node* sender){}, "OK")
			->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
			->setMessage(message)
			->setMessageFont(Color3B::WHITE, 40)
			->setPopupPosition(visibleSize / 2)
			->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->show(this);
	};

	createTestButton([=](Node* sender){
		this->createGameScene(sender);
	}, "Start", Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.1f));
   
	createTestButton([=](Node* sender){
		createWidgetPopup(CCharacterSelectPopup::create());
	}, "Select", Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.2f));

	createTestButton([=](Node* sender){
		createWidgetPopup(CWorkshopPopup::create());
	}, "Workshop", Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.3f));

	createTestButton([=](Node* sender){
		createWidgetPopup(CGachaPopup::create());
	}, "Gacha", Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.4f));

	createTestButton([=](Node* sender){
		createOneButtonPopup("Finished Reward Ads");
	}, "RewardAds", Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.5f));

	createTestButton([=](Node* sender){
		createOneButtonPopup("Finished Normal Ads");
	}, "NormalAds", Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.6f));
    
	//createTestButton([=](Node* sender){
	//	createWidgetPopup(CGoogleCloudTestPopup::create());
	//}, "CloudSave", Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.7f));
}

void CMenuScene::createGameScene(Node* sender)
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

void CMenuScene::update(float delta)
{
    CMenuSceneObjectManager::Instance()->Execute(delta);
}