#include "GameScene.h"
#include "../Task/PoolingManager.h"
#include "../GameObject/Bullet.h"
#include "../GameObject/Planet.h"
#include "../GameObject/Player.h"
#include "../GameObject/ObjectManager.h"
#include "../GameObject/Shooter/Shooter.h"

USING_NS_CC;
using namespace Shooter;
CGameScene* CGameScene::m_GameScene = nullptr;
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
	CObjectManager::Instance()->RemoveAllObject();
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
		m_GameScene = this;

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

		auto leftButton = MenuItemImage::create(
			"leftButton_1.png",
			"leftButton_2.png",
			CC_CALLBACK_1(CGameScene::leftButtonCallback, this));
		leftButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		leftButton->setPosition(Vec2(origin.x + visibleSize.width * 0.2f,
			origin.x + visibleSize.height * 0.15f));

		auto lbutton = Menu::create(leftButton, NULL);
		lbutton->setPosition(Vec2::ANCHOR_MIDDLE);
		this->addChild(lbutton, 100);

		auto rightButton = MenuItemImage::create(
			"rightButton_1.png",
			"rightButton_2.png",
			CC_CALLBACK_1(CGameScene::rightButtonCallback, this));
		rightButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		rightButton->setPosition(Vec2(origin.x + visibleSize.width * 0.8f,
			origin.x + visibleSize.height * 0.15f));

		auto rbutton = Menu::create(rightButton, NULL);
		rbutton->setPosition(Vec2::ANCHOR_MIDDLE);
		this->addChild(rbutton, 100);


		auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));
		this->addChild(label, 100);

		auto background = Sprite::create("background_2.png");
		background->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.y + visibleSize.height * 0.5f));
		this->addChild(background);

		auto planet = CPlanet::create("planet.png", 190.f, 0.0f, 5.0f);
		planet->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.y + visibleSize.height * 0.25f));
		this->addChild(planet, 100);

		auto player = CPlayer::create("player.png", 6.f, 0.0f, 5.0f);
		player->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.y + visibleSize.height * 0.42f));
		this->addChild(player, 100);

		CObjectManager::Instance()->setM_Player(player);
		CObjectManager::Instance()->setM_Planet(planet);
		CPoolingManager::Instance()->CreateBulletList(30, 800);
		CPoolingManager::Instance()->CreateEnemyList(5, 800);
		RandomShoot(250.0f, 0.5f, 3);
		RandomMissileShoot(600.f, 10.0f, 2);
		AimingMissileShoot(800.f, 15.0f, 2);
		//DoubleScrewShoot(250.0f, 0.1f, 1, LEFT);
		//ScrewShoot(250.0f, 0.1f, 1, RIGHT);
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

void CGameScene::rightButtonCallback(cocos2d::Ref* pSender)
{
	CCLOG("RIGHT BUTTON SELECTED");
}

void CGameScene::leftButtonCallback(cocos2d::Ref* pSender)
{
	CCLOG("LEFT BUTTON SELECTED");
}

void CGameScene::update(float delta)
{
	CObjectManager::Instance()->Execute(delta);
}
