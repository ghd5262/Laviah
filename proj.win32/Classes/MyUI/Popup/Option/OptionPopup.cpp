#include "OptionPopup.hpp"
#include "../../MyButton.h"
#include "../../UserCoinButton.h"
#include "../../../Scene/GameScene.h"
#include "../../../DataManager/UserDataManager.h"
#include <array>

using namespace cocos2d;
using namespace cocos2d::ui;

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
    
    // Create the title scroll view
    m_TitleScrollView = createListView(Size(layerSize.width, layerSize.height * 0.2f), 80,
                                       Vec2(layerSize.width * 0.5f, layerSize.height * 0.9f));
    scrollBack->addChild(m_TitleScrollView);
    
    m_ContentScrollView = createListView(Size(layerSize.width, layerSize.height * 0.6f), 0,
                                         Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f));
    scrollBack->addChild(m_ContentScrollView);
    
    std::array<std::string, 5> titleArray = {
        std::string("Option"),
        std::string("Save"),
        std::string("Language"),
        std::string("Challenges"),
        std::string("Developer")
    };
    std::array<LayerColor*, 5> contentArray = {
        LayerColor::create(Color4B::BLUE    ,layerSize.width, layerSize.height * 0.6f),
        LayerColor::create(Color4B::GRAY    ,layerSize.width, layerSize.height * 0.6f),
        LayerColor::create(Color4B::GREEN   ,layerSize.width, layerSize.height * 0.6f),
        LayerColor::create(Color4B::MAGENTA ,layerSize.width, layerSize.height * 0.6f),
        LayerColor::create(Color4B::BLACK   ,layerSize.width, layerSize.height * 0.6f),
    };
    
    for(auto title : titleArray)
    {
        auto layer = Widget::create();
        layer->setContentSize(Size(240, 80));
        layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_TitleScrollView->pushBackCustomItem(layer);

        auto label = Label::createWithTTF(title, FONT::MALGUNBD, 80);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->setPosition(layer->getContentSize() / 2);
        layer->addChild(label);
    }
    
    for(auto content : contentArray)
    {
        auto layer = Widget::create();
        layer->setContentSize(Size(layerSize.width, layerSize.height * 0.6f));
        layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_ContentScrollView->pushBackCustomItem(layer);
        
        content->setIgnoreAnchorPointForPosition(false);
        content->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        content->setPosition(layer->getContentSize() / 2);
        layer->addChild(content);
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
    
    
    auto btnEnd = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->End(sender);
    })
    ->setButtonSingleUse(true)
    ->setButtonNormalImage("endIcon.png")
    ->setButtonPosition(Vec2(bg->getContentSize().width * 0.92f,
                             bg->getContentSize().height * 0.05f))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(bg);
    
    btnEnd->setOpacity(0);
    
    
    this->setOpenAnimation([=](Node* sender){
        btnEnd->runAction(FadeIn::create(0.5f));
        btnUserCoin->runAction(FadeIn::create(0.5f));
        scrollBack->runAction(EaseExponentialOut::create(MoveTo::create(0.8f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.62f))));
    });
    
    this->setCloseAnimation([=](Node* sender){
        btnEnd->runAction(FadeTo::create(0.5f, 0));
        btnUserCoin->runAction(FadeTo::create(0.5f, 0));
        scrollBack->runAction(EaseSineIn::create(MoveTo::create(0.4f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 1.5f))));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->End(sender);
    });
    
    return true;
}

void COptionPopup::End(Node* sender){
    CCLOG("format popup End");
    this->popupClose();
}

void COptionPopup::TitleScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type)
{
    ListView* listView = dynamic_cast<ListView*>(ref);
    if (listView == nullptr || type != ScrollView::EventType::CONTAINER_MOVED) return;
    
    // Get center dp
    auto center = listView->getCenterItemInCurrentView();
    auto centerIdx = listView->getIndex(center);
    auto centerChild = listView->getChildren().at(centerIdx);
    m_CenterDP = dynamic_cast<CPopup*>(centerChild);
    if (m_CenterDP == nullptr) return;
    
    // Center dp touch enable
    m_CenterDP->popupTouchEnable(true);
    
    // Center dp color change
    
    
    // Get CenterDP's Character Param
//    auto centerCharacterParam = m_CenterDP->getCharacterParam();
    
    // Change name label
//    m_CenterCharacterNameLabel->setString(TRANSLATE(centerCharacterParam->_name));
    
    // touch disable the other dp
    for (auto otherDP : listView->getChildren())
    {
        if (otherDP != nullptr && otherDP != center)
        {
            dynamic_cast<CPopup*>(otherDP)->popupTouchEnable(false);
        }
    }
}

cocos2d::ui::ListView* COptionPopup::createListView(Size size, size_t distance, Vec2 pos)
{
    auto listView = ListView::create();
    listView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    listView->setBounceEnabled(true);
    listView->setBackGroundImageScale9Enabled(true);
    listView->setContentSize(size);
    //    listView->setScrollBarPositionFromCorner(Vec2(7, 7));
    listView->setItemsMargin(distance);
    listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    listView->setPosition(pos);
    listView->setMagneticType(ListView::MagneticType::CENTER);
    listView->ScrollView::addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(COptionPopup::TitleScrollCallback, this));
    return listView;
}