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
    removeAllChildrenWithCleanup(true);
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
        CUserDataManager::Instance();
		CCharacterDataManager::Instance();
        CSDKUtil::Instance();

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
//            auto button = CMyButton::createWithLayerColor(Size(80, 80), Color4B(255, 255, 255, 0), "", 0, Color3B::WHITE, END, [alien](){
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
    
	auto gameStartBtn = CMyButton::createWithLayerColor(Size(430, 150), Color4B(255, 48, 48, 255 * 0.8f), "Start", 40, Color3B::WHITE,
                                   END, std::bind(&CMenuScene::createGameScene, this), EFFECT_SIZEDOWN);
    
    gameStartBtn->setPosition(Vec2(origin.x + visibleSize.width * 0.8f,
                                 origin.x + visibleSize.height * 0.1f));
    gameStartBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    gameStartBtn->setCascadeOpacityEnabled(true);
    this->addChild(gameStartBtn);
    
    
	auto characterSelectBtn = CMyButton::createWithLayerColor(Size(430, 150), Color4B(255, 48, 48, 255 * 0.8f), "Select", 40, Color3B::WHITE,
                                                          END, [this, origin, visibleSize](){
                                                              auto popup = CPopup::createWithSpecificFormat(CCharacterSelectPopup::create(), POPUPEFFECT_none);
                                                              popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
                                                                                      origin.x + visibleSize.height * 0.5f));
                                                              popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                                              this->addChild(popup);
                                                          }, EFFECT_SIZEDOWN);
    
    characterSelectBtn->setPosition(Vec2(origin.x + visibleSize.width * 0.8f,
                                   origin.x + visibleSize.height * 0.2f));
    characterSelectBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    characterSelectBtn->setCascadeOpacityEnabled(true);
    this->addChild(characterSelectBtn);
    
	auto workShopBtn = CMyButton::createWithLayerColor(Size(430, 150), Color4B(255, 48, 48, 255 * 0.8f), "Workshop", 40, Color3B::WHITE,
                                                          END, [this, origin, visibleSize](){
                                                              auto popup = CPopup::createWithSpecificFormat(CWorkshopPopup::create(), POPUPEFFECT_none);
                                                              popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
                                                                                      origin.x + visibleSize.height * 0.5f));
                                                              popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                                              this->addChild(popup);
                                                          }, EFFECT_SIZEDOWN);
    
    workShopBtn->setPosition(Vec2(origin.x + visibleSize.width * 0.8f,
                                         origin.x + visibleSize.height * 0.3f));
    workShopBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    workShopBtn->setCascadeOpacityEnabled(true);
    this->addChild(workShopBtn);


	auto gachaBtn = CMyButton::createWithLayerColor(Size(430, 150), Color4B(255, 48, 48, 255 * 0.8f), "Gacha", 40, Color3B::WHITE,
		END, [this, origin, visibleSize](){
		auto popup = CPopup::createWithSpecificFormat(CGachaPopup::create(), POPUPEFFECT_none);
		popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.x + visibleSize.height * 0.5f));
		popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		this->addChild(popup);
	}, EFFECT_SIZEDOWN);

	gachaBtn->setPosition(Vec2(origin.x + visibleSize.width * 0.8f,
		origin.x + visibleSize.height * 0.4f));
	gachaBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	gachaBtn->setCascadeOpacityEnabled(true);
	this->addChild(gachaBtn);
    
    
	auto unityBtnReward = CMyButton::createWithLayerColor(Size(430, 150), Color4B(255, 48, 48, 255 * 0.8f), "RewardAds", 40, Color3B::WHITE,
		END, [this, origin, visibleSize](){
		CSDKUtil::Instance()->ShowRewardUnityAds([this, origin, visibleSize](){
			auto popup = CPopup::createWithOneButton("Finished RewardAds",
				CMyButton::createWithLayerColor(Size(430, 150), Color4B(0, 0, 0, 255 * 0.8f), "OK", 40, Color3B::WHITE,
				END, [](){}), 40);
			popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
				origin.x + visibleSize.height * 0.5f));
			popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			this->addChild(popup);
		});
	}, EFFECT_SIZEDOWN);
    
    unityBtnReward->setPosition(Vec2(origin.x + visibleSize.width * 0.8f,
                                     origin.x + visibleSize.height * 0.5f));
    unityBtnReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    unityBtnReward->setCascadeOpacityEnabled(true);
    this->addChild(unityBtnReward);


	auto unityBtnNormal = CMyButton::createWithLayerColor(Size(430, 150), Color4B(255, 48, 48, 255 * 0.8f), "NormalAds", 40, Color3B::WHITE,
		END, [this, origin, visibleSize](){
		CSDKUtil::Instance()->ShowNormalUnityAds([this, origin, visibleSize](){
			auto popup = CPopup::createWithOneButton("Finished NormalAds",
				CMyButton::createWithLayerColor(Size(430, 150), Color4B(0, 0, 0, 255 * 0.8f), "OK", 40, Color3B::WHITE,
				END, [](){}), 40);
			popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
				origin.x + visibleSize.height * 0.5f));
			popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			this->addChild(popup);
		});
	}, EFFECT_SIZEDOWN);

	unityBtnNormal->setPosition(Vec2(origin.x + visibleSize.width * 0.8f,
		origin.x + visibleSize.height * 0.6f));
	unityBtnNormal->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	unityBtnNormal->setCascadeOpacityEnabled(true);
	this->addChild(unityBtnNormal);


	auto googleCloudSaveTestBtn = CMyButton::createWithLayerColor(Size(430, 150), Color4B(255, 48, 48, 255 * 0.8f), "CloudSave", 40, Color3B::WHITE,
		END, [this, origin, visibleSize](){
		auto popup = CPopup::createWithSpecificFormat(CGoogleCloudTestPopup::create(), POPUPEFFECT_none);
		popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.x + visibleSize.height * 0.5f));
		popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		this->addChild(popup);
	}, EFFECT_SIZEDOWN);

	googleCloudSaveTestBtn->setPosition(Vec2(origin.x + visibleSize.width * 0.8f,
		origin.x + visibleSize.height * 0.7f));
	googleCloudSaveTestBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	googleCloudSaveTestBtn->setCascadeOpacityEnabled(true);
	this->addChild(googleCloudSaveTestBtn);
    
    //downloader.reset(new network::Downloader());
    
    //auto downloadByCDNTestBtn = CMyButton::createWithLayerColor(Size(430, 150), Color4B(255, 48, 48, 255 * 0.8f), "Downlaod", 40, Color3B::WHITE,
    //    END, [this, origin, visibleSize](){
    //        
    //        downloader->createDownloadDataTask("http://www.nowtrade.co.kr/Resources/imageRes/coin_1.png", "coin_1.png");
    //        
    //        
    //        
    //        // define progress callback
    //        downloader->onTaskProgress = [this](const network::DownloadTask& task,
    //                                            int64_t bytesReceived,
    //                                            int64_t totalBytesReceived,
    //                                            int64_t totalBytesExpected)
    //        {
    //            float percent = float(totalBytesReceived * 100) / totalBytesExpected;
    //            char buf[32];
    //            sprintf(buf, "%.1f%%[total %d KB]", percent, int(totalBytesExpected/1024));
    //    
    //            CCLOG("%s", buf);
    //        };
    //        
    //        // define success callback
    //        downloader->onDataTaskSuccess = [this, origin, visibleSize](const cocos2d::network::DownloadTask& task, std::vector<unsigned char>& data)
    //        {
    //            // create texture from data
    //            Texture2D* texture = nullptr;
    //            do
    //            {
    //                Image img;
    //                if (false == img.initWithImageData(data.data(), data.size()))
    //                {
    //                    break;
    //                }
    //                
    //                texture = new Texture2D();
    //                if (false == texture->initWithImage(&img))
    //                {
    //                    break;
    //                }
    //                auto sprite = Sprite::createWithTexture(texture);
    //                sprite->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.x + visibleSize.height * 0.5f));
    //                auto spriteSize = sprite->getContentSize();
    //                this->addChild(sprite);
    //                
    //            } while (0);
    //            CC_SAFE_RELEASE(texture);
    //        };
    //        
    //        downloader->onFileTaskSuccess = [this](const cocos2d::network::DownloadTask& task)
    //        {
    //            Texture2D* texture = nullptr;
    //            do
    //            {
    //                auto view = this->getChildByName(task.identifier);
    //                if (std::string::npos == task.storagePath.find(".png"))
    //                {
    //                    // download big file success
    //                    char buf[32];
    //                    sprintf(buf, "Download [%s] success.", task.identifier.c_str());
    //                    break;
    //                }
    //                // create sprite from file
    //                auto sprite = Sprite::create(task.storagePath);
    //                auto viewSize = view->getContentSize();
    //                sprite->setPosition(viewSize.width / 2, viewSize.height / 2);
    //                auto spriteSize = sprite->getContentSize();
    //                float scale = MIN((viewSize.height - 20) / spriteSize.height, (viewSize.width - 20) / spriteSize.width);
    //                sprite->setScale(scale);
    //                view->addChild(sprite);
    //                
    //            } while (0);
    //            CC_SAFE_RELEASE(texture);
    //        };
    //        
    //        // define failed callback
    //        downloader->onTaskError = [this](const cocos2d::network::DownloadTask& task,
    //                                         int errorCode,
    //                                         int errorCodeInternal,
    //                                         const std::string& errorStr)
    //        {
    //            CCLOG("Failed to download : %s, identifier(%s) error code(%d), internal error code(%d) desc(%s)"
    //                , task.requestURL.c_str()
    //                , task.identifier.c_str()
    //                , errorCode
    //                , errorCodeInternal
    //                , errorStr.c_str());
    //        };
    //        
    //    }, EFFECT_SIZEDOWN);
    //
    //downloadByCDNTestBtn->setPosition(Vec2(origin.x + visibleSize.width * 0.8f,
    //                                         origin.x + visibleSize.height * 0.8f));
    //downloadByCDNTestBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    //downloadByCDNTestBtn->setCascadeOpacityEnabled(true);
    //this->addChild(downloadByCDNTestBtn);
    
}

void CMenuScene::createGameScene()
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