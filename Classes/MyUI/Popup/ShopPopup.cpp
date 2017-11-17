#include "ShopPopup.hpp"
#include "WorkshopPopup.h"
#include "ItemShopPopup.hpp"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../../GameObject/ObjectManager.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../SDKBOX/SDKBoxHeaders.h"
#include <array>

using namespace cocos2d;
using namespace cocos2d::ui;

CShopPopup* CShopPopup::create()
{
    CShopPopup *pRet = new(std::nothrow) CShopPopup();
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

bool CShopPopup::init()
{
    if (!CPopup::init()) return false;
    
    auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
    if (bg != nullptr){
        bg->setIgnoreAnchorPointForPosition(false);
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bg->setPosition(Vec2(this->getContentSize().width * 0.5f,
                             this->getContentSize().height * 1.5f));
        this->addChild(bg);
    }
    
    Size layerSize = bg->getContentSize();
    
    
    m_ContentScrollView = createPageView(Size(layerSize.width, layerSize.height * 0.9f),
                                         Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f));
    bg->addChild(m_ContentScrollView);
    
    // Create the title scroll view
    m_TitleScrollView = createListView(Size(layerSize.width, layerSize.height * 0.2f),
                                       layerSize.width / 5,
                                       Vec2(layerSize.width * 0.5f, layerSize.height * 0.8f));
    bg->addChild(m_TitleScrollView);
    
    // 임시 막음
    m_ContentScrollView->setTouchEnabled(false);
    
    std::array<std::string, 1> titleArray = {
        TRANSLATE("WORKSHOP_POPUP_TITLE"),
//        "아이템샵",
    };
    
    std::array<CPopup*, 1> contentArray = {
        CWorkshopPopup::create(),
//        CItemShopPopup::create()
    };
    
    
    for(int index = 0; index < titleArray.size(); index++){
        auto topBtn = Button::create("empty_150x150.png");
        topBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        topBtn->setCascadeOpacityEnabled(true);
        topBtn->addClickEventListener([=](Ref* sender){
            
            auto btn = dynamic_cast<Button*>(sender);
            m_TitleScrollView->scrollToItem(btn->getTag(), Vec2::ANCHOR_MIDDLE,
                                            Vec2::ANCHOR_MIDDLE, .5f);
            m_ContentScrollView->scrollToItem(btn->getTag());
        });
        m_TitleScrollView->addChild(topBtn, 0, index);
        
        auto label = Label::createWithSystemFont(titleArray.at(index), FONT::MALGUNBD, 80);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->setPosition(topBtn->getContentSize() / 2);
        label->setCascadeOpacityEnabled(true);
        label->setCascadeColorEnabled(true);
        topBtn->addChild(label);
    }
    
    for(auto content : contentArray)
    {
        auto contentBG = Widget::create();
        contentBG->setContentSize(Size(layerSize.width, layerSize.height * 0.5f));
        contentBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_ContentScrollView->pushBackCustomItem(contentBG);
        
        content->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setPopupPosition(contentBG->getContentSize() / 2)
        ->setDefaultCallbackEnable(false)
        ->setBackgroundVisible(false)
        ->show(contentBG);
        
        content->setTag(100);
    }
    
    auto btnUserCoin = CUserCoinButton::create();
    if (btnUserCoin != nullptr)
    {
        btnUserCoin->setPosition(Vec2(layerSize.width * 0.5f,
                                      layerSize.height * 0.05f));
        btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        this->addChild(btnUserCoin);
    }
    
    auto createButton = [=](const std::function<void(Node*)> &callback, std::string imageName, Vec2 pos)->CMyButton*{
        return CMyButton::create()
        ->addEventListener(callback)
        ->setButtonSingleUse(true)
        ->setButtonNormalImage(imageName)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(pos)
        ->show(this);
    };
    
    auto btnEnd = createButton([=](Node* sender){
        this->end();
    }, "endIcon.png", Vec2(layerSize.width * 0.92f,
                           layerSize.height * 0.05f));
    
    this->setOpenAnimation([=](Node* sender){
        auto action = [=](Node* owner){
            auto delay = DelayTime::create(1.f);
            auto fade  = FadeIn::create(0.5f);
            auto sequence = Sequence::createWithTwoActions(delay, fade);
            owner->setOpacity(0);
            owner->runAction(sequence);
        };
        
        action(btnEnd);
        action(btnUserCoin);
        
        auto moveAction = MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f));
        auto easeAction = EaseExponentialInOut::create(moveAction);
        
        bg->runAction(easeAction);
        
        m_TitleScrollView->jumpToItem(0, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
        m_ContentScrollView->jumpToItem(0, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
        
    }, 1.2f);
    
    this->setCloseAnimation([=](Node* sender){
        btnEnd->runAction(FadeTo::create(0.3f, 0));
        btnUserCoin->runAction(FadeTo::create(0.3f, 0));
        
        bg->runAction(EaseExponentialInOut::create(MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f,
                                                                             layerSize.height * 1.5f))));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->end();
    });
    
    return true;
}

