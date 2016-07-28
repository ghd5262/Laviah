#pragma once
#include "cocos2d.h"

USING_NS_CC;

class CGoogleCloudTestPopupDP : public cocos2d::Layer
{
public:
	static CGoogleCloudTestPopupDP* create(int keyIdx, const std::function<void(cocos2d::Ref*)> &func);
	void DeSelect();

protected:
	virtual bool init() override;
	bool initVariable();

private:
	void Buy();

	CGoogleCloudTestPopupDP(int keyIdx, const std::function<void(cocos2d::Ref*)> &func)
		: m_UserKeyIdx(keyIdx)
		, m_SelectFunc(func){};
	virtual ~CGoogleCloudTestPopupDP(){};

private:
	int m_UserKeyIdx;
	std::function<void(cocos2d::Ref*)> m_SelectFunc;
};

