#include "MenuScene.h"
#include "GameScene.h"
#include "EmptyScene.h"
#include "../GameObject/Planet.h"
#include "../GameObject/Rocket.h"
#include "../GameObject/Player.h"
#include "../MyUI/MyButton.h"
#include "../Task/PoolingManager.h"
#include "../GameObject/MenuSceneObjectManager.h"
#include "../DataManager/BulletPatternDataManager.h"
#include "../DataManager/BulletDataManager.h"
#include "../DataManager/WorkshopItemDataManager.h"
#include "../DataManager/UserDataManager.h"
#include "../MyUI/Popup/CharacterSelectPopup.h"
#include "../MyUI/Popup/WorkshopPopup.h"
#include "../MyUI/Popup/GachaPopup.h"
#include "../MyUI/Popup/PatternTestPopup.h"
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
	clearData();
	m_MenuScene = this;

	CBulletPatternDataManager::Instance();
	CBulletDataManager::Instance();
	CWorkshopItemDataManager::Instance();
	CCharacterDataManager::Instance();
	CGradientDataManager::Instance();

	Size visibleSize = Director::getInstance()->getVisibleSize();

	int currentCharacterIdx = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::CHARACTER);
	sCHARACTER_PARAM currentCharacterInfo = CCharacterDataManager::Instance()->getCharacterInfoByIndex(currentCharacterIdx);

	auto planet = CPlanet::create();
	planet->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	planet->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f));
	planet->setScale(2.f);
	this->addChild(planet);
	CMenuSceneObjectManager::Instance()->setPlanet(planet);

	auto player = CPlayer::create();
	player->setScale(2.f);
	player->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	player->setPosition(Vec2(visibleSize.width * 0.5f, planet->getPosition().y + (((planet->getContentSize().width * planet->getScale()) / 2) + 20)));
	this->addChild(player);
	CMenuSceneObjectManager::Instance()->setPlayer(player);

	auto rocket = CRocket::create(sROCKET_PARAM());
	if (rocket != nullptr)
	{
		this->addChild(rocket);
		CMenuSceneObjectManager::Instance()->setRocket(rocket);
	}

	InitMenuSceneUI();

	return true;
}

static const Color4B COLOR_BUTTON_RED = COLOR::BRIGHTRED_ALPHA;

