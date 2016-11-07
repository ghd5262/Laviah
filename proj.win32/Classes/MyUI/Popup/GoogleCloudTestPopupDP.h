#pragma once
#include "cocos2d.h"
#include "../../DataManager/UserDataManager.h"

USING_NS_CC;

class CGoogleCloudTestPopupDP : public Widget
{
public:
    static CGoogleCloudTestPopupDP* create(std::string key);
	void DeSelect(Node* sender);

protected:
	virtual bool init() override;
	bool initVariable();

private:
	void Buy(Node* sender);

	CGoogleCloudTestPopupDP(std::string key)
        : m_UserKey(key){};
	virtual ~CGoogleCloudTestPopupDP(){};

private:
    std::string m_UserKey;
};