void CShopPopup::end(){
    CCLOG("format popup End");
    CObjectManager::Instance()->ZoomMoveMiddle();
    CGameScene::getGameScene()->MenuFadeIn();
    
    this->popupClose(1.3f);
}

void CShopPopup::TitleScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type)
{
    ListView* listView = dynamic_cast<ListView*>(ref);
    
    if (listView == nullptr) return;
    if (type != ScrollView::EventType::CONTAINER_MOVED) return;
    
    // Get center dp
    auto center         = listView->getCenterItemInCurrentView();
    auto centerIdx      = listView->getIndex(center);
    auto centerChild    = listView->getChildren().at(centerIdx);
    auto centerIcon     = dynamic_cast<Button*>(centerChild);
    if (centerIcon == nullptr) return;
    
    // Center dp color change
    //    centerIcon->setColor(COLOR::DARKGRAY);
    centerIcon->setOpacity(255);
//    centerIcon->setScale(1.5f); // 임시 막음
    
    // touch disable the other dp
    for (auto otherIcon : listView->getChildren())
    {
        if (otherIcon != nullptr && otherIcon != center)
        {
            dynamic_cast<Button*>(otherIcon)->setOpacity(255 * 0.4f);
            dynamic_cast<Button*>(otherIcon)->setScale(1.f);
        }
    }
}

void CShopPopup::ContentScrollCallback(cocos2d::Ref* ref, cocos2d::ui::PageView::EventType type)
{
    PageView* pageView = dynamic_cast<PageView*>(ref);
    if (pageView == nullptr || type != PageView::EventType::TURNING) return;
    
    // Get center dp
    auto center         = pageView->getCenterItemInCurrentView();
    auto centerIdx      = pageView->getIndex(center);
    auto centerChild    = pageView->getChildren().at(centerIdx);
    auto centerBG       = dynamic_cast<Widget*>(centerChild);
    auto centerContent  = dynamic_cast<CPopup*>(centerBG->getChildByTag(100));
    if (centerContent == nullptr) return;
    
    // scroll title view
    m_TitleScrollView->scrollToItem(centerIdx, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, .5f);
    
    // Center dp touch enable
    centerContent->popupTouchEnable(true);
    
    //    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::OPTION, (int)centerIdx);
    
    // touch disable the other dp
    for (auto otherBG : pageView->getChildren())
    {
        if (otherBG != nullptr && otherBG != center)
        {
            auto otherContent = dynamic_cast<CPopup*>(otherBG->getChildByTag(100));
            otherContent->popupTouchEnable(false);
        }
    }
}

cocos2d::ui::ListView* CShopPopup::createListView(Size size, size_t distance, Vec2 pos)
{
    auto listView = ListView::create();
    listView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    listView->setContentSize(size);
    listView->setTouchEnabled(false);
    listView->setScrollBarEnabled(false);
    listView->setItemsMargin(distance);
    listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    listView->setPosition(pos);
    listView->setMagneticType(ListView::MagneticType::CENTER);
    listView->ScrollView::addEventListener((ListView::ccScrollViewCallback)CC_CALLBACK_2(CShopPopup::TitleScrollCallback, this));
    return listView;
}

cocos2d::ui::PageView* CShopPopup::createPageView(Size size, Vec2 pos)
{
    auto pageView = PageView::create();
    pageView->setDirection(cocos2d::ui::PageView::Direction::HORIZONTAL);
    pageView->setBounceEnabled(true);
    pageView->setContentSize(size);
    pageView->setIndicatorEnabled(true);
    pageView->setIndicatorSelectedIndexColor(COLOR::DARKGRAY);
    pageView->setIndicatorPositionAsAnchorPoint(Vec2(0.5f, -0.1f));
    pageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    pageView->setPosition(pos);
    pageView->addEventListener((PageView::ccPageViewCallback)CC_CALLBACK_2(CShopPopup::ContentScrollCallback, this));
    return pageView;
}
