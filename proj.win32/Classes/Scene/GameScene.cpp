#include "GameScene.h"
#include "../Task/Task.h"
#include "../GameObject/Bullet.h"
#include "../GameObject/Planet.h"
#include "../GameObject/ObjectManager.h"
#include "../GameObject/Shooter/Shooter.h"
USING_NS_CC;

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

CGameScene::~CGameScene()
{
	removeAllChildrenWithCleanup(true);
}

bool CGameScene::init()
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

bool CGameScene::initVariable()
{
	try
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		auto closeItem = MenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			CC_CALLBACK_1(CGameScene::menuCloseCallback, this));

		closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
			origin.y + closeItem->getContentSize().height / 2));

		auto menu = Menu::create(closeItem, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 100);

		auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));
		this->addChild(label, 100);

		auto background = Sprite::create("background_2.png");
		background->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.y + visibleSize.height * 0.5f));
		this->addChild(background);

		auto planet = std::shared_ptr<CPlanet>(::new CPlanet("planet.png", 165, 0.0f, 5.0f), [](CPlanet* planet)
		{::delete planet; });
		planet.get()->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.y + visibleSize.height * 0.25f - label->getContentSize().height));
		this->addChild(planet.get());
		CObjectManager::Instance()->setM_Planet(planet);

		CObjectManager::Instance()->setM_BulletList(new CTaskList(800, 3000));
		CObjectManager::Instance()->setM_ItemList(new CTaskList(800, 10));
		CObjectManager::Instance()->setM_EnemyList(new CTaskList(800, 20));
		//RandomShoot();
		addChild(new CRandomShooter(0.5f, Vec2(CObjectManager::Instance()->getM_Planet()->getPosition().x,
			CObjectManager::Instance()->getM_Planet()->getPosition().y)));
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CGameScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void CGameScene::update(float delta)
{
	CObjectManager::Instance()->Execute(delta);
}
