#include "RevivePopup.hpp"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../CountDown.hpp"
#include "../StageProgressBar.hpp"
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
    auto back = LayerColor::create(COLOR::WHITEGRAY_ALPHA, 1080, 570);
    back->setIgnoreAnchorPointForPosition(false);
    back->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    back->setPosition(popupSize / 2);
    this->addChild(back);
    
    auto labelCreate = [=](Node* parent, std::string text, size_t fSize, Vec2 fPos, Vec2 fAnchor){
        auto label = Label::createWithTTF(text, FONT::MALGUNBD, fSize);
        label->setAnchorPoint(fAnchor);
        label->setPosition(fPos);
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
        return button;
    };
    
    auto createCircleProgress = [=](Node* parent, std::string name, float timer){
        // bar
        auto bar = Sprite::create(name);
        bar->setColor(COLOR::BRIGHTRED);
        auto progressBar = ProgressTimer::create(bar);
        progressBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        progressBar->setPosition(parent->getContentSize().width * 0.5f,
                                 parent->getContentSize().height * 0.5f);
        progressBar->setType(ProgressTimer::Type::RADIAL);
        progressBar->setReverseDirection(true);
        progressBar->setPercentage(100.f);
        parent->addChild(progressBar);
        progressBar->runAction(ProgressTo::create(timer, 0));
        progressBar->setOpacity(255 * 0.8f);
        return progressBar;
    };
    
    // create progress bar
    CStageProgressBar::create()
    ->setProgressBar("achievementProgress.png")
    ->setLabelVisible(false)
    ->setBarBGColor(COLOR::TRANSPARENT_ALPHA)
    ->setBarColor(COLOR::GOLD)
    ->setBarAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP)
    ->setBarPosition(Vec2(back->getContentSize().width * 0.5f,
                          back->getContentSize().height))
    ->show(back);
    
    // create labels
    auto value = CObjectManager::Instance()->getLevelTimer();
    auto max   = CStageDataManager::Instance()->getStageFinishTime(GVALUE->CURRENT_PLANET);
    auto percent = GVALUE->getPercent(value, max);
    auto text  = StringUtils::format("%d", percent);
    
    auto percentNumberLabel = labelCreate(this, text, 280,
                                          Vec2(popupSize.width * 0.5f, popupSize.height * 0.65f),
                                          Vec2::ANCHOR_MIDDLE_BOTTOM);
    
    auto labelSize = percentNumberLabel->getContentSize().width * 0.5f;
    auto percentLabel = labelCreate(this, "%", 130,
                                    Vec2(popupSize.width * 0.5f + labelSize,
                                     popupSize.height * 0.67f),
                                    Vec2::ANCHOR_BOTTOM_LEFT);
    
    auto bgSize = back->getContentSize();
    
    auto reviveChance = META_DATA("FREE_REVIVE_CHANCE").asInt();
    auto freeRevive  = GVALUE->REVIVE_COUNT < reviveChance;
    auto reviveText  = StringUtils::format(TRANSLATE("REVIVE_POPUP_TEXT_FREE").c_str(),
                                           reviveChance - GVALUE->REVIVE_COUNT, reviveChance);
    if(!freeRevive)
        reviveText = StringUtils::format(TRANSLATE("REVIVE_POPUP_TEXT_VIDEO").c_str(), reviveChance);
    
    labelCreate(back, reviveText, 40,
                Vec2(bgSize.width * 0.5f, bgSize.height * 0.6f),
                Vec2::ANCHOR_MIDDLE)
    ->setColor(COLOR::DARKGRAY);
    
    auto reviveFree  = createButton([=](Node* sender){ this->reviveByFree(); },
                                    back, "playIcon_revive.png",
                                    Vec2(bgSize.width * 0.5f, bgSize.height * 0.25f), freeRevive);
    reviveFree->setColor(COLOR::BRIGHTRED);
    reviveFree->setOpacity(255 * 0.8f);
    auto reviveVideo = createButton([=](Node* sender){ this->reviveByVideo(sender); },
                                    back, "videoIcon_revive.png",
                                    Vec2(bgSize.width * 0.5f, bgSize.height * 0.25f), !freeRevive);
    reviveVideo->setColor(COLOR::BRIGHTRED);
    reviveVideo->setOpacity(255 * 0.8f);

    m_FreeTimer = createCircleProgress(reviveFree, "playIcon_timer.png", 5.f);
    m_VideoTimer = createCircleProgress(reviveVideo, "videoIcon_timer.png", 5.f);
    
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
    
    this->setOpenAnimation([=](Node* sender){

        back->setScale(0.f);
        back->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, 1.0f), 0.5f));
        btnEnd->runAction(FadeIn::create(0.5f));
        percentNumberLabel->runAction(FadeIn::create(0.5f));
        percentLabel->runAction(FadeIn::create(0.5f));
    }, 1.3f);
    
    this->setCloseAnimation([=](Node* sender){
        
        back->runAction(Sequence::create(EaseElasticIn::create(ScaleTo::create(0.45f, 0.1f), 1),
                                         FadeTo::create(0.f, 0), nullptr));
        btnEnd->runAction(FadeTo::create(0.5f, 0));
        percentNumberLabel->runAction(FadeTo::create(0.5f, 0));
        percentLabel->runAction(FadeTo::create(0.5f, 0));
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
                auto value = StringUtils::format("%02d_%d", GVALUE->CURRENT_PLANET, GVALUE->STAGE_LEVEL);
                CGoogleAnalyticsManager::LogEventValue(GA_CATEGORY::WATCH_ADS, GA_ACTION::ADS_REVIVE,
                                                       value);
                GSAVED->REVIVE_COUNT = 0;
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
    GSAVED->REVIVE_COUNT += 1;
    GVALUE->CopyData(GSAVED);
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
