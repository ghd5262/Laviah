#pragma once
#include "cocos2d.h"
#include "../../DataManager/WorkshopItemDataManager.h"
USING_NS_CC;

class CMyButton;
class CLevelProgressBar;
class CWorkshopPopupDP : public Widget
{
public:
	static CWorkshopPopupDP* create(sWORKSHOPITEM_PARAM workshopItem);

protected:
    virtual bool init() override;
    bool initVariable();
    
private:
	void Buy(cocos2d::Node* sender);
    
	CWorkshopPopupDP(sWORKSHOPITEM_PARAM workshopItem)
		: m_WorkshopItem(workshopItem)
		, m_BtnBuy(nullptr)
        , m_LevelProgressBar(nullptr){};
    virtual ~CWorkshopPopupDP(){};
    
private:
	const sWORKSHOPITEM_PARAM m_WorkshopItem;
	CMyButton* m_BtnBuy;
    CLevelProgressBar* m_LevelProgressBar;
};
