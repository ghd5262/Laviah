#include "HealthBarUI.h"


CHealthBarUI* CHealthBarUI::create(HealthCalculatorFunc healthCalFunc)
{
	CHealthBarUI *pRet = new(std::nothrow) CHealthBarUI(healthCalFunc);
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

bool CHealthBarUI::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CHealthBarUI::initVariable()
{
	try{
		Size visibleSize = Director::getInstance()->getVisibleSize();

		m_HealthBarImg = Sprite::create("healthBar2.png");
		if (m_HealthBarImg != nullptr){
			m_HealthBar = ProgressTimer::create(m_HealthBarImg);
			
			if (m_HealthBar != nullptr){
				m_HealthBar->setType(ProgressTimer::Type::BAR);
				m_HealthBar->setMidpoint(Vec2(0, 0));
				m_HealthBar->setBarChangeRate(Vec2(1, 0));
				this->addChild(m_HealthBar);
				auto HealthBarUIAction = ProgressFromTo::create(100, 100, 0);
				m_HealthBar->runAction(HealthBarUIAction);
				
			}
		}
	
		m_HealthBarBG = Sprite::create("healthBarBG2.png");
		if (m_HealthBarBG != nullptr)
		{
			m_HealthBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			this->addChild(m_HealthBarBG);
		}
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CHealthBarUI::Execute(float delta)
{
	UpdateHealthUI(delta);
}

void CHealthBarUI::ChangeHealthCalFunc(HealthCalculatorFunc healthCalFunc)
{
	m_HealthCalculatorFunction = healthCalFunc;
}

void CHealthBarUI::UpdateHealthUI(float delta)
{
	float percent = m_HealthCalculatorFunction(delta);
	m_HealthBar->setPercentage(percent);
}

