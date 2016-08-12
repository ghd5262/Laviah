#pragma once
#include "cocos2d.h"
#include "../../DataManager/UserDataManager.h"

USING_NS_CC;

class CGoogleCloudTestPopupDP : public Widget
{
public:
    static CGoogleCloudTestPopupDP* create(std::string key);
	void DeSelect();

protected:
	virtual bool init() override;
	bool initVariable();

private:
	void Buy();

	CGoogleCloudTestPopupDP(std::string key)
        : m_UserKey(key)
        , m_DPBack(nullptr){};
	virtual ~CGoogleCloudTestPopupDP(){};

private:
    std::string m_UserKey;
    LayerColor* m_DPBack;
};

