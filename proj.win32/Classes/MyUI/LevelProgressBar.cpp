#include "LevelProgressBar.h"

using namespace cocos2d;

CLevelProgressBar* CLevelProgressBar::create(Size barSize, int maxLevel, int curLevel/* = 0 */)
{
	CLevelProgressBar *pRet = new(std::nothrow) CLevelProgressBar(barSize, maxLevel, curLevel);
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

bool CLevelProgressBar::init()
{
    if (!Node::init()) return false;
    
    float oneBarDistance = 6.f;
    float oneBarSize = (m_BarSize.width - (oneBarDistance * (m_MaxLevel - 2))) / m_MaxLevel;
    
    for (int barIdx = 0; barIdx < m_MaxLevel; barIdx++)
    {
        auto bar = LayerColor::create(COLOR::DARKGRAY_ALPHA, oneBarSize, m_BarSize.height);
        bar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bar->setPosition(Vec2(((oneBarDistance + oneBarSize) * barIdx) - (m_BarSize.width * 0.5f), 0));
        addChild(bar);
        m_LevelBarList.emplace_back(bar);
    }
    
    this->UpdateProgress();
    
    return true;
}

void CLevelProgressBar::UpdateProgress()
{
	if (m_CurrentLevel > m_MaxLevel){
		m_CurrentLevel = m_MaxLevel;
		CCLOG("LevelProgressBar : CurrentLevel > MaxLevel");
        return;
	}

	for (int barIdx = 0; barIdx < m_CurrentLevel; barIdx++)
	{
        m_LevelBarList[barIdx]->setColor(Color3B::WHITE);
		m_LevelBarList[barIdx]->setOpacity(255 * 0.8f);
	}
}