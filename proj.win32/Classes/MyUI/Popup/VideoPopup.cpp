#include "VideoPopup.h"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../CountDown.hpp"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../SDKUtil/SDKUtil.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Player.h"

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

    auto popupSize = this->getContentSize();
    
	/* revive label*/
	auto reviveLabel = Label::createWithSystemFont(TRANSLATE("REVIVE_POPUP_TITLE"), FONT::MALGUNBD, 80);
	if (reviveLabel != nullptr)
	{
		reviveLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		reviveLabel->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.85f));
		reviveLabel->setColor(COLOR::DARKGRAY);
		this->addChild(reviveLabel);
		reviveLabel->setOpacity(0);
	}

    auto createButton = [=](const std::function<void(Node*)> &callback, std::string iconName, std::string content, Vec2 pos, Color3B color)->CMyButton*{
		auto button = CMyButton::create()
			->addEventListener(callback)
            ->setButtonSingleUse(true)
			->setButtonNormalImage("resultPopup_1.png")
			->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->setButtonPosition(pos)
			->show(this);

		button->setOpacity(0);
		button->setCascadeOpacityEnabled(true);
        auto btnSize = button->getContentSize();

		auto contentLabel = Label::createWithSystemFont(content, FONT::MALGUNBD, 50);
		contentLabel->setColor(color);
		contentLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		contentLabel->setPosition(btnSize / 2);
		button->addChild(contentLabel);

        auto icon = Sprite::create(iconName);
        icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		icon->setPosition(Vec2(btnSize.width * 0.5f - (contentLabel->getContentSize().width / 2) - 50, btnSize.height * 0.5f));
        icon->setColor(color);
        button->addChild(icon);
        

        
        return button;
	};

    Vec2 posArray[] = {
        Vec2(popupSize.width * 0.5f, popupSize.height * 0.2f),
        Vec2(popupSize.width * 0.5f, popupSize.height * 0.1f)
    };
    
    std::string contentArray[] = {
        TRANSLATE("REVIVE_BUTTON_WATCH_VIDEO"),
        StringUtils::format(TRANSLATE("REVIVE_BUTTON_USE_COIN").c_str(), g_coinToRevive)
    };
    
    std::string iconArray[] = {
        "videoIcon.png",
        "coin_5.png"
    };
    
	/* Video */
	auto btnVideo = createButton([=](Node* sender){
		this->Video(sender);
    }, iconArray[0], contentArray[0], posArray[0], COLOR::DARKGRAY);

	/* Use Coin */
	auto btnCoin = createButton([=](Node* sender){
		this->UseCoin(sender);
	}, iconArray[1], contentArray[1], posArray[1], COLOR::GOLD);
    
	if (CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::COIN) < g_coinToRevive)
		btnCoin->setTouchEnable(false);

    auto btnEnd = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->End(sender);
    })
    ->setButtonSingleUse(true)
    ->setButtonNormalImage("endIcon.png")
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(popupSize.width * 0.92f, popupSize.height * 0.05f))
    ->show(this);
    
    btnEnd->setOpacity(0);
    btnEnd->setCascadeOpacityEnabled(true);
    
    m_CountDown = CCountDown::create()
    ->addLastEventListner([=](Node* sender){
        this->End(sender);
    })
    ->setFont(Color4B::WHITE, 65)
    ->setMaxNumber(10)
    ->setMinNumber(0)
    ->setInterval(0.8f)
    ->setLabelPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.7f))
    ->setLabelAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(this);
    
	auto btnUserCoin = CUserCoinButton::create();
	if (btnUserCoin != nullptr)
	{
		btnUserCoin->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.05f));
		btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		btnUserCoin->setCascadeOpacityEnabled(true);
		btnUserCoin->setOpacity(0);
        this->addChild(btnUserCoin);
    }

	this->setOpenAnimation([=](Node* sender){
		auto action = [=](Node* sender, float height){
			sender->runAction(
				Spawn::createWithTwoActions(
				EaseExponentialOut::create(
				MoveTo::create(1.3f, Vec2(popupSize.width * 0.5f, popupSize.height * height))),
				FadeIn::create(1.f)));
		};

		action(btnVideo, 0.55f);
		action(btnCoin, 0.5f);
        
        btnEnd->runAction(FadeIn::create(0.5f));
        reviveLabel->runAction(FadeIn::create(0.5f));
		btnUserCoin->runAction(FadeIn::create(0.5f));
	});

	this->setCloseAnimation([=](Node* sender){

		auto action = [=](Node* sender, float height){
			sender->runAction(
				Spawn::createWithTwoActions(
				EaseSineIn::create(
				MoveTo::create(0.35f, Vec2(popupSize.width * 0.5f, popupSize.height * height))),
				FadeTo::create(0.2f, 0)));
		};

		action(btnVideo, 0.2f);
		action(btnCoin, 0.1f);
        
        btnEnd->runAction(FadeTo::create(0.5f, 0));
        reviveLabel->runAction(FadeTo::create(0.5f, 0));
		btnUserCoin->runAction(FadeTo::create(0.5f, 0));
	});
    
    this->setDefaultCallback([=](Node* sender){
        this->End(sender);
    });

	return true;
}

void CVideoPopup::End(Node* sender){
    m_CountDown->Pause();
    m_CountDown->setVisible(false);
    CGameScene::getGameScene()->GameResult();
	this->popupClose();
}

void CVideoPopup::Video(Node* sender){
	CSDKUtil::Instance()->ShowRewardUnityAds([=](){this->Resume(); });
}

void CVideoPopup::UseCoin(Node* sender){
	if (CUserDataManager::Instance()->CoinUpdate(-g_coinToRevive)){
		this->Resume();
	}
}

void CVideoPopup::Resume()
{
	GLOBAL->REVIVE_COUNT += 1;

    m_CountDown->Pause();
    m_CountDown->setVisible(false);
    CObjectManager::Instance()->getPlayer()->PlayerAlive();
    CGameScene::getGameScene()->GameResume();
	this->popupClose();
}