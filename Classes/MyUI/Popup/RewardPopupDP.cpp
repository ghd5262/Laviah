#include "RewardPopupDP.hpp"
#include "../MyButton.h"
#include "../Popup.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/ChallengeDataManager.hpp"
#include "../../DataManager/ChallengeRewarder/ChallengeRewarder.hpp"
#include "../../DataManager/CharacterDataManager.h"
#include "../../GameObject/BulletCreator.h"

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
    this->setContentSize(_director->getWinSize());
    
    
    auto rewardKey      = m_Reward._key;
    auto rewardValue    = m_Reward._value;
    std::string value   = "";
    
    if (CHALLENGE_REWARD_KEY::REWARD_COIN == rewardKey){
        value = StringUtils::format("%d Gold", rewardValue);
        this->goldReward();
    }
    if (CHALLENGE_REWARD_KEY::REWARD_CHARACTER == rewardKey){
        auto data = CCharacterDataManager::Instance()->getCharacterByIndex(rewardValue);
        value = StringUtils::format("%s", data->_name.c_str());
        
    }
    
    // create title
    auto title = Label::createWithSystemFont(value, FONT::MALGUNBD, 80,
                                             Size(this->getContentSize().width * 0.8f,
                                                  this->getContentSize().height),
                                             TextHAlignment::CENTER,
                                             TextVAlignment::CENTER);
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    title->setPosition(Vec2(this->getContentSize().width * 0.5f,
                            this->getContentSize().height * 0.8f));
    this->addChild(title);

    
    this->setOpenAnimation([=](Node* sender){

        auto action = [=](Node* owner){
            auto fade  = FadeIn::create(0.5f);
            owner->setOpacity(0);
            owner->runAction(fade);
        };
        
        action(title);
    });
    
    this->setCloseAnimation([=](Node* sender){
        
        title->runAction(FadeTo::create(0.3f, 0));
    });
    return true;
}

void CRewardPopupDP::goldReward()
{
    auto action = [=](CBullet* sender){
        auto layerSize = this->getContentSize();
        sender->setPosition(layerSize.width * 0.5f,
                            layerSize.height * 0.3f);
        sender->setRotation(random<float>(0.f, 360.f));
        
        auto moveAction = MoveTo::create(0.5f, CBullet::getSquarePosition(random<int>(30, 330),
                                                                          random<int>(50 , layerSize.height - 50)));
        auto easeAction = EaseExponentialInOut::create(moveAction);
        auto callFunc   = CallFunc::create([=](){
            sender->R_UpAndBezier();
        });
        auto sequence   = Sequence::createWithTwoActions(easeAction, callFunc);
        sender->runAction(sequence);
    };
    
    auto limit = std::min(50, m_Reward._value);
    for(int count = 0; count < limit; count++)
    {
        auto gold = CBulletCreator::CreateBullet('U', 1, 1);
        gold->setLocalZOrder(ZORDER::POPUP);
        action(gold);
    }
}

void CRewardPopupDP::characterReward()
{
    
}