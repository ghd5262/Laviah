#pragma once
#include "GameObject.h"

using namespace cocos2d;

class CBackGround : public CGameObject {
public:
    static CBackGround* create();
    
    virtual void Execute(float delta = 0.f) override;

protected:
    virtual bool init() override;
    virtual bool initVariable() override;

private:
    CBackGround();
    virtual ~CBackGround(){}
    
private:
    ParticleSystemQuad* m_pParticleBG1;
    ParticleSystemQuad* m_pParticleBG2;
    LayerGradient* m_Gradient;
    Sprite* m_tempStartGradient;
    Sprite* m_tempEndGradient;
    float m_fTime;
    float m_fLimitTime;
};