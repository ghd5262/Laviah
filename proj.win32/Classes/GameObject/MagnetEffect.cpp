#include "MagnetEffect.h"
#include "../Particle/Particles.h"

CMagnetEffect* CMagnetEffect::create(std::string textureName, float boundingRadius, float limitTime)
{
    CMagnetEffect *pRet = new(std::nothrow) CMagnetEffect(textureName, boundingRadius, limitTime);
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

CMagnetEffect::CMagnetEffect(std::string textureName, float boundingRadius, float limitTime)
: m_TextureName(textureName)
, m_OriginBoundingRadius(boundingRadius)
, m_bMagnetAlive(false)
, m_limitTime(limitTime)
, m_Timer(0.f)
, m_IntervalTimer(MAGNET_INTERVAL)
, m_BoundingSizeByPercent(0.f)
{
}

bool CMagnetEffect::init()
{
    if (!initVariable())
        return false;
    return true;
}

bool CMagnetEffect::initVariable()
{
    try{
        scheduleUpdate();
        
        m_pTexture = Sprite::create(m_TextureName);
        if (m_pTexture != nullptr){
            m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
            addChild(m_pTexture);
        }
        
        m_BoundingSizeByPercent = m_OriginBoundingRadius/m_pTexture->getContentSize().width;
        m_pTexture->setScale(0);
    }
    catch (...){
        CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
        assert(false);
        return false;
    }
    return true;
}

void CMagnetEffect::Execute(float delta)
{
    if (m_bMagnetAlive)
    {
        m_Timer += delta;
        m_IntervalTimer += delta;
        
        if(m_Timer > m_limitTime)
            FinishedBarrierItem();
        
        if(m_IntervalTimer > MAGNET_INTERVAL)
        {
            m_pTexture->runAction(Sequence::create(ScaleTo::create(0, m_BoundingSizeByPercent), EaseOut::create( ScaleTo::create(MAGNET_INTERVAL - 0.2f, 0), 0.3f), NULL));
            m_IntervalTimer = 0.f;
        }
    }
}


void CMagnetEffect::GotMagnetItem(){
    m_Timer = 0.f;
    m_IntervalTimer = MAGNET_INTERVAL;
    m_bMagnetAlive = true;
    m_pParticle = CParticle_Explosion_2::create("whiteSquare.png");
    if (m_pParticle != nullptr){
        m_pParticle->retain();
        m_pParticle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_pParticle->setPosition(getPosition());
        m_pParticle->setStartRadius(m_OriginBoundingRadius);
        m_pParticle->setEndRadius(0);
        m_pParticle->setDuration(m_limitTime);
        m_pParticle->setTotalParticles(30);
        m_pParticle->setSpeed(100);
        m_pParticle->setSpeedVar(100);
        m_pParticle->setStartSize(10);
        addChild(m_pParticle, 101);
    }
}


