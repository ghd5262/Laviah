#pragma once
#include "cocos2d.h"

USING_NS_CC;

class CWorkshopPopupDP : public cocos2d::Layer
{
public:
	static CWorkshopPopupDP* create(int workshopItemIdx, const std::function<void(cocos2d::Ref*)> &func);
    void DeSelect();
    
protected:
    virtual bool init() override;
    bool initVariable();
    
private:
    void Buy();
    
	CWorkshopPopupDP(int workshopItemIdx, const std::function<void(cocos2d::Ref*)> &func)
		: m_WorkshopItemIdx(workshopItemIdx)
    , m_SelectFunc(func){};
    virtual ~CWorkshopPopupDP(){};
    
private:
    int m_WorkshopItemIdx;
    std::function<void(cocos2d::Ref*)> m_SelectFunc;
};
