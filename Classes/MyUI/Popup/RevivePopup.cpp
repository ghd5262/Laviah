#include "RevivePopup.hpp"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../CountDown.hpp"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../SDKBOX/SDKBoxHeaders.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Player.h"
#include "../../Download/DownloadManager.h"
#include "../../DataManager/StageDataManager.hpp"
#include "../../DataManager/PlanetDataManager.hpp"
#include <array>

CRevivePopup* CRevivePopup::create()
{
    CRevivePopup *pRet = new(std::nothrow) CRevivePopup();
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

bool CRevivePopup::init()
{
    if (!CPopup::init()) return false;
    
    auto popupSize = this->getContentSize();
    
    /* revive label*/
//    auto reviveLabel = Label::createWithSystemFont(TRANSLATE("REVIVE_POPUP_TITLE"), FONT::MALGUNBD, 80);
//    if (reviveLabel != nullptr)
//    {
//        reviveLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//        reviveLabel->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.8f));
//        this->addChild(reviveLabel);
//        reviveLabel->setOpacity(0);
//    }
    
//    auto createBtn = [=](const std::function<void(Node*)> &callback, std::string iconName, std::string content, Color3B color)->CMyButton*{
//        auto button = CMyButton::create()
//        ->addEventListener(callback)
//        ->setButtonSingleUse(true)
//        ->setButtonNormalImage("resultPopup_1.png")
//        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
//        ->show(this);
//        
//        button->setOpacity(0);
//        button->setCascadeOpacityEnabled(true);
//        auto btnSize = button->getContentSize();
//        
//        auto contentLabel = Label::createWithSystemFont(content, FONT::MALGUNBD, 50);
//        contentLabel->setColor(color);
//        contentLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//        contentLabel->setPosition(Vec2(btnSize.width * 0.55f, btnSize.height * 0.5f));
//        button->addChild(contentLabel);
//        
//        auto icon = Sprite::create(iconName);
//        icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//        icon->setPosition(Vec2(btnSize.width * 0.55f - (contentLabel->getContentSize().width / 2) - 50, btnSize.height * 0.5f));
//        icon->setColor(color);
//        icon->setScale(100 / icon->getContentSize().height);
//        button->addChild(icon);
//        
//        return button;
//    };
//    
//    auto reviveCost = META_DATA("REVIVE_COST").asInt();
//    std::array<std::string, 2> titleArr = {
//        TRANSLATE("REVIVE_BUTTON_WATCH_VIDEO"),
//        StringUtils::format(TRANSLATE("REVIVE_BUTTON_USE_COIN").c_str(), reviveCost),
//    };
//    
//    std::array<std::string, 2> iconArr = {
//        "videoIcon.png",
//        "coinIcon.png",
//    };
//    
//    std::array<Color3B, 2> colorArr = {
//        COLOR::DARKGRAY,
//        COLOR::GOLD,
//    };
//    
//    auto video = createBtn([=](Node* s){this->reviveByVideo(s);}, iconArr[0], titleArr[0], colorArr[0] );
////    auto free  = createBtn([=](Node* s){this->reviveByFree();  }, iconArr[1], titleArr[1], colorArr[1] );
//
//    auto free = CMyButton::create()
//    ->addEventListener([=](Node* sender){ this->reviveByFree(); })
//    ->setButtonSingleUse(true)
//    ->setButtonNormalImage("resultPopup_1.png")
//    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
//    ->show(this);
//    
//    free->setOpacity(0);
//    free->setCascadeOpacityEnabled(true);
//    
//    auto reviveText   = StringUtils::format("체크포인트에서 계속하기 %d / %d", 5 - GVALUE->REVIVE_COUNT, 5);
//    auto contentLabel = Label::createWithSystemFont(reviveText, FONT::MALGUNBD, 50);
//    contentLabel->setColor(COLOR::DARKGRAY);
//    contentLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    contentLabel->setPosition(Vec2(free->getContentSize().width * 0.5f,
//                                   free->getContentSize().height * 0.5f));
//    free->addChild(contentLabel);
    

    
//    auto layer = LayerColor::create(COLOR::BRIGHTGRAY_ALPHA, 1080 * 0.9f, 1500.f * 0.5f);
//    layer->setIgnoreAnchorPointForPosition(false);
//    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    layer->setPosition(this->getContentSize() / 2);
//    this->addChild(layer);
    
//    auto buttonCreate = [=](std::function<void(Node*)> callback, std::string content, bool visible){
//        auto btn = CMyButton::create()
//        ->addEventListener(callback)
//        ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 600, 150))
//        ->setContents(content)
//        ->setButtonPosition(Vec2(layer->getContentSize().width * 0.5f,
//                                 layer->getContentSize().height * 0.2f))
//        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
//        ->setButtonSingleUse(true)
//        ->show(this);
//        btn->setVisible(visible);
//        return btn;
//    };
    
    auto labelCreate = [=](Node* parent, std::string text, size_t fSize, Vec2 fPos, Vec2 fAnchor){
        auto label = Label::createWithTTF(text, FONT::MALGUNBD, fSize);
        label->setAnchorPoint(fAnchor);
        label->setPosition(fPos);
        label->setCascadeOpacityEnabled(true);
        parent->addChild(label);
        return label;
    };
    
    auto createButton = [=](const std::function<void(Node*)> &callback, Node* parent,
                            std::string name, Vec2 pos, bool visible, bool use = true){
        auto button = CMyButton::create()
        ->addEventListener(callback)
        ->setButtonSingleUse(use)
        ->setButtonNormalImage(name)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(pos)
        ->show(parent);
        button->setVisible(visible);
        button->setCascadeOpacityEnabled(true);
        return button;
    };
    
    auto createCircleProgress = [=](Node* parent, std::string name, float timer){
        // bar
        auto bar = Sprite::create(name);
        bar->setCascadeOpacityEnabled(true);
        auto progressBar = ProgressTimer::create(bar);
        progressBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        progressBar->setPosition(parent->getContentSize().width * 0.5f,
                                 parent->getContentSize().height * 0.5f);
        progressBar->setType(ProgressTimer::Type::RADIAL);
        progressBar->setReverseDirection(true);
        progressBar->setPercentage(100.f);
        progressBar->setCascadeOpacityEnabled(true);
        parent->addChild(progressBar);
        progressBar->runAction(ProgressTo::create(timer, 0));
        
        return progressBar;
    };
    
    // create progress bar
    
    // bar bg
    auto barBG = Sprite::create("stageProgressBar2.png");
    barBG->setColor(Color3B::BLACK);
    barBG->setOpacity(255 * 0.8f);
    barBG->setCascadeOpacityEnabled(true);
    barBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(barBG);
    
    // bar
    auto bar = Sprite::create("stageProgressBar2.png");
    bar->setCascadeOpacityEnabled(true);
    bar->setColor(COLOR::GOLD);
    auto progressBar = ProgressTimer::create(bar);
    progressBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    progressBar->setPosition(barBG->getContentSize().width * 0.5f,
                             barBG->getContentSize().height * 0.5f);
    progressBar->setMidpoint(Vec2(0, 0));
    progressBar->setType(ProgressTimer::Type::BAR);
    progressBar->setBarChangeRate(Vec2(1, 0));
    progressBar->setCascadeOpacityEnabled(true);
    barBG->addChild(progressBar);
    
    auto barSize   = progressBar->getContentSize();
    auto maxLevel  = CStageDataManager::Instance()->getStageMaxLevel(GVALUE->CURRENT_PLANET);
    auto percent   = GVALUE->getPercent(GVALUE->NOTICE_LEVEL, maxLevel);
    progressBar->runAction(ProgressTo::create(1.5f * (percent / 100.f), percent));
    
    // create labels
    {
        auto curStageLevel = StringUtils::format("%d", GVALUE->NOTICE_LEVEL);
        auto maxStageLevel = StringUtils::format(" /%d (%d%%)", maxLevel, percent);
        auto stageLabel = labelCreate(progressBar, curStageLevel, 130.f,
                                      Vec2(barSize.width * 0.45f, 0),
                                      Vec2::ANCHOR_MIDDLE_BOTTOM);
        auto labelSize = stageLabel->getContentSize().width * 0.5f;
        labelCreate(progressBar, maxStageLevel, 50.f,
                    Vec2(barSize.width * 0.45f + labelSize, barSize.height * 0.2f),
                    Vec2::ANCHOR_BOTTOM_LEFT);
        
        labelCreate(progressBar, "Level  ", 50.f,
                    Vec2(barSize.width * 0.45f - labelSize, barSize.height * 0.2f),
                    Vec2::ANCHOR_BOTTOM_RIGHT);
    }

    auto reviveBG = Sprite::create("revivePopupBG.png");
    reviveBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    reviveBG->setCascadeOpacityEnabled(true);
    this->addChild(reviveBG);
    auto bgSize = reviveBG->getContentSize();
    
    auto freeRevive  = GVALUE->REVIVE_COUNT <= 4;
    auto reviveText  = StringUtils::format("체크포인트에서 계속하기 (%d / %d)", 5 - GVALUE->REVIVE_COUNT, 5);
    if(!freeRevive)
        reviveText = "동영상 보고 계속하기";
    
    labelCreate(reviveBG, reviveText, 50,
                Vec2(bgSize.width * 0.5f, bgSize.height * 0.75f),
                Vec2::ANCHOR_MIDDLE);
    
    auto reviveFree  = createButton([=](Node* sender){ this->reviveByFree(); },
                                    reviveBG, "playIcon_revive.png",
                                    Vec2(bgSize.width * 0.5f, bgSize.height * 0.35f), freeRevive);
    
    auto reviveVideo = createButton([=](Node* sender){ this->reviveByVideo(sender); },
                                    reviveBG, "videoIcon_revive.png",
                                    Vec2(bgSize.width * 0.5f, bgSize.height * 0.35f), !freeRevive);
    
    m_FreeTimer = createCircleProgress(reviveFree, "playIcon_timer.png", 5.f);
    m_VideoTimer = createCircleProgress(reviveVideo, "videoIcon_timer.png", 5.f);
    
    struct buttonInfo {
        Node* _btn;
        Vec2  _pos;
        buttonInfo(Node* btn, Vec2 pos)
        : _btn(btn)
        , _pos(pos){};
    };
    
    std::array<buttonInfo, 2> startPos = {
        buttonInfo(barBG,    Vec2(popupSize.width * 0.5f, popupSize.height * 0.2f)),
        buttonInfo(reviveBG, Vec2(popupSize.width * 0.5f, popupSize.height * -0.15f)),
    };
    
    std::array<buttonInfo, 2> targetPos = {
        buttonInfo(barBG,    Vec2(popupSize.width * 0.5f, popupSize.height * 0.6f)),
        buttonInfo(reviveBG, Vec2(popupSize.width * 0.5f, popupSize.height * 0.475f)),
    };
    
    auto btnEnd = createButton([=](Node* sender){
        this->end();
    }, this, "endIcon.png", Vec2(popupSize.width * 0.92f, popupSize.height * 0.05f), true);
    btnEnd->setOpacity(0);
    
    m_CountDown = CCountDown::create()
    ->addLastEventListner([=](Node* sender){
        this->end();
    })
    ->setFont(Color4B::WHITE, 65)
    ->setMaxNumber(5)
    ->setMinNumber(0)
    ->setInterval(1.0f)
    ->setLabelPosition(Vec2(popupSize.width * 1.5f, popupSize.height * 0.75f))
    ->setLabelAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(this);
    
//    auto btnUserCoin = CUserCoinButton::create();
//    if (btnUserCoin != nullptr)
//    {
//        btnUserCoin->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.05f));
//        btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//        btnUserCoin->setOpacity(0);
//        this->addChild(btnUserCoin);
//    }
    
    this->setOpenAnimation([=](Node* sender){
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
        progressBar->runAction(FadeIn::create(1.f));

        btnEnd->runAction(FadeIn::create(0.5f));
//        reviveLabel->runAction(FadeIn::create(0.5f));
//        btnUserCoin->runAction(FadeIn::create(0.5f));
    }, 1.3f);
    
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
        progressBar->runAction(FadeTo::create(0.2f, 0));

        btnEnd->runAction(FadeTo::create(0.5f, 0));
//        reviveLabel->runAction(FadeTo::create(0.5f, 0));
//        btnUserCoin->runAction(FadeTo::create(0.5f, 0));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->end();
    });
    return true;
}

