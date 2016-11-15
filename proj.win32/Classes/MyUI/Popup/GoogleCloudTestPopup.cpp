#include "GoogleCloudTestPopup.h"
#include "GoogleCloudTestPopupDP.h"
#include "GoogleCloudTestAddKeyDP.h"
#include "../MyButton.h"
#include "../../Scene/GameScene.h"
#include "ui/UIScrollView.h"
#include "ui/UIImageView.h"
#include "ui/UIPageView.h"
#include "../../DataManager/UserDataManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CGoogleCloudTestPopup* CGoogleCloudTestPopup::create()
{
    CGoogleCloudTestPopup *pRet = new(std::nothrow) CGoogleCloudTestPopup();
    if (pRet)
    {
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool CGoogleCloudTestPopup::init()
{
    if (!CPopup::init()) return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto scrollBack = LayerColor::create(COLOR::WHITEGRAY_ALPHA, 1080.f, 1500.f);
    if (scrollBack != nullptr){
        scrollBack->setIgnoreAnchorPointForPosition(false);
        scrollBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        scrollBack->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 1.5f));
        this->addChild(scrollBack);
    }
    
    /* workShop label*/
    auto workShopLabel = Label::createWithTTF("GoogleCloudTest", "fonts/malgunbd.ttf", 80);
    if (workShopLabel != nullptr)
    {
        workShopLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        workShopLabel->setPosition(Vec2(scrollBack->getContentSize().width * 0.5f, scrollBack->getContentSize().height * 0.9f));
        workShopLabel->setColor(COLOR::DARKGRAY);
        scrollBack->addChild(workShopLabel);
        workShopLabel->setOpacity(0);
    }
    
    
    auto keyList = CUserDataManager::Instance()->getKeyList();
    Size layerSize = scrollBack->getContentSize();
    Size dpSize = Size(1080, 200);
    size_t dpDistance = 15;
    float spawnCount = 4;
    
    unsigned currentKeyIdx = CUserDataManager::Instance()->getUserData_Number("USER_CUR_SELECT_KEY");
    
    // Create the list view
    auto listView = ListView::create();
    if (listView != nullptr){
        listView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
        listView->setBounceEnabled(true);
        listView->setBackGroundImageScale9Enabled(true);
        listView->setContentSize(Size(scrollBack->getContentSize().width, (dpSize.height + dpDistance) * spawnCount));
        listView->setScrollBarPositionFromCorner(Vec2(7, 7));
        listView->setItemsMargin(dpDistance);
        listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        listView->setPosition(layerSize / 2);
        listView->setMagneticType(ListView::MagneticType::CENTER);
        listView->ScrollView::addEventListener((ui::ListView::ccScrollViewCallback)std::bind(&CGoogleCloudTestPopup::ScrollCallback, this, std::placeholders::_1, std::placeholders::_2));
        scrollBack->addChild(listView);
        
        unsigned dpIdx = 0;
        unsigned currentItemDPIdx = 0;
        
        for (auto key : keyList)
        {
            auto keyDP = CGoogleCloudTestPopupDP::create(key.first);
            keyDP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            listView->pushBackCustomItem(keyDP);
            
            if( dpIdx == currentKeyIdx ){
                currentItemDPIdx = dpIdx;
            }
            dpIdx++;
        }
        
        // Scrolling to current character
        Director::getInstance()->getScheduler()->schedule([listView, currentItemDPIdx](float delta){
            listView->scrollToItem(currentItemDPIdx, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, 0.f);
        }, Director::getInstance(), 0.f, 0, 0.f, false, "ScrollToItem");
    }
    
    
    auto btnEnd = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->End(sender);
    })
    ->setButtonNormalImage("endIcon.png")
    ->setButtonPosition(Vec2(this->getContentSize().width * 0.92f,
                             this->getContentSize().height * 0.05f))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(this);
    
    btnEnd->setOpacity(0);
    
    
    this->setOpenAnimation([=](Node* pausePopup){
        
        workShopLabel->runAction(FadeIn::create(0.5f));
        scrollBack->runAction(EaseExponentialOut::create(MoveTo::create(0.8f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.12f))));
        btnEnd->runAction(FadeIn::create(0.5f));
    });
    
    this->setCloseAnimation([=](Node* pausePopup){
        workShopLabel->runAction(FadeTo::create(0.5f, 0));
        btnEnd->runAction(FadeTo::create(0.5f, 0));
        scrollBack->runAction(EaseSineIn::create(MoveTo::create(0.4f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 1.5f))));
    });
    
    return true;
}

void CGoogleCloudTestPopup::End(Node* sender){
    CCLOG("format popup End");
    this->popupClose();
}

void CGoogleCloudTestPopup::ScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type)
{
    ListView* listView = dynamic_cast<ListView*>(ref);
    if (listView == nullptr || type != ScrollView::EventType::CONTAINER_MOVED)
    {
        return;
    }
}