#include "FacebookRankPopup.hpp"
#include "FacebookRankPopupDP.hpp"
#include "../MyButton.h"
#include "../MenuLayer.hpp"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../Facebook/FacebookManager.hpp"

using namespace cocos2d;
using namespace cocos2d::ui;

CFacebookRankPopup* CFacebookRankPopup::create()
{
    CFacebookRankPopup *pRet = new(std::nothrow) CFacebookRankPopup();
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

bool CFacebookRankPopup::init()
{
    if (!CPopup::init()) return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
    if (bg != nullptr){
        bg->setIgnoreAnchorPointForPosition(false);
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bg->setPosition(this->getContentSize() / 2);
        this->addChild(bg);
    }
    
    auto scrollBack = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1500.f);
    if (scrollBack != nullptr){
        scrollBack->setIgnoreAnchorPointForPosition(false);
        scrollBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        scrollBack->setPosition(visibleSize / 2);
        this->addChild(scrollBack);
    }

    auto userList   = CFacebookManager::Instance()->getFBUserList();
    auto sequence   = CFacebookManager::Instance()->getMyRank();
    Size layerSize  = scrollBack->getContentSize();
    Size dpSize     = Size(900, 150);
    size_t dpDistance = 15;
    float spawnCount = 8;
    
    // Create the list view
    auto listView = ListView::create();
    if (listView != nullptr){
        listView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
        listView->setBounceEnabled(true);
        listView->setBackGroundImageScale9Enabled(true);
        listView->setContentSize(Size(dpSize.width, (dpSize.height + dpDistance) * spawnCount));
        listView->setScrollBarPositionFromCorner(Vec2(7, 7));
        listView->setItemsMargin(dpDistance);
        listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        listView->setPosition(layerSize / 2);
        listView->setMagneticType(ListView::MagneticType::BOTH_END);
        listView->setCascadeOpacityEnabled(true);
        listView->setOpacity(0);
        scrollBack->addChild(listView);
        
        auto index = 1;
        // create dp
        for (auto user : userList)
        {
            auto userDP = CFacebookRankPopupDP::create(user.second, index++);
            userDP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            userDP->setCascadeOpacityEnabled(true);
            listView->pushBackCustomItem(userDP);
        }
        
        if(userList.size() > spawnCount){
            // Scrolling to current character
            this->scheduleOnce([=](float delta){
                listView->scrollToItem(sequence, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, 0.f);
            }, 0.f, "ScrollToItem");
        }
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
    btnEnd->setTouchEnabled(false);
    
    this->setOpenAnimation([=](Node* sender){
        auto action = [=](Node* owner){
            auto delay = DelayTime::create(1.f);
            auto fade  = FadeIn::create(0.5f);
            auto sequence = Sequence::createWithTwoActions(delay, fade);
            owner->runAction(sequence);
        };
        
        action(btnEnd);
        action(listView);
        btnEnd->setTouchEnabled(true);
        
        // set default callback again.
        this->changeDefaultCallback([=](Node* sender){ this->End(sender); });
        this->setDefaultCallbackCleanUp(true);
    });
    
    this->setCloseAnimation([=](Node* sender){
        btnEnd->runAction(FadeTo::create(0.3f, 0));
        listView->runAction(FadeTo::create(0.3f, 0));

    });
    
    this->setDefaultCallback([=](Node* sender){}, false);
    
    return true;
}

void CFacebookRankPopup::End(Node* sender){
    CObjectManager::Instance()->ZoomOutRank();
    CMenuLayer::Instance()->setVisible(true);
    this->popupClose();
}