#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class CBackGround : public cocos2d::Node {
public:
    static CBackGround* create();
    void BonusTimeStart();
    void BonusTimeEnd();
    
    virtual void update(float delta) override;

    CC_SYNTHESIZE(bool, m_IsBonusTime, IsBonusTime);
    
protected:
    virtual bool init() override;

private:
    void changeBackground();
    void createBackParticleNormal();
    void createBottomParticleNormal();
    void createBackParticleBonusTime();
    void particlePause(ParticleSystemQuad* particle);
    void particleResume(ParticleSystemQuad* particle);

    CBackGround()
    : m_BackParticle(nullptr)
    , m_BottomParticle(nullptr)
    , m_BonusTimeParticle(nullptr)
    , m_Gradient(nullptr)
    , m_tempStartGradient(nullptr)
    , m_tempEndGradient(nullptr)
	, m_Index(nullptr)
    , m_Time(0.f)
    , m_fLimitTime(30.f)
    , m_IsBonusTime(false)
    , m_IsChanging(false){}

    virtual ~CBackGround(){}
    
private:
    ParticleSystemQuad* m_BackParticle;
    ParticleSystemQuad* m_BottomParticle;
    ParticleSystemQuad* m_BonusTimeParticle;
    LayerGradient* m_Gradient;
	Label* m_Index;
    Sprite* m_tempStartGradient;
    Sprite* m_tempEndGradient;
    float m_Time;
    float m_fLimitTime;
    bool m_IsChanging;
};