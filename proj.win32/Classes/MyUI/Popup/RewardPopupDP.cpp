#include "RewardPopupDP.hpp"
#include "../MyButton.h"
#include "../Popup.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/ChallengeDataManager.hpp"

USING_NS_CC;

CRewardPopupDP* CRewardPopupDP::create(const sREWARD_DATA reward)
{
	CRewardPopupDP *pRet = new(std::nothrow) CRewardPopupDP(reward);
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

        auto scaleUp    = ScaleTo::create(0.3f, 1.f);
		auto jumpAction = JumpBy::create(0.3f, Vec2(0, 0), 150, 1);
        auto spawn      = Spawn::createWithTwoActions(scaleUp, jumpAction);
        
        this->runAction(spawn);
    });
    
    return true;
}

void CRewardPopupDP::initReward()
{
	auto name = CChallengeDataManager::Instance()->getRewardImageName(m_Reward._key, m_Reward._value);
	auto sprite = Sprite::create(name);
	if (sprite == nullptr)
		sprite = Sprite::createWithSpriteFrameName(name);

    this->setContentSize(sprite->getContentSize());
    sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprite->setPosition(this->getContentSize() / 2);
    this->addChild(sprite);
}