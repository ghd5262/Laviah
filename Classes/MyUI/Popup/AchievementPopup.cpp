#include "AchievementPopup.hpp"
#include "AchievementPopupDP.hpp"
#include "../MyButton.h"
#include "../MenuLayer.hpp"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../GameObject/ObjectManager.h"
#include "../../DataManager/AchievementDataManager.hpp"

using namespace cocos2d;
using namespace cocos2d::ui;

CAchievementPopup* CAchievementPopup::create()
{
    CAchievementPopup *pRet = new(std::nothrow) CAchievementPopup();
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

bool CAchievementPopup::init()
{
    if (!CPopup::init()) return false;
    
    Size layerSize  = this->getContentSize();
    
    auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
    if (bg != nullptr){
        bg->setIgnoreAnchorPointForPosition(false);
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bg->setPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 1.5f));
        this->addChild(bg);
    }
    
    /* ranking label*/
    auto rankingLabel = Label::createWithSystemFont("Achievement", FONT::MALGUNBD, 80);
    if (rankingLabel != nullptr)
    {
        rankingLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        rankingLabel->setPosition(Vec2(bg->getContentSize().width * 0.5f,
                                       bg->getContentSize().height * 0.8f));
        this->addChild(rankingLabel);
    }
    
    auto achievementList = CAchievementDataManager::Instance()->getHiddenAchievementList();
    Size dpSize          = Size(1080, 200);
    size_t dpDistance    = 15;
    float spawnCount     = 4;
    
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
        bg->addChild(listView);
        
        // create dp
        for (auto achievement : achievementList)
        {
            auto achievementDP = CAchievementPopupDP::create(achievement.second);
            achievementDP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            achievementDP->setCascadeOpacityEnabled(true);
            listView->pushBackCustomItem(achievementDP);
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
    
    this->setOpenAnimation([=](Node* sender){
        auto action = [=](Node* owner){
            auto delay = DelayTime::create(1.f);
            auto fade  = FadeIn::create(0.5f);
            auto sequence = Sequence::createWithTwoActions(delay, fade);
            owner->setOpacity(0);
            owner->runAction(sequence);
        };
        
        action(btnEnd);
        action(rankingLabel);
        
        auto moveAction = MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f));
        auto easeAction = EaseExponentialInOut::create(moveAction);
        
        bg->runAction(easeAction);
    }, 1.2f);
    
    this->setCloseAnimation([=](Node* sender){
        bg->runAction(EaseExponentialInOut::create(MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f,
                                                                             layerSize.height * 1.5f))));
        btnEnd->runAction(FadeTo::create(0.3f, 0));
        rankingLabel->runAction(FadeTo::create(0.3f, 0));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->End(sender);
    });
    
    return true;
}

void CAchievementPopup::End(Node* sender){
    CObjectManager::Instance()->ZoomOutRank();
    CGameScene::getGameScene()->MenuFadeIn();
    
    this->popupClose(1.3f);
}