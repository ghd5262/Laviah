#include "VideoPopup.h"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../SDKUtil/SDKUtil.h"

const int g_coinToRevive = 1500;

CVideoPopup* CVideoPopup::create()
{
	CVideoPopup *pRet = new(std::nothrow) CVideoPopup();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool CVideoPopup::init()
{
	if (!CPopup::init()) return false;

	CGameScene::getGameScene()->CountDown(10, "0", [=](){
		CGameScene::getGameScene()->GameEnd();
		this->popupClose();
	});

	/* revive label*/
	auto reviveLabel = Label::createWithTTF("Revive", "fonts/malgunbd.ttf", 80);
	if (reviveLabel != nullptr)
	{
		reviveLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		reviveLabel->setPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.8f));
		reviveLabel->setColor(g_labelColor2);
		this->addChild(reviveLabel);
		reviveLabel->setOpacity(0);
	}

	auto createButton = [=](const std::function<void(Node*)> &callback, std::string name, Vec2 pos)->CMyButton*{
		auto button = CMyButton::create()
			->addEventListener(callback)
			->setButtonNormalImage(name)
			->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->setButtonPosition(pos)
			->show(this);

		button->setOpacity(0);
		button->setCascadeOpacityEnabled(true);
		return button;
	};

	/* Video */
	auto btnVideo = createButton([=](Node* sender){
		this->Video(sender);
	}, "resultPopup_1.png", Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.3f));


	auto watchVideoIcon = Sprite::create("videoIcon.png");
	if (watchVideoIcon != nullptr)
	{
		watchVideoIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		watchVideoIcon->setPosition(Vec2(btnVideo->getContentSize().width * 0.5 - btnVideo->getContentSize().width * 0.18f, btnVideo->getContentSize().height * 0.5));
		btnVideo->addChild(watchVideoIcon);
		watchVideoIcon->setColor(g_labelColor1);
	}

	auto watchVideoLabel = Label::createWithTTF("Watch a Video", "fonts/malgunbd.ttf", 50);
	if (watchVideoLabel != nullptr){
		watchVideoLabel->setColor(g_labelColor1);
		watchVideoLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		watchVideoLabel->setPosition(Vec2(btnVideo->getContentSize().width * 0.5f - btnVideo->getContentSize().width * 0.13f, btnVideo->getContentSize().height * 0.5));
		btnVideo->addChild(watchVideoLabel);
	}

	/* Use Coin */
	auto btnUseCoin = createButton([=](Node* sender){
		this->UseCoin(sender);
	}, "resultPopup_1.png", Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.2f));


	auto useCoinIcon = Sprite::create("coin_5.png");
	if (useCoinIcon != nullptr)
	{
		useCoinIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		useCoinIcon->setPosition(Vec2(btnUseCoin->getContentSize().width * 0.5f - btnUseCoin->getContentSize().width * 0.18f, btnUseCoin->getContentSize().height * 0.5f));
		btnUseCoin->addChild(useCoinIcon);
		useCoinIcon->setColor(g_labelColor3);
	}

	auto useCoinLabel = Label::createWithTTF(StringUtils::format("Use %d Coin", g_coinToRevive).c_str(), "fonts/malgunbd.ttf", 50);
	if (useCoinLabel != nullptr){
		useCoinLabel->setColor(g_labelColor3);
		useCoinLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		useCoinLabel->setPosition(Vec2(btnUseCoin->getContentSize().width * 0.5f - btnUseCoin->getContentSize().width * 0.13f, btnUseCoin->getContentSize().height * 0.5f));
		btnUseCoin->addChild(useCoinLabel);
	}

	// 돈이 모자르면 버튼 불가처리
	if (CUserDataManager::Instance()->getUserData_Number("USER_COIN") < g_coinToRevive)
		btnUseCoin->setTouchEnable(false);

	auto btnEnd = createButton([=](Node* sender){
		this->End(sender);
	}, "endIcon.png", Vec2(this->getContentSize().width * 0.92f, this->getContentSize().height * 0.05f));


	auto btnUserCoin = CUserCoinButton::create();
	if (btnUserCoin != nullptr)
	{
		btnUserCoin->setPosition(Vec2(this->getContentSize().width * 0.5f,
			this->getContentSize().height * 0.05f));
		btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		this->addChild(btnUserCoin);
		btnUserCoin->setCascadeOpacityEnabled(true);
		btnUserCoin->setOpacity(0);
	}


	this->setOpenAnimation([=](Node* sender){
		auto action = [=](Node* sender, float height){
			sender->runAction(
				Spawn::createWithTwoActions(
				EaseExponentialOut::create(
				MoveTo::create(1.3f, Vec2(this->getContentSize().width * 0.5f,
				this->getContentSize().height * height))),
				FadeIn::create(1.f)));
		};

		action(btnVideo, 0.6f);
		action(btnUseCoin, 0.55f);

		reviveLabel->runAction(FadeIn::create(0.5f));
		btnEnd->runAction(FadeIn::create(0.5f));
		btnUserCoin->runAction(FadeIn::create(0.5f));
	});

	this->setCloseAnimation([=](Node* sender){

		auto action = [=](Node* sender, float height){
			sender->runAction(
				Spawn::createWithTwoActions(
				EaseSineIn::create(
				MoveTo::create(0.35f, Vec2(this->getContentSize().width * 0.5f,
				this->getContentSize().height * height))),
				FadeTo::create(0.2f, 0)));
		};

		action(btnVideo, 0.3f);
		action(btnUseCoin, 0.2f);

		reviveLabel->runAction(FadeTo::create(0.5f, 0));
		btnEnd->runAction(FadeTo::create(0.5f, 0));
		btnUserCoin->runAction(FadeTo::create(0.5f, 0));
	});

	return true;
}

void CVideoPopup::End(Node* sender){
	CCLOG("format popup End");
    CGameScene::getGameScene()->CountDownCancel();
	CGameScene::getGameScene()->GameEnd();
	this->popupClose();
}

void CVideoPopup::Video(Node* sender){
	CCLOG("format popup Video");
	CSDKUtil::Instance()->ShowRewardUnityAds([=](){this->Resume(); });
}

void CVideoPopup::UseCoin(Node* sender){
	CCLOG("format popup UseCoin");
	if (CUserDataManager::Instance()->CoinUpdate(-g_coinToRevive)){
		this->Resume();
	}
}

void CVideoPopup::Resume()
{
	CGameScene::getGameScene()->CountDownCancel();
	CGameScene::getGameScene()->GameStart();
	this->popupClose();
}