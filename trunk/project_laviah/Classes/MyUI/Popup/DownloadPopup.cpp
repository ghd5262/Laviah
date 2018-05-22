#include "DownloadPopup.hpp"
#include "../../GameObject/ObjectManager.h"
#include "../../Scene/GameScene.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CDownloadPopup* CDownloadPopup::create()
{
    CDownloadPopup *pRet = new(std::nothrow) CDownloadPopup();
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

bool CDownloadPopup::init()
{
    if (!CPopup::init()) return false;
    
    auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
    if (bg != nullptr){
        bg->setIgnoreAnchorPointForPosition(false);
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bg->setPosition(Vec2(this->getContentSize().width * 0.5f,
                             this->getContentSize().height * 1.5f));
        this->addChild(bg);
    }
    
    Size layerSize = bg->getContentSize();
    
    auto downloadButton = Sprite::create("downloadButton.png");
    downloadButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    downloadButton->setPosition(layerSize / 2);
    bg->addChild(downloadButton);
    
    this->setOpenAnimation([=](Node* sender){
        auto action = [=](Node* owner){
            auto delay = DelayTime::create(1.f);
            auto fade  = FadeIn::create(0.5f);
            auto sequence = Sequence::createWithTwoActions(delay, fade);
            owner->setOpacity(0);
            owner->runAction(sequence);
        };
        
        auto moveAction = MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f));
        auto easeAction = EaseExponentialInOut::create(moveAction);
        
        bg->runAction(easeAction);
    }, 1.2f);
    
    this->setCloseAnimation([=](Node* sender){
        
        bg->runAction(EaseExponentialInOut::create(MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f,
                                                                             layerSize.height * 1.5f))));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->end();
    });
    
    return true;
}

void CDownloadPopup::end(){
    CCLOG("format popup End");
    CObjectManager::Instance()->ZoomMoveMiddle();
    CGameScene::getGameScene()->MenuFadeIn();
    
    this->popupClose(1.3f);
}
