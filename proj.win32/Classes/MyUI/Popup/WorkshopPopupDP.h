#pragma once
#include "cocos2d.h"
#include "../../DataManager/WorkshopItemDataManager.h"
USING_NS_CC;

class CWorkshopPopupDP : public Widget
{
public:
	static CWorkshopPopupDP* create(sWORKSHOPITEM_PARAM workshopItem);

	virtual const Size& getContentSize() const override;

protected:
    virtual bool init() override;
    bool initVariable();
    
private:
    void Buy();
    
	CWorkshopPopupDP(sWORKSHOPITEM_PARAM workshopItem)
		: m_WorkshopItem(workshopItem){};
    virtual ~CWorkshopPopupDP(){};
    
private:
	const sWORKSHOPITEM_PARAM m_WorkshopItem;
	LayerColor* m_DPBack;
};
