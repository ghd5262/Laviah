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
	void Buy(Node* sender);
    
	CWorkshopPopupDP(sWORKSHOPITEM_PARAM workshopItem)
		: m_WorkshopItem(workshopItem)
        , m_DPBack(nullptr)
        , m_BuyBtn(nullptr)
        , m_LevelProgressBar(nullptr){};
    virtual ~CWorkshopPopupDP(){};
    
private:
	const sWORKSHOPITEM_PARAM m_WorkshopItem;
	LayerColor* m_DPBack;
    CMyButton* m_BuyBtn;
    CLevelProgressBar* m_LevelProgressBar;
};
