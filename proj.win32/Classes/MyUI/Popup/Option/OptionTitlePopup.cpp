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
//    listView->setSwallowTouches(false);
    listView->setContentSize(size);
    listView->setItemsMargin(distance);
    listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    listView->setPosition(pos);
    listView->setMagneticType(ListView::MagneticType::BOTH_END);
    listView->ScrollView::addEventListener((ListView::ccScrollViewCallback)CC_CALLBACK_2(COptionTitlePopup::TitleScrollCallback, this));
    return listView;
}

void COptionTitlePopup::TitleScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type)
{
    ListView* listView = dynamic_cast<ListView*>(ref);
    
    if (listView == nullptr) return;
    
    //switch (type) {
//        case ScrollView::EventType::CONTAINER_MOVED:
//        case ScrollView::EventType::SCROLL_TO_TOP:CCLOG("SCORLLTAG_SCROLL_TO_TOP");
//            listView->setSwallowTouches(true); break;
//
//        case ScrollView::EventType::SCROLL_TO_BOTTOM:CCLOG("SCORLLTAG_SCROLL_TO_BOTTOM");
//            listView->setSwallowTouches(true); break;
//
//        case ScrollView::EventType::SCROLL_TO_RIGHT:
//        case ScrollView::EventType::SCROLL_TO_LEFT:CCLOG("SCORLLTAG_SCROLL_TO_LEFT");
//            listView->setSwallowTouches(false); break;

//        case ScrollView::EventType::SCROLL_TO_RIGHT:CCLOG("SCORLLTAG_SCROLL_TO_RIGHT");
//        case ScrollView::EventType::SCROLLING:
//            listView->setSwallowTouches(true);
//            CCLOG("SCORLLTAG_SCROLLING");
//            break;
//        case ScrollView::EventType::AUTOSCROLL_ENDED:
//            listView->setSwallowTouches(false);
//            CCLOG("SCORLLTAG_AUTOSCROLL_ENDED");
//            break;
//        case ScrollView::EventType::CONTAINER_MOVED:
//            listView->setSwallowTouches(true);
//            CCLOG("SCORLLTAG_CONTAINER_MOVED");
//            break;
//        case ScrollView::EventType::BOUNCE_TOP:CCLOG("SCORLLTAG_BOUNCE_TOP"); break;
//        case ScrollView::EventType::BOUNCE_LEFT://CCLOG("SCORLLTAG_BOUNCE_LEFT"); break;
//        case ScrollView::EventType::BOUNCE_RIGHT://CCLOG("SCORLLTAG_BOUNCE_RIGHT"); break;
//            listView->setSwallowTouches(false);
//            break;
//        case ScrollView::EventType::BOUNCE_BOTTOM:CCLOG("SCORLLTAG_BOUNCE_BOTTOM"); break;

//            listView->setSwallowTouches(false); break;

//        default:
//            listView->setSwallowTouches(true);
//            break;
//    }
}
