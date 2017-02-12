#include "OptionTitlePopup.hpp"
#include "../../MyButton.h"
#include "../../../Scene/GameScene.h"
#include "../../../DataManager/UserDataManager.h"
#include "../../../DataManager/ChallengeDataManager.hpp"

using namespace cocos2d;
using namespace cocos2d::ui;

COptionTitlePopup* COptionTitlePopup::create()
{
    COptionTitlePopup *pRet = new(std::nothrow) COptionTitlePopup();
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

bool COptionTitlePopup::init()
{
    if (!CPopup::init()) return false;
    
    this->setContentSize(Size(1080.f, 1500.f));
    auto layerSize = getContentSize();
//    auto layer = LayerColor::create(COLOR::TRANSPARENT_ALPHA,
//                                    getContentSize().width * 0.9f,
//                                    getContentSize().height * 0.5f);
//    layer->setIgnoreAnchorPointForPosition(false);
//    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    layer->setPosition(this->getContentSize() / 2);
//    this->addChild(layer);
    
    auto titleScroll = createListView(Size(layerSize.width * 0.9f, layerSize.height * 0.5f), 5,
                                      this->getContentSize() / 2);
    this->addChild(titleScroll);
    
    auto titleList = CChallengeDataManager::Instance()->getHiddenChallengeList();
    for(auto title : titleList)
    {
        auto layer = Widget::create();
        layer->setContentSize(Size(layerSize.width * 0.9f, layerSize.height * 0.05f));
        layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        titleScroll->pushBackCustomItem(layer);
        
        auto btn = this->createTitleDP(TRANSLATE((title.second)->_contents));
        btn->setSwallowTouches(false);
        btn->setButtonPosition(Vec2((layer->getContentSize().width * 0.5f) - 7.f,
                                    layer->getContentSize().height * 0.5f));
        btn->show(layer);
        
        if(1){
            btn->setTouchEnable(false, Color3B::WHITE);
        }
        else{
            btn->setColor(COLOR::GOLD);
        }
    }

    return true;
}

CMyButton* COptionTitlePopup::createTitleDP(std::string content)
{
    return CMyButton::create()
    ->addEventListener([=](Node* sender){
    
    })
    ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA,
                                  getContentSize().width * 0.9f - 7,
                                  getContentSize().height * 0.05f))
    ->setContents(content)
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE);
}

cocos2d::ui::ListView* COptionTitlePopup::createListView(Size size, size_t distance, Vec2 pos)
{
    auto listView = ListView::create();
    listView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(true);
    listView->setBackGroundImageScale9Enabled(true);
    listView->setScrollBarPositionFromCorner(Vec2(7, 7));
    listView->setSwallowTouches(false);
    listView->setContentSize(size);
    listView->setItemsMargin(distance);
    listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    listView->setPosition(pos);
    listView->setMagneticType(ListView::MagneticType::BOTH_END);
//    listView->ScrollView::addEventListener((ListView::ccScrollViewCallback)CC_CALLBACK_2(COptionPopup::TitleScrollCallback, this));
    return listView;
}