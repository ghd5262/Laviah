#include "MenuScene.h"
#include "../GameObject/Planet.h"
#include "../MyUI/UIManager.h"
#include "../MyUI/MyButton.h"

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
    m_PauseBtn = nullptr;
    m_PauseBtn = CMyButton::create("pauseIcon.png",
                                   END,
                                   [this, origin, visibleSize]()
                                   {
                                       OpenGamePausePopup();
                                   }, EFFECT_SIZEDOWN);
    
    m_PauseBtn->setPosition(Vec2(origin.x + visibleSize.width * 0.08f,
                                 origin.x + visibleSize.height * 0.05f));
    m_PauseBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_PauseBtn->setCascadeOpacityEnabled(true);
    m_GridWorld->addChild(m_PauseBtn, 102);
    
}