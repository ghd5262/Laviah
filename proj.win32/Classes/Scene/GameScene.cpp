#include "GameScene.h"
#include "../GameObject/SoccerPitch.h"
#include "../GameObject/SoccerBall.h"
#include "../GameObject/SoccerTeam.h"

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
}

bool CGameScene::init()
{
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

		//* 축구장 */
		auto soccerPitch = CSoccerPitch::create();
		this->addChild(soccerPitch, eOBJECT_Z_ORDER_soccerPitch, eOBJECT_TAG_soccerPitch);

		//* 축구공 */
		auto soccerBall = CSoccerBall::create(Vec2(soccerPitch->getContentSize().width * 0.5f, soccerPitch->getContentSize().height * 0.5f),
			30, 20, soccerPitch->getM_VecWalls());
		soccerBall->setAnchorPoint(Vec2(0.5f, 0.5f));
		this->addChild(soccerBall, eOBJECT_Z_ORDER_soccerBall, eOBJECT_TAG_soccerBall);

		//* 플레이어 팀 */
		auto playerTeam = CSoccerTeam::create(soccerPitch->getM_PlayerPost(), soccerPitch->getM_OtherPost(), soccerPitch, eTEAM_blue);
		this->addChild(playerTeam, eOBJECT_Z_ORDER_soccerTeam, eOBJECT_TAG_myTeam);

		//* 상대 팀 */
		auto otherTeam = CSoccerTeam::create(soccerPitch->getM_OtherPost(), soccerPitch->getM_PlayerPost(), soccerPitch, eTEAM_red);
		this->addChild(otherTeam, eOBJECT_Z_ORDER_soccerTeam, eOBJECT_TAG_otherTeam);
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
