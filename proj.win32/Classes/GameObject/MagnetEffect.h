#pragma once
#include "GameObject.h"

class CMagnetEffect : public CGameObject{
public:
    static CMagnetEffect* create(std::string textureName, float boundingRadius, float limitTime);
    inline void GotMagnetItem(){
        m_pTexture->setVisible(true);
        m_bMagnetAlive = true;
        
        auto action = Sequence::create(ScaleTo::create(BARRIER_TIMER, BARRIER_SIZE),
                                       CallFunc::create([&](){
            FinishedBarrierItem();
        }), nullptr);
        m_pTexture->runAction(action);
    }
    virtual void Execute(float delta = 0.f) override;
    
protected:
    virtual bool init() override;
    virtual bool initVariable() override;
    
    //getter & setter
    CC_SYNTHESIZE(bool, m_bMagnetAlive, MagnetAlive);
    
private:
    inline void FinishedBarrierItem()
    {
        m_pTexture->setScale(0.f);
        m_pTexture->setVisible(false);
        setBRadius(0);
        m_bMagnetAlive = false;
    }
    CMagnetEffect(std::string textureName, float boundingRadius);
    virtual ~CMagnetEffect(){}
    
private:
    float m_OriginBoundingRadius;
    std::string m_TextureName;
    Sprite* m_pTexture;
    // ParticleSystemQuad* m_pParticle;
};