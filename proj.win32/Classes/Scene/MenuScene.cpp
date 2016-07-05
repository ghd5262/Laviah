#include "MenuScene.h"
#include "GameScene.h"
#include "EmptyScene.h"
#include "../GameObject/Planet.h"
#include "../GameObject/Alien.h"
#include "../MyUI/UIManager.h"
#include "../MyUI/MyButton.h"
#include "../Task/PoolingManager.h"
#include "../GameObject/MenuSceneObjectManager.h"
#include "../DataManager/BulletPatternDataManager.h"
#include "../DataManager/StageDataManager.h"
#include "../DataManager/BulletDataManager.h"
#include "../DataManager/WorkshopItemDataManager.h"
#include "../MyUI/Popup/CharacterSelectPopup.h"
#include "../MyUI/Popup/WorkshopPopup.h"
#include "../MyUI/LevelProgressBar.h"

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

        auto planet = CPlanet::create("planet.png", 170, 0.0f, 200.0f);
        planet->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
                                 origin.y + visibleSize.height * 0.5f));
        this->addChild(planet);
        planet->runAction(RepeatForever::create(RotateBy::create(30, 360)));
        CMenuSceneObjectManager::Instance()->setPlanet(planet);
        
        
        CPoolingManager::Instance()->CreateAlienList(30, 800);
        
        this->addChild(CAlien::create(sALIEN_PARAM(), random<int>(10, 30), 200));
        this->addChild(CAlien::create(sALIEN_PARAM(), random<int>(10, 30), 200));
        this->addChild(CAlien::create(sALIEN_PARAM(), random<int>(10, 30), 200));
        this->addChild(CAlien::create(sALIEN_PARAM(), random<int>(10, 30), 200));
        this->addChild(CAlien::create(sALIEN_PARAM(), random<int>(10, 30), 200));
        this->addChild(CAlien::create(sALIEN_PARAM(), random<int>(10, 30), 200));
        this->addChild(CAlien::create(sALIEN_PARAM(), random<int>(10, 30), 200));
        this->addChild(CAlien::create(sALIEN_PARAM(), random<int>(10, 30), 200));
        
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

	auto bar1 = CLevelProgressBar::create(Size(800, 40), 10, 3);
	bar1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bar1->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
		origin.x + visibleSize.height * 0.1f));
	addChild(bar1);

	auto bar2 = CLevelProgressBar::create(Size(800, 40), 15, 8);
	bar2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bar2->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
		origin.x + visibleSize.height * 0.2f));
	addChild(bar2);

	auto bar3 = CLevelProgressBar::create(Size(800, 40), 3, 1);
	bar3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bar3->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
		origin.x + visibleSize.height * 0.3f));
	addChild(bar3);
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
