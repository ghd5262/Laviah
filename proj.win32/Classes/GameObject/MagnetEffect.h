#pragma once

#include "GameObject.h"


class CMagnetEffect : public CGameObject {
    const float MAGNET_INTERVAL = 1.f;
    
public:
    static CMagnetEffect* create(std::string textureName, float boundingRadius, float limitTime);
    
    void GotMagnetItem();
    virtual void Execute(float delta) override;
    
protected:
    virtual bool init() override;
    virtual bool initVariable() override;
    
    //getter & setter
    CC_SYNTHESIZE(bool, m_bMagnetAlive, MagnetAlive);
    
private:
    inline void FinishedBarrierItem()
    {
        m_bMagnetAlive = false;
    }
    CMagnetEffect(std::string textureName, float boundingRadius, float limitTime);
    virtual ~CMagnetEffect(){}
    
private:
    std::string m_TextureName;
    cocos2d::Sprite* m_pTexture;
    float m_OriginBoundingRadius;
    float m_BoundingSizeByPercent;
    float m_limitTime;
    float m_Timer;
    float m_IntervalTimer;
    cocos2d::ParticleSystemQuad* m_pParticle;
};