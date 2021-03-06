#pragma once
#include <vector>
#include "../GameObject/GameObject.h"

class CLevelProgressBar : public CGameObject
{
public:
	static CLevelProgressBar* create(cocos2d::Size barSize, int maxLevel, int curLevel = 0);
	void UpdateProgress();

protected:
	virtual bool init() override;

private:
	CLevelProgressBar(cocos2d::Size barSize, int maxLevel, int curLevel)
		: m_BarSize(barSize)
		, m_MaxLevel(maxLevel)
		, m_CurrentLevel(curLevel){}
	virtual ~CLevelProgressBar(){}

	//getter & setter
	CC_SYNTHESIZE(int, m_CurrentLevel, CurrentLevel);
	CC_SYNTHESIZE_READONLY(int, m_MaxLevel, MaxLevel);

private:
	std::vector<cocos2d::LayerColor*> m_LevelBarList;
	cocos2d::Size m_BarSize;
};

