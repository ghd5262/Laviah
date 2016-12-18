#include "MagnetEffect.h"
#include "../Particle/Particles.h"

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
: m_OriginBoundingRadius(0.f)
, m_bMagnetAlive(false)
, m_LimitTime(0.f)
, m_Timer(0.f)
, m_IntervalTimer(MAGNET_INTERVAL)
{
}

bool CMagnetEffect::init()
{
    if (!Node::init()) return false;
    
    scheduleUpdate();
    
    m_pTexture = Sprite::create("barrier.png");
    if (m_pTexture != nullptr){
        m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
        addChild(m_pTexture);
    }
    
    m_pTexture->setScale(0);
    
    return true;
}

void CMagnetEffect::Execute(float delta)
{
    if (m_bMagnetAlive)
    {
        m_Timer += delta;
        m_IntervalTimer += delta;
        
		if (m_Timer > m_LimitTime)
            FinishedBarrierItem();
        
        if (m_IntervalTimer > MAGNET_INTERVAL)
        {
			m_pParticle = CParticle_Explosion_2::create("fire.png");
			if (m_pParticle != nullptr){
				m_pParticle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				m_pParticle->setPosition(Vec2::ZERO);
				m_pParticle->setStartRadius(m_OriginBoundingRadius);
				m_pParticle->setEndRadius(0);
				m_pParticle->setDuration(MAGNET_INTERVAL);
				m_pParticle->setTotalParticles(10);
				m_pParticle->setStartSize(10);
				addChild(m_pParticle, 101);
			}
			auto boundingSizeByPercent = (m_OriginBoundingRadius / m_pTexture->getContentSize().width) * 2;
			m_pTexture->runAction(Sequence::create(
				ScaleTo::create(0, boundingSizeByPercent), 
				EaseOut::create(
				ScaleTo::create(MAGNET_INTERVAL - 0.2f, 0), 0.3f), NULL));
            m_IntervalTimer = 0.f;
        }
    }
}

void CMagnetEffect::GotMagnetItem(){
    m_Timer = 0.f;
    m_IntervalTimer = MAGNET_INTERVAL;
    m_bMagnetAlive = true;
}


