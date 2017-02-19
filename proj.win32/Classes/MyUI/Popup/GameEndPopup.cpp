#include "GameEndPopup.hpp"
#include "../MyButton.h"
#include "../../Scene/GameScene.h"

CGameEndPopup* CGameEndPopup::create()
{
    CGameEndPopup *pRet = new(std::nothrow) CGameEndPopup();
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

bool CGameEndPopup::init()
{
    if (!CPopup::init()) return false;
    
    auto popupSize = this->getContentSize();
    
    /* crash label*/
    auto btn = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->End(sender);
    })
    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, popupSize.width, popupSize.height))
    ->setContents("Crash!")
    ->setFont(Color3B::WHITE, 65)
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.6f))
    ->show(this);
    
    this->setOpenAnimation([=](Node* sender){
        btn->runAction(FadeIn::create(0.5f));
    });
    
    this->setCloseAnimation([=](Node* sender){
        btn->runAction(FadeTo::create(0.5f, 0));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->End(sender);
    });
    
    return true;
}

void CGameEndPopup::End(Node* sender){
    if (GLOBAL->RUN_SCORE < 3000)	CGameScene::getGameScene()->GameResult();
    else							CGameScene::getGameScene()->WatchVideo();
    
    this->popupClose();
}