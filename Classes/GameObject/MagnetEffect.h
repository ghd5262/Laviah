#pragma once

#include "GameObject.h"


class CMagnetEffect : public CGameObject {
    const float MAGNET_INTERVAL = 1.f;
    
public:
    static CMagnetEffect* create();
    
    void GotMagnetItem();
    virtual void update(float delta) override;
    
protected:
    virtual bool init() override;
    
private:
    CMagnetEffect();
    virtual ~CMagnetEffect(){}
    
private:
    cocos2d::ParticleSystemQuad* m_Particle;
    cocos2d::Sprite* m_Texture;
    float m_Timer;
    float m_IntervalTimer;
    float m_LimitTime;
};