void CRevivePopup::end(){
    if(m_ReviveButtonTouched) return;
    
    m_IsEnded = true;
    m_CountDown->Pause();
    m_CountDown->setVisible(false);
    m_FreeTimer->pause();
    m_VideoTimer->pause();
    CGameScene::getGameScene()->GameResult();
    this->popupClose();
}

void CRevivePopup::reviveByFree()
{
    if(m_IsEnded) return;
    
//    auto reviveCost = META_DATA("REVIVE_COST").asInt();
//    if (CUserDataManager::Instance()->CoinUpdate(-reviveCost)){
//        CGoogleAnalyticsManager::LogEventCoin(GA_ACTION::COIN_USE_REVIVE, -reviveCost);
        m_ReviveButtonTouched = true;
        this->gameResume();
//    }
}

void CRevivePopup::reviveByVideo(cocos2d::Node* sender)
{
    if(m_IsEnded) return;
    m_ReviveButtonTouched = true;
    m_CountDown->Pause();
    m_FreeTimer->pause();
    m_VideoTimer->pause();
    auto showAdsFailed = [=](){
        auto button = dynamic_cast<CMyButton*>(sender);
        if(!button) return;
        button->setTouchEnable(true);
        m_ReviveButtonTouched = false;
        m_CountDown->Resume();
        m_FreeTimer->resume();
        m_VideoTimer->resume();
    };
    
    CDownloadManager::IsNetworkConnected([=](bool connected){
        if(connected){
            CUnityAdsManager::Instance()->ShowUnityAds([=](){
                CGoogleAnalyticsManager::LogEventAction(GA_CATEGORY::WATCH_ADS, GA_ACTION::ADS_REVIVE);
                this->gameResume();
            });
            CUnityAdsManager::Instance()->setUnityAdsFailedCallback([=](){
                showAdsFailed();
            });
        }
        else{
            CGameScene::getGameScene()->CreateAlertPopup()
            ->setPositiveButton([=](Node* sender){
                showAdsFailed();
            }, TRANSLATE("BUTTON_OK"))
            ->setMessage(TRANSLATE("NETWORK_CONNECT_CHECK"))
            ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
        }
    });
}

void CRevivePopup::gameResume()
{
    GVALUE->REVIVE_COUNT += 1;
    //    GVALUE->REVIVED = 1;
    CUserDataManager::Instance()->setUserData_NumberAdd(USERDATA_KEY::SAVED_REVIVE, 1);
    
    m_CountDown->Pause();
    m_FreeTimer->pause();
    m_VideoTimer->pause();
    m_CountDown->setVisible(false);
    CObjectManager::Instance()->getPlayer()->PlayerAlive();
    CObjectManager::Instance()->ReviveFromSavedPoint();
    CGameScene::getGameScene()->GameResume();
    
    this->popupClose();
}
