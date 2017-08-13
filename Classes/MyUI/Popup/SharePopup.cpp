#include "SharePopup.hpp"
#include "../MyButton.h"
#include "../../GameObject/ObjectManager.h"
#include "../../Scene/GameScene.h"
#include "../../SDKBOX/SDKBox.h"

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
    
    auto popupSize   = this->getContentSize();

//    auto back        = LayerColor::create(Color4B::WHITE, popupSize.width, popupSize.height);
//    back->setIgnoreAnchorPointForPosition(false);
//    back->setCascadeOpacityEnabled(true);
//    back->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    back->setPosition(popupSize / 2);
//    back->setScale(0.75f);
//    this->addChild(back);
    
    auto screen      = Node::create();
    screen->setContentSize(popupSize);
    screen->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    screen->setCascadeOpacityEnabled(true);
    screen->setPosition(popupSize / 2);
    this->addChild(screen);
    
    auto captureNode = CObjectManager::Instance()->getCaptureNode();
    auto copiedNode  = Sprite::createWithTexture(captureNode->getTexture());
    copiedNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    copiedNode->setScaleY(-1);
    copiedNode->setPosition(popupSize / 2);
    copiedNode->setCascadeOpacityEnabled(true);
    screen->addChild(copiedNode);
    
    auto ui          = Node::create();
    ui->setContentSize(popupSize);
    ui->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    ui->setPosition(popupSize / 2);
    ui->setCascadeOpacityEnabled(true);
    screen->addChild(ui);
    
    CObjectManager::Instance()->AddUIToCapturedNode(ui);
    
    auto createBtn = [=](const std::function<void(Node*)> &callback, std::string icon, Vec2 pos, bool use){
        auto btn = CMyButton::create()
        ->addEventListener(callback)
        ->setButtonSingleUse(use)
        ->setButtonNormalImage(icon)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(pos)
        ->show(this);
        btn->setCascadeOpacityEnabled(true);
        
        return btn;
    };
    
    auto btnEnd = createBtn([=](Node* sender){
        this->End();
    }, "endIcon.png", Vec2(popupSize.width * 0.92f, popupSize.height * 0.05f), true);
    
    auto btnShare = createBtn([=](Node* sender){
        CGameScene::getGameScene()->OpenPermRequestPopup([=](){
            if(CFacebookManager::Instance()->SaveNodeToFile(screen))
                CFacebookManager::OpenPhotoShareDialog("");
        });
    }, "shareIcon_1.png", Vec2(popupSize.width * 0.08f, popupSize.height * 0.05f), false);
    
    this->setOpenAnimation([=](Node* sender){
        
        auto action = [=](Node* sender, float delayTime){
            sender->setOpacity(0);
            auto delay = DelayTime::create(delayTime);
            auto fade  = FadeIn::create(0.3f);
            auto sequence = Sequence::createWithTwoActions(delay, fade);
            sender->runAction(sequence);
        };
        
        action(screen, 0.f);
        action(btnEnd, 0.3f);
        action(btnShare, 0.3f);

//        action(back, 0.f);
    }, 0.6f);
    
    this->setCloseAnimation([=](Node* sender){
        
        screen->runAction(FadeTo::create(0.3f, 0));
        btnEnd->runAction(FadeTo::create(0.3f, 0));
        btnShare->runAction(FadeTo::create(0.3f, 0));

//        back->runAction(FadeTo::create(0.3f, 0));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->End();
    });
    
    return true;
}

void CSharePopup::End(){
    this->popupClose();
}
