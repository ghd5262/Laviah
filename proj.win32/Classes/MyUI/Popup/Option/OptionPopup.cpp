#include "OptionPopup.hpp"
#include "OptionHeaders.h"
#include "../../MyButton.h"
#include "../../UserCoinButton.h"
#include "../../../GameObject/ObjectManager.h"
#include "../../../Scene/GameScene.h"
#include "../../../DataManager/UserDataManager.h"
#include <array>

namespace OPTION_POPUP {
    static int CONTENT_TAG = 100;
}
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace OPTION_POPUP;

COptionPopup* COptionPopup::create()
{
    COptionPopup *pRet = new(std::nothrow) COptionPopup();
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

bool COptionPopup::init()
{
    if (!CPopup::init()) return false;
    
    Size visibleSize = this->getContentSize();
    
    auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
    if (bg != nullptr){
        bg->setIgnoreAnchorPointForPosition(false);
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bg->setPosition(this->getContentSize() / 2);
        this->addChild(bg);
    }
    
    auto scrollBack = LayerColor::create(COLOR::WHITEGRAY_ALPHA, 1080.f, 1500.f);
    if (scrollBack != nullptr){
        scrollBack->setIgnoreAnchorPointForPosition(false);
        scrollBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        scrollBack->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 1.5f));
        this->addChild(scrollBack);
    }
    
    Size layerSize = scrollBack->getContentSize();
    
 
    m_ContentScrollView = createPageView(Size(layerSize.width, layerSize.height * 1.2f),
                                         Vec2(layerSize.width * 0.5f, layerSize.height * 0.4f));
    scrollBack->addChild(m_ContentScrollView);
    
    // Create the title scroll view
    m_TitleScrollView = createListView(Size(layerSize.width, layerSize.height * 0.2f), layerSize.width / 5,
                                       Vec2(layerSize.width * 0.5f, layerSize.height * 0.8f));
    scrollBack->addChild(m_TitleScrollView);
    
    std::array<std::string, 5> iconArray = {
        std::string("musicIcon.png"),
        std::string("saveIcon.png"),
        std::string("languageIcon.png"),
        std::string("challengeIcon.png"),
        std::string("developerIcon.png")
    };

    std::array<CPopup*, 5> contentArray = {
        COptionMusicPopup::create(),
        COptionDataSavePopup::create(),
        COptionLanguagePopup::create(),
        COptionTitlePopup::create(),
        COptionDeveloperPopup::create(),
    };
    
    int iconIndex = 0;
    for(auto icon : iconArray)
    {
        auto iconBtn = Button::create(icon);
        iconBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        iconBtn->setColor(COLOR::DARKGRAY);
        iconBtn->setTag(iconIndex++);
        iconBtn->addClickEventListener([=](Ref* sender){
            auto btn = dynamic_cast<Button*>(sender);
            m_TitleScrollView->scrollToItem(btn->getTag(), Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, .5f);
            m_ContentScrollView->scrollToItem(btn->getTag());
        });
        m_TitleScrollView->addChild(iconBtn);
    }
    
    for(auto content : contentArray)
    {
        auto bg = Widget::create();
        bg->setContentSize(Size(layerSize.width, layerSize.height * 0.5f));
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_ContentScrollView->pushBackCustomItem(bg);
        
        content->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setPopupPosition(bg->getContentSize() / 2)
        ->setDefaultCallbackEnable(false)
        ->setBackgroundVisible(false)
        ->show(bg);
        
        content->setTag(CONTENT_TAG);
    }
        
    auto btnUserCoin = CUserCoinButton::create();
    if (btnUserCoin != nullptr)
    {
        btnUserCoin->setPosition(Vec2(bg->getContentSize().width * 0.5f,
                                      bg->getContentSize().height * 0.05f));
        btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        btnUserCoin->setCascadeOpacityEnabled(true);
        btnUserCoin->setOpacity(0);
        bg->addChild(btnUserCoin);
    }
    
    auto createButton = [=](const std::function<void(Node*)> &callback, std::string imageName, Vec2 pos)->CMyButton*{
        auto btn = CMyButton::create()
        ->addEventListener(callback)
        ->setButtonSingleUse(true)
        ->setButtonNormalImage(imageName)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(pos)
        ->show(bg);
        btn->setOpacity(0);
        return btn;
    };
    
    auto btnEnd = createButton([=](Node* sender){
        this->End(sender);
    }, "endIcon.png", Vec2(bg->getContentSize().width * 0.92f,
                           bg->getContentSize().height * 0.05f));
    
    auto btnShare = createButton([=](Node* sender){
        CObjectManager::Instance()->Share();
        this->End(sender);
    }, "shareIcon_2.png", Vec2(bg->getContentSize().width * 0.08f,
                           bg->getContentSize().height * 0.05f));
    
    
    this->setOpenAnimation([=](Node* sender){
        btnEnd->runAction(FadeIn::create(0.5f));
        btnShare->runAction(FadeIn::create(0.5f));

        btnUserCoin->runAction(FadeIn::create(0.5f));
        scrollBack->runAction(EaseExponentialOut::create(MoveTo::create(0.8f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.62f))));
        m_TitleScrollView->jumpToItem(m_InitialScrollIndex, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
        m_ContentScrollView->jumpToItem(m_InitialScrollIndex, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
    });
    
    this->setCloseAnimation([=](Node* sender){
        btnEnd->runAction(FadeTo::create(0.5f, 0));
        btnShare->runAction(FadeTo::create(0.5f, 0));

        btnUserCoin->runAction(FadeTo::create(0.5f, 0));
        scrollBack->runAction(EaseSineIn::create(MoveTo::create(0.4f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 1.5f))));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->End(sender);
    });
    
    return true;
}

