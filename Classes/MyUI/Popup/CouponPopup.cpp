#include "CouponPopup.hpp"
#include "../MyButton.h"
#include "../../DataManager/CouponDataManager.hpp"
#include "../../DataManager/AchievementRewarder/AchievementRewarder.hpp"
#include "../../Scene/GameScene.h"
#include "../../SDKBOX/SDKBoxHeaders.h"

using namespace cocos2d::ui;

CCouponPopup* CCouponPopup::create()
{
    CCouponPopup *pRet = new(std::nothrow) CCouponPopup();
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

bool CCouponPopup::init()
{
    if (!CPopup::init()) return false;
    
    auto bg = LayerColor::create(COLOR::WHITEGRAY_ALPHA, 1080.f, 570.f);
    if (bg != nullptr){
        bg->setIgnoreAnchorPointForPosition(false);
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bg->setPosition(this->getContentSize() / 2);
        this->addChild(bg);
    }
    
//    auto mentLabel = Label::createWithSystemFont(TRANSLATE("OPTION_COUPON_MENT"), FONT::MALGUNBD, 40);
//    if (mentLabel != nullptr)
//    {
//        mentLabel->setColor(COLOR::DARKGRAY);
//        mentLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//        mentLabel->setPosition(Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.7f));
//        bg->addChild(mentLabel);
//    }
    
    auto couponBG = LayerColor::create(COLOR::DARKGRAY_ALPHA,
                                       bg->getContentSize().width * 0.85f,
                                       1500.f * 0.08f);
    couponBG->setIgnoreAnchorPointForPosition(false);
    couponBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    couponBG->setPosition(Vec2(bg->getContentSize().width * 0.5f,
                               bg->getContentSize().height * 0.6f));
    bg->addChild(couponBG);
    
    m_TextField = cocos2d::ui::TextField::create(TRANSLATE("OPTION_COUPON_MENT"), FONT::MALGUNBD, 40);
    m_TextField->setPosition(couponBG->getContentSize() / 2);
    m_TextField->setTextColor(COLOR::WHITEGRAY_ALPHA);
    couponBG->addChild(m_TextField);
    
    
    CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->input();
    })
    ->setButtonSingleUse(true)
    ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 430, 150))
    ->setContents(TRANSLATE("BUTTON_CONFIRM"))
    ->setFont(Color3B::WHITE, 40)
    ->setButtonPosition(Vec2(bg->getContentSize().width * 0.275f, bg->getContentSize().height * 0.25f))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(bg);
    
    
    CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->cancel();
    })
    ->setButtonSingleUse(true)
    ->setLayer(LayerColor::create(COLOR::BRIGHTRED_ALPHA, 430, 150))
    ->setContents(TRANSLATE("BUTTON_NO"))
    ->setFont(Color3B::WHITE, 40)
    ->setButtonPosition(Vec2(bg->getContentSize().width * 0.725f, bg->getContentSize().height * 0.25f))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(bg);
    
    return true;
}

void CCouponPopup::input(){
    CCLOG("input");
    if (!m_TextField->getString().size()) {
        this->popupClose();
        return;
    };
    
    auto rewardget = [=](COUPON_REWARD_DATA data){
        std::vector<sREWARD_DATA> rewardList;
        rewardList.clear();
        for (int index = 0; index < data._keyList.size(); index++)
            rewardList.emplace_back(data._keyList.at(index), data._valueList.at(index));
        
        auto title = TRANSLATE("REWARD_TITLE_COUPON");
        CGameScene::getGameScene()->Reward([=](bool isPlay){}, rewardList, title);
        CCouponDataManager::Instance()->UseCoupon(data._couponKey);
        CGoogleAnalyticsManager::LogEventValue(GA_CATEGORY::COUPON, GA_ACTION::OPTION_COUPON,
                                               data._couponKey);
    };
    
    auto couponCheck = [=](std::string key){
        CCouponDataManager::Instance()->CouponCheck([=](int state, COUPON_REWARD_DATA data){
            switch(state)
            {
                case COUPON_STATE::EXPIRED:
                    createOneBtnPopup(TRANSLATE("OPTION_COUPON_EXPIRED"));  break;
                case COUPON_STATE::ALREADY_USED:
                    createOneBtnPopup(TRANSLATE("OPTION_COUPON_USED"));     break;
                case COUPON_STATE::AVAILABLE:
                    rewardget(data); break;
                case COUPON_STATE::WRONG:
                    createOneBtnPopup(TRANSLATE("OPTION_COUPON_WRONG"));    break;
            }
        }, key);
    };
    
    couponCheck(m_TextField->getString());
    
    this->popupClose();
}

void CCouponPopup::cancel(){
    CCLOG("cancel");
    this->popupClose();
}

void CCouponPopup::createOneBtnPopup(std::string text)
{
    CGameScene::getGameScene()->CreateAlertPopup()
    ->setPositiveButton([=](Node* sender){}, TRANSLATE("BUTTON_OK"))
    ->setMessage(text)
    ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
}
