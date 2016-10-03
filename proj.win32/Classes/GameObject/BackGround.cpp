#include "BackGround.h"
#include "../DataManager/GradientDataManager.h"
#include "../Particle/Particles.h"
#include "../Scene/GameScene.h"

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

CBackGround::CBackGround()
: CGameObject(0)
, m_pParticleBG1(nullptr)
, m_pParticleBG2(nullptr)
, m_Gradient(nullptr)
, m_tempStartGradient(nullptr)
, m_tempEndGradient(nullptr)
, m_fTime(0.f)
, m_fLimitTime(15.f)
{
}

bool CBackGround::init()
{
    if (!initVariable())
        return false;
    return true;
}

bool CBackGround::initVariable()
{
    try{
        
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

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

        m_pParticleBG1 = CParticle_BackGround::create("whiteSquare.png");
        if (m_pParticleBG1 != nullptr){
            m_pParticleBG1->retain();
            m_pParticleBG1->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.75f));
            m_pParticleBG1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_pParticleBG1->setTotalParticles(15);
            addChild(m_pParticleBG1);
        }
        
        m_pParticleBG2 = CParticle_BackGround::create("whiteSquare.png");
        if (m_pParticleBG2 != nullptr){
            m_pParticleBG2->retain();
            m_pParticleBG2->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * -0.05f));
            m_pParticleBG2->setPosVar(Vec2(visibleSize.width, 0));
            m_pParticleBG2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_pParticleBG2->setStartSize(60.f);
			m_pParticleBG2->setLife(3.f);
			m_pParticleBG2->setLifeVar(2.f);
			m_pParticleBG2->setSpeed(80);
			m_pParticleBG2->setSpeedVar(30);
			m_pParticleBG2->setAngleVar(0);
			
            addChild(m_pParticleBG2);
        }
    }
    catch (...){
        CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
        assert(false);
        return false;
    }
    return true;
}

void CBackGround::ChangeBackground()
{
	if (m_tempStartGradient != nullptr && m_tempEndGradient != nullptr){
		auto gradientTo = CGradientDataManager::Instance()->getRandomGradient();
		m_tempStartGradient->runAction(TintTo::create(1.5f, gradientTo._rightColor));
		m_tempEndGradient->runAction(TintTo::create(1.5f, gradientTo._leftColor));
	}
}

void CBackGround::setParticlePause(bool isPause)
{
	if (m_pParticleBG2 != nullptr){
		if (isPause)
			m_pParticleBG2->pause();
		else
			m_pParticleBG2->release();
	}
}

void CBackGround::Execute(float delta)
{
	m_fTime += delta;
	if (m_fTime > m_fLimitTime)
	{
		ChangeBackground();
		m_fTime = 0.f;
	}

	if (m_Gradient != nullptr){
		m_Gradient->setStartColor(m_tempStartGradient->getColor());
		m_Gradient->setEndColor(m_tempEndGradient->getColor());
	}
}