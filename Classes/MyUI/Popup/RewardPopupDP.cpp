#include "RewardPopupDP.hpp"
#include "../MyButton.h"
#include "../Popup.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/ChallengeDataManager.hpp"
#include "../../DataManager/ChallengeRewarder/ChallengeRewarder.hpp"
#include "../../DataManager/CharacterDataManager.h"
#include "../../GameObject/BulletCreator.h"
#include "../../Particle/Particles.h"

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
        value = TRANSLATE(data->_name);
        this->characterReward();
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
        auto layerSize   = this->getContentSize();
        sender->setPosition(layerSize.width * 0.5f,
                            layerSize.height * 0.3f);
        sender->setRotation(random<float>(0.f, 360.f));
        
        auto targetPos   = CBullet::getSquarePosition(random<int>(30, 330), random<int>(2000 , 3700));
        auto moveAction  = MoveTo::create(1.5f, targetPos);
        auto easeAction  = EaseExponentialInOut::create(moveAction);
        auto delayAction = DelayTime::create(random<float>(1.f, 3.f));
        auto callFunc    = CallFunc::create([=](){
            sender->R_UpAndBezier();
        });
        auto sequence    = Sequence::create(easeAction, delayAction, callFunc, nullptr);
        sender->runAction(sequence);
    };
    
    auto limit = std::min(80, m_Reward._value);
    for(int count = 0; count < limit; count++)
    {
        auto gold = CBulletCreator::CreateBullet('U', 1, 1, false);
        gold->setLocalZOrder(ZORDER::POPUP);
        action(gold);
    }
}

void CRewardPopupDP::characterReward()
{
    // create character
    auto data       = CCharacterDataManager::Instance()->getCharacterByIndex(m_Reward._value);
    auto character  = Sprite::createWithSpriteFrameName(data->_normalTextureName);
    character->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    character->setPosition(this->getContentSize() / 2);
    character->setScale(2.5f);
    this->addChild(character);
    
    switch (random<int>(1, 2)) {
        case 1: this->characterCreator1(character); break;
        case 2: this->characterCreator2(character); break;
    }
}

void CRewardPopupDP::characterCreator1(cocos2d::Sprite* character)
{
    character->setColor(Color3B::BLACK);
    character->setOpacity(0);

    auto delay      = DelayTime::create(2.f);
    auto fadeIn     = FadeIn::create(1.f);
    auto tint       = TintTo::create(1.f, Color3B::WHITE);
    auto sequence   = Sequence::create(delay, fadeIn, tint, nullptr);
    character->runAction(sequence);
    
    auto particle   = CParticle_Explosion_2::create("helpButton.png");
    if (particle != nullptr){
        particle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        particle->setPosition(this->getContentSize() / 2);
        particle->setStartRadius(250);
        particle->setEndRadius(0);
        particle->setDuration(2.5f);
        particle->setStartSize(80);
        particle->setStartSizeVar(70);
        particle->setEndSize(0);
        particle->setSpeed(200);
        particle->setTotalParticles(100);
        particle->setAngle(random<int>(0, 360));
        particle->setStartColor(Color4F::BLACK);
        particle->setEndColor(Color4F::BLACK);
        this->addChild(particle);
    }
}

void CRewardPopupDP::characterCreator2(cocos2d::Sprite* character)
{    
    character->setPosition(Vec2(-this->getContentSize().width * 0.5f,
                                this->getContentSize().height * 0.5f));
    auto move    = MoveTo::create(3.f, this->getContentSize() / 2);
    auto spin    = RotateBy::create(3.f, 360);
    auto spawn   = Spawn::createWithTwoActions(move, spin);
    auto elastic = EaseElasticOut::create(spawn, 1);
    auto delay   = DelayTime::create(0.5f);
    auto function= CallFunc::create([=](){
        
        auto moveOut  = MoveTo::create(3.f, Vec2(this->getContentSize().width * 1.5f,
                                              this->getContentSize().height * 0.5f));
        auto spinOut  = RotateBy::create(3.f, 360);
        auto spawnOut = Spawn::createWithTwoActions(moveOut, spinOut);
        auto elastic  = EaseElasticIn::create(spawnOut, 1);
        
        character->runAction(elastic);
    });
    
    character->runAction(Sequence::create(elastic, delay, function, NULL));
}

void CRewardPopupDP::characterCreator3(cocos2d::Sprite* character)
{
    
}
