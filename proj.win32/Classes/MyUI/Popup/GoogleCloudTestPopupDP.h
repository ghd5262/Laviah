#pragma once
#include "cocos2d.h"
#include "../../DataManager/UserDataManager.h"

USING_NS_CC;

class CGoogleCloudTestPopupDP : public cocos2d::Layer
{
public:
    static CGoogleCloudTestPopupDP* create(std::string key, const std::function<void(cocos2d::Ref*)> &func);
	void DeSelect();

protected:
	virtual bool init() override;
	bool initVariable();

private:
	void Buy();

	CGoogleCloudTestPopupDP(std::string key, const std::function<void(cocos2d::Ref*)> &func)
		: m_UserKey(key)
		, m_SelectFunc(func){};
	virtual ~CGoogleCloudTestPopupDP(){};

private:
    std::string m_UserKey;
	std::function<void(cocos2d::Ref*)> m_SelectFunc;
};

