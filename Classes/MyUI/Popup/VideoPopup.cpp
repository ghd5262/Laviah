#include "VideoPopup.h"
#include "RewardPopup.h"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../CountDown.hpp"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/ChallengeRewarder/ChallengeRewarder.hpp"
#include "../../SDKUtil/SDKUtil.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Player.h"
#include <array>

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
//	if (!CPopup::init()) return false;
//
//    auto popupSize = this->getContentSize();
//    
//	/* revive label*/
//	auto reviveLabel = Label::createWithSystemFont(TRANSLATE("REVIVE_POPUP_TITLE"), FONT::MALGUNBD, 80);
//	if (reviveLabel != nullptr)
//	{
//		reviveLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//		reviveLabel->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.8f));
//		reviveLabel->setColor(COLOR::DARKGRAY);
//		this->addChild(reviveLabel);
//		reviveLabel->setOpacity(0);
//	}
//
//    auto createButton = [=](const std::function<void(Node*)> &callback, std::string iconName, std::string content, Vec2 pos, Color3B color)->CMyButton*{
//		auto button = CMyButton::create()
//			->addEventListener(callback)
//            ->setButtonSingleUse(true)
//			->setButtonNormalImage("resultPopup_1.png")
//			->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
//			->setButtonPosition(pos)
//			->show(this);
//
//		button->setOpacity(0);
//		button->setCascadeOpacityEnabled(true);
//        auto btnSize = button->getContentSize();
//
//		auto contentLabel = Label::createWithSystemFont(content, FONT::MALGUNBD, 50);
//		contentLabel->setColor(color);
//		contentLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//		contentLabel->setPosition(btnSize / 2);
//		button->addChild(contentLabel);
//
//        auto icon = Sprite::create(iconName);
//        icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//		icon->setPosition(Vec2(btnSize.width * 0.5f - (contentLabel->getContentSize().width / 2) - 50, btnSize.height * 0.5f));
//        icon->setColor(color);
//        button->addChild(icon);
//        
//
//        
//        return button;
//	};
//
//    Vec2 posArray[] = {
//        Vec2(popupSize.width * 0.5f, popupSize.height * 0.2f),
//        Vec2(popupSize.width * 0.5f, popupSize.height * 0.1f)
//    };
//    
//    std::string contentArray[] = {
//        TRANSLATE("REVIVE_BUTTON_WATCH_VIDEO"),
//        StringUtils::format(TRANSLATE("REVIVE_BUTTON_USE_COIN").c_str(), g_coinToRevive)
//    };
//    
//    std::string iconArray[] = {
//        "videoIcon.png",
//        "coin_5.png"
//    };
//    
//	/* Video */
//	auto btnVideo = createButton([=](Node* sender){
//		this->Video(sender);
//    }, iconArray[0], contentArray[0], posArray[0], COLOR::DARKGRAY);
//
//	/* Use Coin */
//	auto btnCoin = createButton([=](Node* sender){
//		this->UseCoin(sender);
//	}, iconArray[1], contentArray[1], posArray[1], COLOR::GOLD);
//    
//	if (CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::COIN) < g_coinToRevive)
//		btnCoin->setTouchEnable(false);
//
//    auto btnEnd = CMyButton::create()
//    ->addEventListener([=](Node* sender){
//        this->End(sender);
//    })
//    ->setButtonSingleUse(true)
//    ->setButtonNormalImage("endIcon.png")
//    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
//    ->setButtonPosition(Vec2(popupSize.width * 0.92f, popupSize.height * 0.05f))
//    ->show(this);
//    
//    btnEnd->setOpacity(0);
//    btnEnd->setCascadeOpacityEnabled(true);
//    
//    m_CountDown = CCountDown::create()
//    ->addLastEventListner([=](Node* sender){
//        this->End(sender);
//    })
//    ->setFont(Color4B::WHITE, 65)
//    ->setMaxNumber(10)
//    ->setMinNumber(0)
//    ->setInterval(0.8f)
//    ->setLabelPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.7f))
//    ->setLabelAnchorPoint(Vec2::ANCHOR_MIDDLE)
//    ->show(this);
//    
//	auto btnUserCoin = CUserCoinButton::create();
//	if (btnUserCoin != nullptr)
//	{
//		btnUserCoin->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.05f));
//		btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//		btnUserCoin->setCascadeOpacityEnabled(true);
//		btnUserCoin->setOpacity(0);
//        this->addChild(btnUserCoin);
//    }
//
//	this->setOpenAnimation([=](Node* sender){
//		auto action = [=](Node* sender, float height){
//			sender->runAction(
//				Spawn::createWithTwoActions(
//				EaseExponentialOut::create(
//				MoveTo::create(1.3f, Vec2(popupSize.width * 0.5f, popupSize.height * height))),
//				FadeIn::create(1.f)));
//		};
//
//		action(btnVideo, 0.55f);
//		action(btnCoin, 0.5f);
//        
//        btnEnd->runAction(FadeIn::create(0.5f));
//        reviveLabel->runAction(FadeIn::create(0.5f));
//		btnUserCoin->runAction(FadeIn::create(0.5f));
//	});
//
//	this->setCloseAnimation([=](Node* sender){
//
//		auto action = [=](Node* sender, float height){
//			sender->runAction(
//				Spawn::createWithTwoActions(
//				EaseSineIn::create(
//				MoveTo::create(0.35f, Vec2(popupSize.width * 0.5f, popupSize.height * height))),
//				FadeTo::create(0.2f, 0)));
//		};
//
//		action(btnVideo, 0.2f);
//		action(btnCoin, 0.1f);
//        
//        btnEnd->runAction(FadeTo::create(0.5f, 0));
//        reviveLabel->runAction(FadeTo::create(0.5f, 0));
//		btnUserCoin->runAction(FadeTo::create(0.5f, 0));
//	});
//    
//    this->setDefaultCallback([=](Node* sender){
//        this->End(sender);
//    });
    if (!CPopup::init()) return false;
    
    auto popupSize = this->getContentSize();

    /* revive label*/
    auto reviveLabel = Label::createWithSystemFont(TRANSLATE("REVIVE_POPUP_TITLE"), FONT::MALGUNBD, 80);
    if (reviveLabel != nullptr)
    {
        reviveLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        reviveLabel->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.8f));
        this->addChild(reviveLabel);
        reviveLabel->setOpacity(0);
    }
    
