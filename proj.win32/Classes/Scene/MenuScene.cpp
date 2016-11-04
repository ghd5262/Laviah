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
#include "../MyUI/Popup/GoogleCloudTestPopup.h"
#include "../SDKUtil/SDKUtil.h"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/CharacterDataManager.h"
#include "../DataManager/GradientDataManager.h"

USING_NS_CC;

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

void CMenuScene::InitMenuSceneUI()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    CMyButton::create()
    ->addEventListener([=](Ref* sender, Widget::TouchEventType type){
        CPopup::create()
        ->setMessage("New Button, Popup Test Succeed")
        ->setPositiveButton([=](Ref* sender, Widget::TouchEventType type){
            CCLOG("PRESS OK!!");
        }, "OK")
        ->show(this);
    }, eMYBUTTON_STATE::EXECUTE)
    ->setLayer(LayerColor::create(Color4B(255, 48, 48, 255 * 0.8f), 430, 150))
    ->setContents("New Button")
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.8f))
    ->show(this);

    auto gameStartBtn = CMyButton::createWithLayerColor(Size(430, 150), Color4B(255, 48, 48, 255 * 0.8f), "Start", 40, Color3B::WHITE, eMYBUTTON_STATE::END, [=](Node* sender){
        this->createGameScene(sender);
//        CCLOG("Start Button Press!!");
    }, EFFECT_SIZEDOWN)
    ->show(this);
    
    gameStartBtn->setPosition(Vec2(origin.x + visibleSize.width * 0.8f,
                                 origin.x + visibleSize.height * 0.1f));
    gameStartBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    gameStartBtn->setCascadeOpacityEnabled(true);
//    this->addChild(gameStartBtn);
//
//    
	CMyButton::createWithLayerColor(Size(430, 150), Color4B(255, 48, 48, 255 * 0.8f), "Select", 40, Color3B::WHITE,
		eMYBUTTON_STATE::END, [=](Node* sender){
            
            CPopup::create(CCharacterSelectPopup::create())
            ->WidgetInitTest()
            ->setPopupPosition(visibleSize / 2)
            ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
            ->show(this);
        }, EFFECT_SIZEDOWN)
    ->setButtonPosition(Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.2f))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(this);
    
	auto workShopBtn = CMyButton::createWithLayerColor(Size(430, 150), Color4B(255, 48, 48, 255 * 0.8f), "Workshop", 40, Color3B::WHITE,
		eMYBUTTON_STATE::END, [this, origin, visibleSize](Node* sender){
                                                              auto popup = CPopup::createWithSpecificFormat(CWorkshopPopup::create(), POPUPEFFECT_none)->show(this);
                                                              popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
                                                                                      origin.x + visibleSize.height * 0.5f));
                                                              popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                                          }, EFFECT_SIZEDOWN)->show(this);
    
    workShopBtn->setPosition(Vec2(origin.x + visibleSize.width * 0.8f,
                                         origin.x + visibleSize.height * 0.3f));
    workShopBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    workShopBtn->setCascadeOpacityEnabled(true);


	auto gachaBtn = CMyButton::createWithLayerColor(Size(430, 150), Color4B(255, 48, 48, 255 * 0.8f), "Gacha", 40, Color3B::WHITE,
		eMYBUTTON_STATE::END, [this, origin, visibleSize](Node* sender){
		auto popup = CPopup::createWithSpecificFormat(CGachaPopup::create(), POPUPEFFECT_none)->show(this);
		popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.x + visibleSize.height * 0.5f));
		popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	}, EFFECT_SIZEDOWN)->show(this);

	gachaBtn->setPosition(Vec2(origin.x + visibleSize.width * 0.8f,
		origin.x + visibleSize.height * 0.4f));
	gachaBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	gachaBtn->setCascadeOpacityEnabled(true);
    
    
	auto unityBtnReward = CMyButton::createWithLayerColor(Size(430, 150), Color4B(255, 48, 48, 255 * 0.8f), "RewardAds", 40, Color3B::WHITE,
		eMYBUTTON_STATE::END, [this, origin, visibleSize](Node* sender){
		CSDKUtil::Instance()->ShowRewardUnityAds([this, origin, visibleSize](){
			auto popup = CPopup::createWithOneButton("Finished RewardAds",
				CMyButton::createWithLayerColor(Size(430, 150), Color4B(0, 0, 0, 255 * 0.8f), "OK", 40, Color3B::WHITE,
				eMYBUTTON_STATE::END, [](Node* sender){}), 40)->show(this);
			popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
				origin.x + visibleSize.height * 0.5f));
			popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		});
	}, EFFECT_SIZEDOWN)->show(this);
    
    unityBtnReward->setPosition(Vec2(origin.x + visibleSize.width * 0.8f,
                                     origin.x + visibleSize.height * 0.5f));
    unityBtnReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    unityBtnReward->setCascadeOpacityEnabled(true);


    auto unityBtnNormal = CMyButton::createWithLayerColor(Size(430, 150), Color4B(255, 48, 48, 255 * 0.8f), "NormalAds", 40, Color3B::WHITE, eMYBUTTON_STATE::END,
        [=](Node* sender){
             CSDKUtil::Instance()->ShowNormalUnityAds([=](){
                 auto popup = CPopup::createWithOneButton("Finished NormalAds",
                                                          CMyButton::createWithLayerColor(Size(430, 150), Color4B(0, 0, 0, 255 * 0.8f), "OK", 40, Color3B::WHITE, eMYBUTTON_STATE::END,
                                                            [=](Node* sender){
                                                            
                                                            }), 40)->show(this);
                 popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
                                         origin.x + visibleSize.height * 0.5f));
                 popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
             });
        }, EFFECT_SIZEDOWN)->show(this);
    
    unityBtnNormal->setPosition(Vec2(origin.x + visibleSize.width * 0.8f,
                                     origin.x + visibleSize.height * 0.6f));
    unityBtnNormal->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    unityBtnNormal->setCascadeOpacityEnabled(true);
    
    
    auto googleCloudSaveTestBtn = CMyButton::createWithLayerColor(Size(430, 150), Color4B(255, 48, 48, 255 * 0.8f), "CloudSave", 40, Color3B::WHITE,
		eMYBUTTON_STATE::END, [this, origin, visibleSize](Node* sender){
		auto popup = CPopup::createWithSpecificFormat(CGoogleCloudTestPopup::create(), POPUPEFFECT_none)->show(this);
		popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.x + visibleSize.height * 0.5f));
		popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	}, EFFECT_SIZEDOWN)->show(this);

	googleCloudSaveTestBtn->setPosition(Vec2(origin.x + visibleSize.width * 0.8f,
		origin.x + visibleSize.height * 0.7f));
	googleCloudSaveTestBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	googleCloudSaveTestBtn->setCascadeOpacityEnabled(true);
    
    auto sprite = Sprite::create("cocos2dbanner.png");
    if(sprite != nullptr)
    {
        addChild(sprite);
    }
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