#include "SharePopup.hpp"
#include "../MyButton.h"
#include "../../GameObject/ObjectManager.h"
#include "../../Scene/GameScene.h"
#include "../../Facebook/FacebookManager.hpp"

CSharePopup* CSharePopup::create()
{
    CSharePopup *pRet = new(std::nothrow) CSharePopup();
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

bool CSharePopup::init()
{
    if (!CPopup::init()) return false;
    
    auto popupSize = this->getContentSize();
    
    auto back = LayerColor::create(Color4B::WHITE, 1000, 1600);
    back->setCascadeOpacityEnabled(true);
    back->setIgnoreAnchorPointForPosition(false);
    back->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    back->setPosition(popupSize / 2);
    this->addChild(back);
    
    auto pic  = CObjectManager::Instance()->Capture(900, 1600);
    pic->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    pic->setPosition(Vec2(back->getContentSize().width * 0.5f,
                          back->getContentSize().height * 0.47f));
    back->addChild(pic);
    
    auto date = Label::createWithTTF(CTranslateManager::Instance()->getCurrentDate(false),
                                     FONT::MALGUN, 50);
    date->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    date->setPosition(Vec2(back->getContentSize().width * 0.06f,
                           back->getContentSize().height * 0.16f));
    date->setColor(COLOR::DARKGRAY);
    back->addChild(date);
    
    auto createBtn = [=](const std::function<void(Node*)> &callback, std::string icon, Vec2 pos){
        auto btn = CMyButton::create()
        ->addEventListener(callback)
        ->setButtonSingleUse(true)
        ->setButtonNormalImage(icon)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(pos)
        ->show(this);
        btn->setColor(COLOR::DARKGRAY);
        
        return btn;
    };
    
    auto btnEnd = createBtn([=](Node* sender){
        this->End();
    }, "endIcon.png", Vec2(popupSize.width * 0.88f,
                           popupSize.height * 0.14f));
    
    auto btnShare = createBtn([=](Node* sender){
        CGameScene::getGameScene()->OpenPermRequestPopup([=](){
            if(CFacebookManager::Instance()->SaveNodeToFile(back))
                CFacebookManager::OpenPhotoShareDialog();
        });
    }, "shareIcon_1.png", Vec2(popupSize.width * 0.12f,
                               popupSize.height * 0.14f));
    
    this->setOpenAnimation([=](Node* sender){
        
        auto action = [=](Node* sender, float delayTime){
            sender->setOpacity(0);
            auto delay = DelayTime::create(delayTime);
            auto fade  = FadeIn::create(0.3f);
            auto sequence = Sequence::createWithTwoActions(delay, fade);
            sender->runAction(sequence);
        };
        
        action(back, 0.f);
        action(btnEnd, 0.3f);
        action(btnShare, 0.3f);
    }, 0.6f);
    
    this->setCloseAnimation([=](Node* sender){
        
        back->runAction(FadeTo::create(0.3f, 0));
        btnEnd->runAction(FadeTo::create(0.3f, 0));
        btnShare->runAction(FadeTo::create(0.3f, 0));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->End();
    });
    
    return true;
}

void CSharePopup::End(){
    this->popupClose();
}