#include "FacebookRankUpPopup.hpp"
#include "FacebookRankPopupDP.hpp"
#include "../MyButton.h"
#include "../../Scene/GameScene.h"
#include "../../SDKBOX/SDKBox.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CFacebookRankUpPopup* CFacebookRankUpPopup::create()
{
    CFacebookRankUpPopup *pRet = new(std::nothrow) CFacebookRankUpPopup();
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

bool CFacebookRankUpPopup::init()
{
    if (!CPopup::init()) return false;
    
    auto bg = LayerColor::create(COLOR::WHITEGRAY_ALPHA, 1080.f, 800);
    if (bg != nullptr){
        bg->setIgnoreAnchorPointForPosition(false);
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bg->setPosition(this->getContentSize() / 2);
        this->addChild(bg);
    }
    
    auto title     = Label::createWithTTF("축하합니다!", FONT::MALGUNBD, 80);
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    title->setPosition(Vec2(bg->getContentSize().width * 0.5f,
                            bg->getContentSize().height * 0.9f));
    title->setColor(COLOR::DARKGRAY);
    bg->addChild(title);
    
    auto myData    = CFacebookManager::Instance()->getMyFacebookData();
    auto myRank    = CFacebookManager::Instance()->getMyRank();
    auto rivalRank = myRank + 1; // 나보다 1낮은 등급
    auto rivalData = CFacebookManager::Instance()->getFriendByRank(rivalRank);
    
    auto rivalDP   = CFacebookRankPopupDP::create(rivalData, rivalRank + 1);
    rivalDP->setPosition(Vec2(bg->getContentSize().width * 0.5f,
                              bg->getContentSize().height * 0.7f));
    rivalDP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rivalDP->setScale(0.78f);
    bg->addChild(rivalDP);
    
    auto myDP      = CFacebookRankPopupDP::create(myData, myRank + 1);
    myDP->setPosition(Vec2(bg->getContentSize().width * 0.5f,
                           bg->getContentSize().height * 0.48f));
    myDP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    myDP->setScale(0.78f);
    bg->addChild(myDP);
    
    CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->End(sender);
    })
    ->setButtonSingleUse(true)
    ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 430, 150))
    ->setContents(TRANSLATE("BUTTON_OK"))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(bg->getContentSize().width * 0.5f,
                             bg->getContentSize().height * 0.2f))
    ->show(bg);
    
    this->setDefaultAnimation(ePOPUP_ANIMATION::NONE, ePOPUP_ANIMATION::CLOSE_CENTER);
    this->setOpenAnimation([=](Node* sender){
        auto moveAction = [=](Node* owner, Vec2 targetPos){
            auto delay = DelayTime::create(1.f);
            auto move  = MoveTo::create(0.5f, targetPos);
            auto ease  = EaseExponentialOut::create(move);
            auto sequence = Sequence::createWithTwoActions(delay, ease);
            owner->runAction(sequence);
        };
        
        auto action = EaseElasticOut::create(ScaleTo::create(0.5f, 1.0f), 0.5f);
        this->setScale(0.f);
        this->runAction(action);
 
        moveAction(myDP, rivalDP->getPosition());
        moveAction(rivalDP, myDP->getPosition());
    });
    
    return true;
}

void CFacebookRankUpPopup::End(Node* sender){
    this->popupClose();
}
