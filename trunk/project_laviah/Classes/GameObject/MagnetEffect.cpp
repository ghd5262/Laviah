#include "MagnetEffect.h"
#include "../Particle/Particles.h"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/CharacterDataManager.h"
#include "../Scene/GameScene.h"
#include "../GameObject/ObjectManager.h"
#include "../GameObject/Bullet/Bullet.h"

using namespace cocos2d;

CMagnetEffect* CMagnetEffect::create()
{
    CMagnetEffect *pRet = new(std::nothrow) CMagnetEffect();
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

CMagnetEffect::CMagnetEffect()
: m_Particle(nullptr)
, m_Texture(nullptr)
, m_LimitTime(0.f)
, m_Timer(0.f)
, m_IntervalTimer(MAGNET_INTERVAL)
{}

bool CMagnetEffect::init()
{
    if (!Node::init()) return false;
    
    this->scheduleUpdate();
    
    m_Texture = Sprite::create("barrier.png");
    if (m_Texture != nullptr){
        m_Texture->setAnchorPoint(Vec2(0.5f, 0.5f));
        addChild(m_Texture);
    }
    
    m_Texture->setScale(0);
    
    return true;
}

void CMagnetEffect::update(float delta)
{
    if(CObjectManager::Instance()->getIsGamePause()) return;
    
    m_Timer += delta;
    m_IntervalTimer += delta;
    
    if (m_Timer > m_LimitTime) return;
    
    if (m_IntervalTimer > MAGNET_INTERVAL)
    {
        m_Particle = CParticle_Explosion_2::create("fire.png");
        m_Particle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_Particle->setPosition(Vec2::ZERO);
        m_Particle->setStartRadius(m_BoundingRadius);
        m_Particle->setEndRadius(0);
        m_Particle->setDuration(MAGNET_INTERVAL);
        m_Particle->setTotalParticles(10);
        m_Particle->setStartSize(10);
        this->addChild(m_Particle, ZORDER::PLAYER);
        
        auto boundingSizeByPercent = ((m_BoundingRadius * 2.f) / m_Texture->getContentSize().width);
        auto scaleDown = ScaleTo::create(MAGNET_INTERVAL - 0.2f, 0);
        auto ease      = EaseOut::create(scaleDown, 0.3f);
        auto scaleUp   = ScaleTo::create(0, boundingSizeByPercent);
        auto sequence  = Sequence::createWithTwoActions(scaleUp, ease);
        
        m_Texture->runAction(sequence);
        m_IntervalTimer = 0.f;
    }
}

void CMagnetEffect::GotMagnetItem(){
    m_LimitTime      = CUserDataManager::Instance()->getItemValueBySkillIndex(3);
    m_BoundingRadius = CUserDataManager::Instance()->getItemValueBySkillIndex(4);
    m_Timer          = 0.f;
    m_IntervalTimer  = MAGNET_INTERVAL;
}

void CMagnetEffect::Clear()
{
    m_BoundingRadius = (0);
    m_LimitTime      = 0.f;
    this->stopAllActions();
}
