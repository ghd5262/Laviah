#include "FacebookRankPopup.hpp"
#include "FacebookRankPopupDP.hpp"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../MenuLayer.hpp"
#include "../../Scene/GameScene.h"
#include "../../DataManager/CharacterDataManager.h"
#include "../../DataManager/UserDataManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Planet.h"
#include "../../GameObject/Player.h"

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
    
    auto scrollBack = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 2100.f);
    if (scrollBack != nullptr){
        scrollBack->setIgnoreAnchorPointForPosition(false);
        scrollBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        scrollBack->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 1.5f));
        this->addChild(scrollBack);
    }
    
//    
//    auto friendList = CWorkshopItemDataManager::Instance()->getSellingWorkshopItemList();
//    Size layerSize  = scrollBack->getContentSize();
//    Size dpSize = Size(900, 150);
//    size_t dpDistance = 15;
//    float spawnCount = 6;
//    
//    unsigned currentItemIdx = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::SELECT_ITEM);
//    
//    // Create the list view
//    auto listView = ListView::create();
//    if (listView != nullptr){
//        listView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
//        listView->setBounceEnabled(true);
//        listView->setBackGroundImageScale9Enabled(true);
//        listView->setContentSize(Size(scrollBack->getContentSize().width, (dpSize.height + dpDistance) * spawnCount));
//        listView->setScrollBarPositionFromCorner(Vec2(7, 7));
//        listView->setItemsMargin(dpDistance);
//        listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//        listView->setPosition(layerSize / 2);
//        listView->setMagneticType(ListView::MagneticType::CENTER);
//        listView->ScrollView::addEventListener((ui::ListView::ccScrollViewCallback)std::bind(&CWorkshopPopup::ScrollCallback, this, std::placeholders::_1, std::placeholders::_2));
//        scrollBack->addChild(listView);
//        
//        unsigned dpIdx = 0;
//        unsigned currentItemDPIdx = 0;
//        
//        for (auto item : itemList)
//        {
//            auto itemDP = CWorkshopPopupDP::create(item);
//            itemDP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//            listView->pushBackCustomItem(itemDP);
//            
//            if (item._idx == currentItemIdx){
//                currentItemDPIdx = dpIdx;
//            }
//            dpIdx++;
//        }
//        
//        // Scrolling to current character
//        this->scheduleOnce([=](float delta){
//            listView->scrollToItem(currentItemDPIdx, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, 0.f);
//        }, 0.f, "ScrollToItem");
//    }
    
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
        action(scrollBack);
        
//        listView->jumpToItem(currentCharacterDPIdx, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
        btnEnd->setTouchEnabled(true);
        
        // set default callback again.
        this->changeDefaultCallback([=](Node* sender){ this->End(sender); });
        this->setDefaultCallbackCleanUp(true);
    });
    
    this->setCloseAnimation([=](Node* sender){
        btnEnd->runAction(FadeTo::create(0.5f, 0));
        scrollBack->runAction(FadeTo::create(0.5f, 0));
    });
    
    this->setDefaultCallback([=](Node* sender){}, false);
    
    return true;
}

void CFacebookRankPopup::End(Node* sender){
    CObjectManager::Instance()->ZoomIn();
    CMenuLayer::Instance()->setVisible(true);
    this->popupClose();
}

// When touch the select or buy button
void CFacebookRankPopup::Select(Node* sender)
{
    if (m_CenterDP == nullptr) return;
    
    auto centerCharacterParam = m_CenterDP->getCharacterParam();
    
    if (CUserDataManager::Instance()->getUserData_IsItemHave(USERDATA_KEY::CHARACTER_LIST,
                                                             centerCharacterParam->_idx))
    {
        m_CenterDP->Select();
        CObjectManager::Instance()->ChangeCharacter();
        this->End(nullptr);
    }
    else{
        auto characterName = TRANSLATE(centerCharacterParam->_name);
        CGameScene::getGameScene()->CreateAlertPopup()
        ->setPositiveButton([=](Node* sender){
            m_CenterDP->Buy();
            CObjectManager::Instance()->ChangeCharacter();
        }, TRANSLATE("BUTTON_YES"))
        ->setNegativeButton([=](Node* sender){
        }, TRANSLATE("BUTTON_NO"))
        ->setMessage(StringUtils::format(TRANSLATE("CHARACTER_BUY_CHECK").c_str(), characterName.c_str()))
        ->show(CGameScene::getGameScene(), ZORDER::POPUP);
    }
}

void CFacebookRankPopup::ScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type)
{
    ListView* listView = dynamic_cast<ListView*>(ref);
    if (listView == nullptr || type != ScrollView::EventType::CONTAINER_MOVED)
    {
        return;
    }
    
    // Get center dp
    auto center = listView->getCenterItemInCurrentView();
    auto centerIdx = listView->getIndex(center);
    auto centerChild = listView->getChildren().at(centerIdx);
    auto centerDP = dynamic_cast<CFacebookRankPopupDP*>(centerChild);
    if (!centerDP) return;
    if (centerDP == m_CenterDP) return;
    m_CenterDP = centerDP;
    
    // Center dp size up
    m_CenterDP->Center();
    
    // Get CenterDP's Character Param
    auto centerCharacterParam = m_CenterDP->getCharacterParam();
    
    // Change name label
    
    // If do not have and no random item, Change the name string to ???
    if (!CUserDataManager::Instance()->getUserData_IsItemHave(USERDATA_KEY::CHARACTER_LIST, centerCharacterParam->_idx)){
    }
    
    // Size down the other dp
    for (auto characterDP : listView->getChildren())
    {
        if (characterDP != nullptr && characterDP != center)
        {
            dynamic_cast<CFacebookRankPopupDP*>(characterDP)->DeSelect();
        }
    }
}