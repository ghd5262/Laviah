#include "RewardPopupDP.hpp"
#include "../MyButton.h"
#include "../Popup.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/AchievementDataManager.hpp"
#include "../../DataManager/AchievementRewarder/AchievementRewarder.hpp"
#include "../../DataManager/CharacterDataManager.h"
#include "../../GameObject/BulletCreator.h"
#include "../../Particle/Particles.h"

USING_NS_CC;

CRewardPopupDP* CRewardPopupDP::create()
{
	CRewardPopupDP *pRet = new(std::nothrow) CRewardPopupDP();
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

CPopup* CRewardPopupDP::show(cocos2d::Node* parent/* = nullptr*/, int zOrder/* = 0*/)
{
    //m_TouchDisable = CMyButton::create()
    //->addEventListener([](Node* sender){})
    //->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    //->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080, 1920))
    //->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    //->setButtonPosition(this->getContentSize() / 2)
    //->show(this, -1);
    
    this->setDefaultCallback([=](Node* sender){}, false);
    return CPopup::show(parent, zOrder);
}

CRewardPopupDP* CRewardPopupDP::setExitCallback(std::function<void()> listener)
{
    m_ExitListener = listener;
    return this;
}

void CRewardPopupDP::exitWithDelay(float delay)
{
    this->scheduleOnce([=](float delay){
        if(m_ExitListener){
            this->retain();
            m_ExitListener();
            m_ExitListener = nullptr;
            this->release();
        }
    }, delay, "AutoExit");
}

CRewardPopupDP* CRewardPopupDP::setRewardData(sREWARD_DATA reward)
{
    m_Reward = reward;
    return this;
}

void CRewardPopupDP::Open()
{
    auto rewardKey    = m_Reward._key;
    auto rewardValue  = m_Reward._value;
    std::string value = "";
    
    if (ACHIEVEMENT_REWARD_KEY::REWARD_COIN == rewardKey){
        value = StringUtils::format("%d Gold", rewardValue);
        this->goldReward();
    }
    if (ACHIEVEMENT_REWARD_KEY::REWARD_CHARACTER == rewardKey){
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
    title->setCascadeOpacityEnabled(true);
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    title->setPosition(Vec2(this->getContentSize().width * 0.5f,
                            this->getContentSize().height * 0.3f));
    this->addChild(title);
    //this->scheduleOnce([=](float delay){
    //    m_TouchDisable->setVisible(false);
    //}, 3.f, "skipEnable");
}

void CRewardPopupDP::goldReward()
{
    auto action = [=](Node* sender){
        auto layerSize   = this->getContentSize();
        sender->setPosition(CBullet::getCirclePosition(random<float>(0.f, 360.f), 1500, layerSize / 2));
        sender->setRotation(random<float>(0.f, 360.f));
        
        auto targetPos   = CBullet::getSquarePosition(random<int>(30, 330), random<int>(100, 1500));
        auto moveAction  = MoveTo::create(1.5f, targetPos);
        auto easeAction  = EaseExponentialInOut::create(moveAction);
        auto delayAction = DelayTime::create(random<float>(1.f, 3.f));
        auto callFunc    = CallFunc::create([=](){
            
            // create action
            auto upActin      = MoveTo::create(0.6f, Vec2(sender->getPositionX(), sender->getPositionY() + 150.f));
            auto sineAction   = EaseExponentialOut::create(upActin);
            auto delayAction  = DelayTime::create(0.5f);
            
            auto seqAction    = Sequence::create(sineAction,
                                                 delayAction,
                                                 CallFunc::create([=](){
                
                // create bezier action config
                auto targetPos   = Vec2(layerSize.width * 0.055f, layerSize.height * 0.925f);
                auto length      = Vec2(targetPos - sender->getPosition()).length();
                auto cPos1       = Vec2(sender->getPosition().x - (length * 0.3f),
                                        sender->getPosition().y - 100.f);
                auto cPos2       = Vec2(targetPos.x, targetPos.y - (length * 0.3f));
                auto time        = std::max<float>(0.5f, (length / layerSize.height) * 1.3f);
                auto scale       = 3.f;
                
                // create bezier action
                ccBezierConfig bezier;
                bezier.controlPoint_1 = Vec2(cPos1);
                bezier.controlPoint_2 = Vec2(cPos2);
                bezier.endPosition    = Vec2(targetPos);
                
                auto bezierAction = BezierTo::create(time, bezier);
                auto exponential  = EaseSineIn::create(bezierAction);
                auto scaleAction  = ScaleTo::create(0.4f, scale);
                auto fadeAction   = FadeTo::create(0.4f, 1);
                auto spawn        = Spawn::create(scaleAction, fadeAction, nullptr);
                
                sender->runAction(Sequence::create(exponential,
                                                   spawn,
                                                   CallFunc::create([=](){
                    sender->removeFromParent();
                }), nullptr));
                
            }), nullptr);
            
            sender->runAction(seqAction);
        });
        auto sequence    = Sequence::create(easeAction, delayAction, callFunc, nullptr);
        sender->runAction(sequence);
    };
    
    auto limit = std::min(60, m_Reward._value);
    for(int count = 0; count < limit; count++)
    {
        auto gold = Sprite::createWithSpriteFrameName("coin_1.png");
        gold->setColor(COLOR::GOLD);
        this->addChild(gold);
        action(gold);
    }
    
    this->exitWithDelay(7.f);
}

void CRewardPopupDP::characterReward()
{
    // create character
    auto data       = CCharacterDataManager::Instance()->getCharacterByIndex(m_Reward._value);
    auto character  = Sprite::createWithSpriteFrameName(data->_normalTextureName);
    character->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    character->setPosition(this->getContentSize() / 2);
    character->setScale(2.5f);
    character->setCascadeOpacityEnabled(true);
    this->addChild(character);
    
    switch (random<int>(1, 2)) {
        case 1: this->characterCreator1(character); break;
        case 2: this->characterCreator2(character); break;
    }
}

void CRewardPopupDP::characterCreator1(cocos2d::Sprite* character)
{
    character->setOpacity(0);

    auto delay      = DelayTime::create(2.f);
    auto fadeIn     = FadeIn::create(1.f);
    auto sequence   = Sequence::create(delay, fadeIn, nullptr);
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
        particle->setTotalParticles(100);
        particle->setAngle(random<int>(0, 360));
        this->addChild(particle);
    }
    
    this->exitWithDelay(4.f);
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
        
        auto moveOut   = MoveTo::create(3.f, Vec2(this->getContentSize().width * 1.5f,
                                                 this->getContentSize().height * 0.5f));
        auto spinOut   = RotateBy::create(3.f, 360);
        auto spawnOut  = Spawn::createWithTwoActions(moveOut, spinOut);
        auto elastic   = EaseElasticIn::create(spawnOut, 1);
        
        auto delayIn   = DelayTime::create(3.f);
        
        auto moveIn    = MoveTo::create(5.f, this->getContentSize() / 2);
        auto spinIn    = RotateBy::create(5.f, -360);
        auto spawnIn   = Spawn::createWithTwoActions(moveIn, spinIn);
        auto elasticIn = EaseElasticOut::create(spawnIn, 1);
        
        character->runAction(Sequence::create(elastic, delayIn, elasticIn, nullptr));
    });
    
    character->runAction(Sequence::create(elastic, delay, function, NULL));
    this->exitWithDelay(6.5f);
}

void CRewardPopupDP::characterCreator3(cocos2d::Sprite* character)
{
    
}
