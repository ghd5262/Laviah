#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class CBackGround : public cocos2d::Node {
public:
    static CBackGround* create();

	void ChangeBackground();
	void setParticlePause(bool isPause);
    
    virtual void update(float delta = 0.f) override;

protected:
    virtual bool init() override;

private:
    CBackGround()
    : m_pParticleBG1(nullptr)
    , m_pParticleBG2(nullptr)
    , m_Gradient(nullptr)
    , m_tempStartGradient(nullptr)
    , m_tempEndGradient(nullptr)
    , m_Time(0.f)
    , m_fLimitTime(20.f){}

    virtual ~CBackGround(){}
    
private:
    ParticleSystemQuad* m_pParticleBG1;
    ParticleSystemQuad* m_pParticleBG2;
    LayerGradient* m_Gradient;
    Sprite* m_tempStartGradient;
    Sprite* m_tempEndGradient;
    float m_Time;
    float m_fLimitTime;
};