COptionPopup* COptionPopup::setInitialScrollIndex(int index)
{
    m_InitialScrollIndex = index;
    return this;
}

void COptionPopup::End(Node* sender){
    CCLOG("format popup End");
    this->popupClose();
}

void COptionPopup::TitleScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type)
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
    centerIcon->setScale(2.f);
    
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

void COptionPopup::ContentScrollCallback(cocos2d::Ref* ref, cocos2d::ui::PageView::EventType type)
{
    PageView* pageView = dynamic_cast<PageView*>(ref);
    if (pageView == nullptr || type != PageView::EventType::TURNING) return;
    
    // Get center dp
    auto center         = pageView->getCenterItemInCurrentView();
    auto centerIdx      = pageView->getIndex(center);
    auto centerChild    = pageView->getChildren().at(centerIdx);
    auto centerBG       = dynamic_cast<Widget*>(centerChild);
    auto centerContent  = dynamic_cast<CPopup*>(centerBG->getChildByTag(CONTENT_TAG));
    if (centerContent == nullptr) return;
    
    // scroll title view
    m_TitleScrollView->scrollToItem(centerIdx, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, .5f);
    
    // Center dp touch enable
    centerContent->popupTouchEnable(true);
    
    // touch disable the other dp
    for (auto otherBG : pageView->getChildren())
    {
        if (otherBG != nullptr && otherBG != center)
        {
            auto otherContent = dynamic_cast<CPopup*>(otherBG->getChildByTag(CONTENT_TAG));
            otherContent->popupTouchEnable(false);
        }
    }
}

cocos2d::ui::ListView* COptionPopup::createListView(Size size, size_t distance, Vec2 pos)
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
    listView->ScrollView::addEventListener((ListView::ccScrollViewCallback)CC_CALLBACK_2(COptionPopup::TitleScrollCallback, this));
    return listView;
}

cocos2d::ui::PageView* COptionPopup::createPageView(Size size, Vec2 pos)
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
    pageView->addEventListener((PageView::ccPageViewCallback)CC_CALLBACK_2(COptionPopup::ContentScrollCallback, this));
    return pageView;
}