//    auto createBtn = [=](const std::function<void(Node*)> &callback, std::string content){
//        auto button = CMyButton::create()
//        ->addEventListener(callback)
//        ->setButtonNormalImage("resultPopup_1.png")
//        ->setContents(content)
//        ->setFont(Color3B::WHITE, 50)
//        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
//        ->show(this);
//        button->setOpacity(0);
//        
//        return button;
//    };
//    
    auto createBtn = [=](const std::function<void(Node*)> &callback, std::string iconName, std::string content, Color3B color)->CMyButton*{
        auto button = CMyButton::create()
        ->addEventListener(callback)
        ->setButtonSingleUse(true)
        ->setButtonNormalImage("resultPopup_1.png")
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
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
        icon->setScale(80 / icon->getContentSize().height);
        button->addChild(icon);
    
        return button;
    };
    
    std::array<std::string, 2> titleArr = {
        TRANSLATE("REVIVE_BUTTON_WATCH_VIDEO"),
        StringUtils::format(TRANSLATE("REVIVE_BUTTON_USE_COIN").c_str(), g_coinToRevive),
    };
    
    std::array<std::string, 2> iconArr = {
        "videoIcon.png",
        "coinIcon.png",
    };
    
    std::array<Color3B, 2> colorArr = {
        COLOR::DARKGRAY,
        COLOR::GOLD,
    };
    
    auto reviveByVideo    = createBtn([=](Node* s){this->ReviveByVideo(s); }, iconArr[0], titleArr[0], colorArr[0] );
    auto reviveByCoin     = createBtn([=](Node* s){this->ReviveByCoin(s);  }, iconArr[1], titleArr[1], colorArr[1] );
    
    struct buttonInfo {
        Node* _btn;
        Vec2  _pos;
        buttonInfo(Node* btn, Vec2 pos)
        : _btn(btn)
        , _pos(pos){};
    };
    
    std::array<buttonInfo, 2> startPos = {
        buttonInfo(reviveByVideo    , Vec2(popupSize.width * 0.5f, popupSize.height * 0.1f)),
        buttonInfo(reviveByCoin     , Vec2(popupSize.width * 0.5f, popupSize.height * 0.0f)),
    };
    
    std::array<buttonInfo, 2> targetPos = {
        buttonInfo(reviveByVideo    , Vec2(popupSize.width * 0.5f, popupSize.height * 0.55f)),
        buttonInfo(reviveByCoin     , Vec2(popupSize.width * 0.5f, popupSize.height * 0.5f)),
    };
    
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
    m_CountDown->Pause();
    
    auto btnUserCoin = CUserCoinButton::create();
    if (btnUserCoin != nullptr)
    {
        btnUserCoin->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.05f));
        btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        btnUserCoin->setCascadeOpacityEnabled(true);
        btnUserCoin->setOpacity(0);
        this->addChild(btnUserCoin);
    }
    
    
    // create crashed!!
    auto btnScreen = CMyButton::create()
    ->addEventListener([=](Node* sender){
        
        // button fade out
        sender->stopActionByTag(100);
        sender->runAction(Sequence::createWithTwoActions(FadeTo::create(0.5f, 0),
                                                         CallFunc::create([=](){
            sender->removeFromParent();
        })));
        
        auto action = [=](Node* btn, Vec2 pos){
            auto move        = MoveTo::create(1.3f, pos);
            auto exponential = EaseExponentialOut::create(move);
            auto fadein      = FadeIn::create(1.f);
            auto spawn       = Spawn::createWithTwoActions(exponential, fadein);
            
            btn->runAction(spawn);
        };
        
        for(int index = 0; index < targetPos.size(); index++){
            targetPos.at(index)._btn->setPosition(startPos.at(index)._pos);
            action(targetPos.at(index)._btn, targetPos.at(index)._pos);
        }
        btnEnd->runAction(FadeIn::create(0.5f));
        reviveLabel->runAction(FadeIn::create(0.5f));
        btnUserCoin->runAction(FadeIn::create(0.5f));
        m_CountDown->Resume();
    })
    ->setLayer(cocos2d::LayerColor::create(COLOR::TRANSPARENT_ALPHA, popupSize.width, popupSize.height))
    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setButtonSingleUse(true)
    ->setButtonPosition(popupSize / 2)
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(this);
    
    auto labelCrash = Label::createWithSystemFont("충돌!!", FONT::MALGUNBD, 80);
    labelCrash->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    labelCrash->setPosition(Vec2(btnScreen->getContentSize().width * 0.5f,
                                 btnScreen->getContentSize().height * 0.8f));
    btnScreen->addChild(labelCrash);
    
    auto labelCrashTip = Label::createWithSystemFont("항상 총알을 조심하세요!", FONT::MALGUNBD, 40);
    labelCrashTip->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    labelCrashTip->setPosition(Vec2(btnScreen->getContentSize().width * 0.5f,
                                    btnScreen->getContentSize().height * 0.75f));
    btnScreen->addChild(labelCrashTip);
    
    this->setOpenAnimation([=](Node* sender){
        auto fade  = FadeIn::create(0.5f);
        auto delay = DelayTime::create(1.f);
        auto sequence = Sequence::createWithTwoActions(delay, fade);
        sequence->setTag(100);
        btnScreen->runAction(sequence);
    });
    
    this->setCloseAnimation([=](Node* sender){
        
        auto action = [=](Node* sprite, Vec2 pos){
            auto move       = MoveTo::create(0.35f, pos);
            auto sine       = EaseSineIn::create(move);
            auto fadeout    = FadeTo::create(0.2f, 0);
            auto spawn      = Spawn::createWithTwoActions(sine, fadeout);
            
            sprite->runAction(spawn);
        };
        
        for(int index = 0; index < startPos.size(); index++){
            action(startPos.at(index)._btn, startPos.at(index)._pos);
        }
        
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

void CVideoPopup::ReviveByCoin(cocos2d::Node* sender)
{
    if (CUserDataManager::Instance()->CoinUpdate(-g_coinToRevive)){
        this->Resume();
    }
}

void CVideoPopup::ReviveByVideo(cocos2d::Node* sender)
{
    CSDKUtil::Instance()->ShowRewardUnityAds([=](){this->Resume(); });
}

void CVideoPopup::GetCoinFromVideo(cocos2d::Node* sender)
{
    CSDKUtil::Instance()->ShowRewardUnityAds([=](){
        this->createRewardPopup(CHALLENGE_REWARD_KEY::REWARD_COIN_RANDOM, 50);
    });
}

void CVideoPopup::GetNewCharacter(cocos2d::Node* sender)
{
    this->createRewardPopup(CHALLENGE_REWARD_KEY::REWARD_CHARACTER_RANDOM, 0);
}

void CVideoPopup::GetFreeReward(cocos2d::Node* sender)
{
    this->createRewardPopup(CHALLENGE_REWARD_KEY::REWARD_COIN_RANDOM, 150);
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

void CVideoPopup::createRewardPopup(std::string key, int value)
{
    auto popup = CGameScene::getGameScene()->Reward();
    auto rewardPopup = dynamic_cast<CRewardPopup*>(popup);
    rewardPopup->AddRewardToList(key, value);
}