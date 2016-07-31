#pragma once
#include "cocos2d.h"
#include "../../DataManager/WorkshopItemDataManager.h"
USING_NS_CC;

class CWorkshopPopupDP : public cocos2d::Layer
{
public:
	static CWorkshopPopupDP* create(sWORKSHOPITEM_PARAM workshopItem, const std::function<void(cocos2d::Ref*)> &func);
    void DeSelect();
    
protected:
    virtual bool init() override;
    bool initVariable();
    
private:
    void Buy();
    
	CWorkshopPopupDP(sWORKSHOPITEM_PARAM workshopItem, const std::function<void(cocos2d::Ref*)> &func)
		: m_WorkshopItem(workshopItem)
    , m_SelectFunc(func){};
    virtual ~CWorkshopPopupDP(){};
    
private:
	const sWORKSHOPITEM_PARAM m_WorkshopItem;
    std::function<void(cocos2d::Ref*)> m_SelectFunc;
};
