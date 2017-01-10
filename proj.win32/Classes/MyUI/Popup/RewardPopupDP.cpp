#include "RewardPopupDP.hpp"
#include "../MyButton.h"
#include "../Popup.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"

USING_NS_CC;

CRewardPopupDP* CRewardPopupDP::create(const sCHALLENGE_PARAM challenge)
{
    CRewardPopupDP *pRet = new(std::nothrow) CRewardPopupDP(challenge);
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

bool CRewardPopupDP::init()
{
    if (!CPopup::init()) return false;
    
    this->setCascadeOpacityEnabled(true);
    this->setScale(0.2f);
    this->initReward();

    this->setOpenAnimation([=](Node* sender){

        auto scaleUp    = ScaleTo::create(0.5f, 1.f);
        auto jumpAction = JumpTo::create(1.f, Vec2(0, 0), 50, 1);
        auto spawn      = Spawn::createWithTwoActions(scaleUp, jumpAction);
        
        this->runAction(spawn);
    });
    
    return true;
}

void CRewardPopupDP::initReward()
{
    auto sprite = Sprite::create();
    this->setContentSize(sprite->getContentSize());
    sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprite->setPosition(this->getContentSize() / 2);
    this->addChild(sprite);
}