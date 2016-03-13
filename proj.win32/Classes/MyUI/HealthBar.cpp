#include "HealthBar.h"


CHealthBar* CHealthBar::create(float maxLife)
{
	CHealthBar *pRet = new(std::nothrow) CHealthBar(maxLife);
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

bool CHealthBar::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CHealthBar::initVariable()
{
	try{
		Size visibleSize = Director::getInstance()->getVisibleSize();

		m_HealthBarImg = Sprite::create("healthBar.png");
		if (m_HealthBarImg != nullptr){
			m_HealthBar = ProgressTimer::create(m_HealthBarImg);
			
			if (m_HealthBar != nullptr){
				m_HealthBar->setType(ProgressTimer::Type::BAR);
				m_HealthBar->setMidpoint(Vec2(0, 0));
				m_HealthBar->setBarChangeRate(Vec2(1, 0));
				this->addChild(m_HealthBar);
				auto healthBarAction = ProgressFromTo::create(m_fMaxLife, 100, 0);
				m_HealthBar->runAction(healthBarAction);
				
			}
		}
	
		m_Health = Sprite::create("health.png");
		if (m_Health != nullptr)
		{
			m_Health->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_Health->setPosition(Vec2(m_HealthBarImg->getContentSize().width * 0.5f, 0));
			this->addChild(m_Health);
		}
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CHealthBar::Hit(float life){
	if (m_fLife - life <= 0){
		m_fLife = 0;
		m_bIsAlive = false;
		m_HealthBar->setPercentage(0);
		m_Health->setPosition(Vec2(- m_HealthBarImg->getContentSize().width * 0.5f, 0));
	}
	else{
		m_fLife -= life;
		float percent = (m_fLife / m_fMaxLife) * 100;
		m_HealthBar->setPercentage(percent);

		m_Health->setPosition(Vec2(												// 하트모양 position
			(m_HealthBarImg->getContentSize().width * (percent * 0.01f))		// x = bar크기 * (percent * 0.01) - bar앵커 포인트가 0.5니까 그만큼뺀다.
			- m_HealthBarImg->getContentSize().width * 0.5f, 0));				// 이렇게 하면 bar의 끝위치가 나옴
	}
}

void CHealthBar::AddLife(float life){
	if (m_fLife + life >= m_fMaxLife){
		m_fLife = m_fMaxLife;
		m_HealthBar->setPercentage(100);
	}
	else{
		m_fLife += life;
		float percent = (m_fLife / m_fMaxLife) * 100;
		m_HealthBar->setPercentage(percent);

		m_Health->setPosition(Vec2(												// 하트모양 position
			(m_HealthBarImg->getContentSize().width * (percent * 0.01f))		// x = bar크기 * (percent * 0.01) - bar앵커 포인트가 0.5니까 그만큼뺀다.
			- m_HealthBarImg->getContentSize().width * 0.5f, 0));				// 이렇게 하면 bar의 끝위치가 나옴
	}
}

void CHealthBar::Execute(float delta)
{
	Hit(delta);
}