void CMenuScene::InitMenuSceneUI()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto createOneButtonPopup = [=](std::string message){
		CPopup::create()
			->setPositiveButton([=](Node* sender){}, "OK")
			->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
			->setMessage(message)
			->setMessageFont(Color3B::WHITE, 40)
			->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
			->setPopupPosition(visibleSize / 2)
			->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->show(this);
	};

	auto createWidgetPopup = [=](CPopup* widget){
		widget->setPopupPosition(visibleSize / 2)
			->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->show(this);
	};

	auto createTestButton = [=](const std::function<void(Node*)> &callback, std::string name, Vec2 pos, Size size){
		return CMyButton::create()
			->addEventListener(callback)
			->setLayer(LayerColor::create(COLOR_BUTTON_RED, size.width, size.height))
			->setContents(name)
			->setFont(Color3B::WHITE, 40)
			->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->setButtonPosition(pos)
			->show(this);
	};

	Vec2 testButtonPos[] = {
		Vec2(visibleSize.width * 0.2f, visibleSize.height * 0.25f),
		Vec2(visibleSize.width * 0.4f, visibleSize.height * 0.25f),
		Vec2(visibleSize.width * 0.6f, visibleSize.height * 0.25f),
		Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.25f),
	};

	createTestButton([=](Node* sender){ createOneButtonPopup("Share is comming soon"); }, "Share", testButtonPos[0], Size(200, 150));
	createTestButton([=](Node* sender){	createWidgetPopup(CCharacterSelectPopup::create());	}, "Select", testButtonPos[1], Size(200, 150));
	createTestButton([=](Node* sender){	this->createGameScene(sender); }, "Start", testButtonPos[2], Size(200, 150));
	createTestButton([=](Node* sender){	createWidgetPopup(CWorkshopPopup::create()); }, "Work", testButtonPos[3], Size(200, 150));

		/*createTestButton([=](Node* sender){ this->createGameScene(sender); }, "Start", testButtonPos[0], testButtonSize[0]);
		createTestButton([=](Node* sender){	createWidgetPopup(CCharacterSelectPopup::create());	}, "Select", testButtonPos[1], testButtonSize[0]);
		createTestButton([=](Node* sender){	createWidgetPopup(CWorkshopPopup::create()); }, "Workshop", testButtonPos[2], testButtonSize[0]);
		createTestButton([=](Node* sender){	createWidgetPopup(CGachaPopup::create()); }, "Gacha", testButtonPos[3], testButtonSize[0]);
		createTestButton([=](Node* sender){
		CSDKUtil::Instance()->ShowNormalUnityAds([=](){
		createOneButtonPopup("Finished Normal Ads");
		});
		}, "NormalAds", testButtonPos[4], testButtonSize[0]);
		createTestButton([=](Node* sender){
		CSDKUtil::Instance()->ShowRewardUnityAds([=](){
		createOneButtonPopup("Finished Reward Ads");
		});
		}, "RewardAds", testButtonPos[5], testButtonSize[0]);

		createTestButton([=](Node* sender){	createWidgetPopup(CPatternTestPopup::create()); }, "Pattern", testButtonPos[12], testButtonSize[0]);



	/*Vec2 testButtonPos[] = {
		Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.1f),
		Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.2f),
		Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.3f),
		Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.4f),
		Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.5f),
		Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.6f),
		Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.6f),
		Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.5f),
		Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.4f),
		Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.3f),
		Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.2f),
		Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.1f),
		Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.7f),
    };

	Size testButtonSize[] = {
		Size(430, 150),
		Size(150, 150)
	};

	typedef std::pair<std::string, std::string> SOUND_PAIR;
	SOUND_PAIR sounds[] = {
		SOUND_PAIR("Click_1-1.mp3", ""),
        SOUND_PAIR("Click_1-2.mp3", ""),
        SOUND_PAIR("Click_2-1.mp3", ""),
        SOUND_PAIR("Click_3.mp3", ""),
        SOUND_PAIR("Click_4-2.mp3", ""),
        SOUND_PAIR("Click_6-2.mp3", ""),
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

	auto createTestButton = [=](const std::function<void(Node*)> &callback, std::string name, Vec2 pos, Size size){
		return CMyButton::create()
			->addEventListener(callback)
			->setLayer(LayerColor::create(COLOR_BUTTON_RED, size.width, size.height))
			->setContents(name)
			->setFont(Color3B::WHITE, 40)
			->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->setButtonPosition(pos)
			->show(this);
	};

	auto createSoundTestButton = [=](const std::function<void(Node*)> &callback, std::string name, Vec2 pos, Size size, SOUND_PAIR sound){
		createTestButton(callback, name, pos, size)->setClickSound(sound.first, sound.second);
	};*/

	/*createTestButton([=](Node* sender){ this->createGameScene(sender); }, "Start", testButtonPos[0], testButtonSize[0]);
	createTestButton([=](Node* sender){	createWidgetPopup(CCharacterSelectPopup::create());	}, "Select", testButtonPos[1], testButtonSize[0]);
	createTestButton([=](Node* sender){	createWidgetPopup(CWorkshopPopup::create()); }, "Workshop", testButtonPos[2], testButtonSize[0]);
	createTestButton([=](Node* sender){	createWidgetPopup(CGachaPopup::create()); }, "Gacha", testButtonPos[3], testButtonSize[0]);
	createTestButton([=](Node* sender){
		CSDKUtil::Instance()->ShowNormalUnityAds([=](){
			createOneButtonPopup("Finished Normal Ads");
		});
	}, "NormalAds", testButtonPos[4], testButtonSize[0]);
	createTestButton([=](Node* sender){
        CSDKUtil::Instance()->ShowRewardUnityAds([=](){
            createOneButtonPopup("Finished Reward Ads");
        });
	}, "RewardAds", testButtonPos[5], testButtonSize[0]);

	createTestButton([=](Node* sender){	createWidgetPopup(CPatternTestPopup::create()); }, "Pattern", testButtonPos[12], testButtonSize[0]);

	CAudioManager::Instance()->PUBLIC_CLICK_SOUND = SOUND_PAIR("", "");
	SOUND_PAIR temp1 = sounds[0];
	SOUND_PAIR temp2 = sounds[1];
	SOUND_PAIR temp3 = sounds[2];
	SOUND_PAIR temp4 = sounds[3];
	SOUND_PAIR temp5 = sounds[4];
	SOUND_PAIR temp6 = sounds[5];
    
    createSoundTestButton([=](Node* sender){CAudioManager::Instance()->PUBLIC_CLICK_SOUND = temp1; }, "1", testButtonPos[6], testButtonSize[1], sounds[0]);
	createSoundTestButton([=](Node* sender){CAudioManager::Instance()->PUBLIC_CLICK_SOUND = temp2; }, "2", testButtonPos[7], testButtonSize[1], sounds[1]);
	createSoundTestButton([=](Node* sender){CAudioManager::Instance()->PUBLIC_CLICK_SOUND = temp3; }, "3", testButtonPos[8], testButtonSize[1], sounds[2]);
	createSoundTestButton([=](Node* sender){CAudioManager::Instance()->PUBLIC_CLICK_SOUND = temp4; }, "4", testButtonPos[9], testButtonSize[1], sounds[3]);
	createSoundTestButton([=](Node* sender){CAudioManager::Instance()->PUBLIC_CLICK_SOUND = temp5; }, "5", testButtonPos[10], testButtonSize[1], sounds[4]);
	createSoundTestButton([=](Node* sender){CAudioManager::Instance()->PUBLIC_CLICK_SOUND = temp6; }, "6", testButtonPos[11], testButtonSize[1], sounds[5]);
   
	createTestButton([=](Node* sender){
		createWidgetPopup(CGoogleCloudTestPopup::create());
	}, "CloudSave", Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.7f));*/
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