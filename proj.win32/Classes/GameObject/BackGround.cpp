#include "BackGround.h"
#include "../DataManager/GradientDataManager.h"
#include "../Particle/Particles.h"
#include "../Scene/GameScene.h"
#include "../GameObject/ObjectManager.h"

CBackGround* CBackGround::create()
{
    CBackGround *pRet = new(std::nothrow) CBackGround();
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

bool CBackGround::init()
{
    if (!Node::init()) return false;
    
    this->scheduleUpdate();
    this->setContentSize(_director->getVisibleSize());
    
    m_Gradient = LayerGradient::create();
    if (m_Gradient != nullptr)
        addChild(m_Gradient);
    
    auto gradientTo = CGradientDataManager::Instance()->getRandomGradient();
    
    m_tempStartGradient = Sprite::create();
    if (m_tempStartGradient != nullptr){
        addChild(m_tempStartGradient);
        m_tempStartGradient->runAction(TintTo::create(1.5f, gradientTo._rightColor));
        m_Gradient->setStartColor(gradientTo._rightColor);
    }
    
    m_tempEndGradient = Sprite::create();
    if (m_tempEndGradient != nullptr){
        addChild(m_tempEndGradient);
        m_tempEndGradient->runAction(TintTo::create(1.5f, gradientTo._leftColor));
        m_Gradient->setEndColor(gradientTo._leftColor);
    }
    
    this->createBackParticleBonusTime();
    this->createBackParticleNormal();
    this->createBottomParticleNormal();
    

    //m_Index = Label::createWithTTF(StringUtils::format("%d", CGradientDataManager::Instance()->getGradientIdx()), FONT::MALGUNBD, 60);
    //m_Index->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.65f));
    //m_Index->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    //m_Index->setTextColor(COLOR::WHITEGRAY_ALPHA);
    //addChild(m_Index);
    
    return true;
}

void CBackGround::update(float delta)
{
    if(CObjectManager::Instance()->getIsGamePause()) return;
    
    m_Time += delta;
    if (m_Time > m_fLimitTime)
    {
        changeBackground();
        m_Time = 0.f;
    }
    
    if (m_IsChangying && m_Gradient != nullptr){
        m_Gradient->setStartColor(m_tempStartGradient->getColor());
        m_Gradient->setEndColor(m_tempEndGradient->getColor());
    }
}

void CBackGround::BonusTimeStart()
{
    particleResume(m_BonusTimeParticle);
    particlePause(m_BottomParticle);
    particlePause(m_BackParticle);
}

void CBackGround::BonusTimeEnd()
{
    particlePause(m_BonusTimeParticle);
    particleResume(m_BottomParticle);
    particleResume(m_BackParticle);
}

void CBackGround::changeBackground()
{
    if (m_tempStartGradient == nullptr || m_tempEndGradient == nullptr) return;
    
    m_IsChangying = true;
    auto gradientTo = CGradientDataManager::Instance()->getRandomGradient();
    
    auto data = sGRADIENT_PARAM(gradientTo._idx, gradientTo._leftColor, gradientTo._rightColor);
    if (random<int>(0, 1)) //flip
    {
        data._leftColor = gradientTo._rightColor;
        data._rightColor = gradientTo._leftColor;
    }
    
    m_tempStartGradient->runAction(TintTo::create(1.5f, data._rightColor));
    m_tempEndGradient->runAction(Sequence::create(TintTo::create(1.5f, data._leftColor),
                                                  CallFunc::create([=](){
        m_IsChangying = false;
    }), nullptr));
    
    //m_Index->setString(StringUtils::format("%d", CGradientDataManager::Instance()->getGradientIdx()));
}

void CBackGround::createBackParticleNormal()
{
    m_BackParticle = CParticle_BackGround::create("whiteSquare.png");
    if (m_BackParticle != nullptr){
        m_BackParticle->setPosition(Vec2(this->getContentSize().width * 0.5f,
                                         this->getContentSize().height * 0.75f));
        m_BackParticle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_BackParticle->setTotalParticles(15);
        addChild(m_BackParticle);
    }
}

void CBackGround::createBottomParticleNormal()
{
    m_BottomParticle = CParticle_BackGround::create("whiteSquare.png");
    if (m_BottomParticle != nullptr){
        m_BottomParticle->setPosition(Vec2(this->getContentSize().width * 0.5f,
                                           this->getContentSize().height * -0.05f));
        m_BottomParticle->setPosVar(Vec2(this->getContentSize().width, 0));
        m_BottomParticle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_BottomParticle->setStartSize(60.f);
        m_BottomParticle->setLife(3.f);
        m_BottomParticle->setLifeVar(2.f);
        m_BottomParticle->setSpeed(80);
        m_BottomParticle->setSpeedVar(30);
        m_BottomParticle->setAngleVar(0);
        
        addChild(m_BottomParticle);
    }
}

void CBackGround::createBackParticleBonusTime()
{
    m_BonusTimeParticle = CParticle_BackGround::create("whiteSquare.png");
    if (m_BonusTimeParticle != nullptr){
        m_BonusTimeParticle->setPosition(Vec2(this->getContentSize().width * 0.5f,
                                         this->getContentSize().height * 0.75f));
        m_BonusTimeParticle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_BonusTimeParticle->setStartSize(30.f);
        m_BonusTimeParticle->setLife(3.f);
        m_BonusTimeParticle->setLifeVar(2.f);
        m_BonusTimeParticle->setSpeed(300);
        m_BonusTimeParticle->setAngle(0);
        m_BonusTimeParticle->setAngleVar(0);
        m_BonusTimeParticle->setStartSpin(0);
        m_BonusTimeParticle->setStartSpinVar(0);
        m_BonusTimeParticle->setEndSpin(0);
        m_BonusTimeParticle->setEndSpinVar(0);
        m_BonusTimeParticle->setGravity(Vec2(-90, 0));
        
        addChild(m_BonusTimeParticle);
    }
    this->particlePause(m_BonusTimeParticle);
}

void CBackGround::particlePause(ParticleSystemQuad* particle)
{
    if (particle != nullptr)
        particle->pauseEmissions();
}

void CBackGround::particleResume(ParticleSystemQuad* particle)
{
    if (particle != nullptr)
        particle->resumeEmissions();
}