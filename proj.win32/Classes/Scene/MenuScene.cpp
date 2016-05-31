#include "MenuScene.h"
#include "GameScene.h"
#include "EmptyScene.h"
#include "../GameObject/Planet.h"
#include "../GameObject/Alien.h"
#include "../MyUI/UIManager.h"
#include "../MyUI/MyButton.h"
#include "../Task/PoolingManager.h"
#include "../GameObject/MenuSceneObjectManager.h"

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
		m_MenuScene = this;
        
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        auto planet = CPlanet::create("planet.png", 170, 0.0f, 200.0f);
        planet->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
                                 origin.y + visibleSize.height * 0.5f));
        this->addChild(planet);
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
    
    auto gameStartBtn = CMyButton::createWithString("defaultBtn_2.png", "Start", 40, Color3B::WHITE, 
                                   END, std::bind(&CMenuScene::createGameScene, this), EFFECT_SIZEDOWN);
    
    gameStartBtn->setPosition(Vec2(origin.x + visibleSize.width * 0.8f,
                                 origin.x + visibleSize.height * 0.1f));
    gameStartBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    gameStartBtn->setCascadeOpacityEnabled(true);
    this->addChild(gameStartBtn);
    
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
