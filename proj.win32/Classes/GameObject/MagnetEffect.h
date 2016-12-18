#pragma once

#include "GameObject.h"


class CMagnetEffect : public CGameObject {
    const float MAGNET_INTERVAL = 1.f;
    
public:
    static CMagnetEffect* create();
    
    void GotMagnetItem();
    virtual void Execute(float delta) override;
    
protected:
    virtual bool init() override;
    
    //getter & setter
    CC_SYNTHESIZE(bool, m_bMagnetAlive, MagnetAlive);
	CC_SYNTHESIZE(float, m_LimitTime, LimitTime);
	CC_SYNTHESIZE(float, m_OriginBoundingRadius, OriginBoundingRadius);
private:
    inline void FinishedBarrierItem()
    {
        m_bMagnetAlive = false;
    }
    CMagnetEffect();
    virtual ~CMagnetEffect(){}
    
private:
    cocos2d::Sprite* m_pTexture;
    float m_Timer;
    float m_IntervalTimer;
    cocos2d::ParticleSystemQuad* m_pParticle;
};