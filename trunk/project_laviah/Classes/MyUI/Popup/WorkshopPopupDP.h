#pragma once
#include "cocos2d.h"
#include "../../DataManager/WorkshopItemDataManager.h"
USING_NS_CC;

class CMyButton;
class CLevelProgressBar;
class CWorkshopPopupDP : public cocos2d::ui::Widget
{
public:
	static CWorkshopPopupDP* create(const WORKSHOPITEM_PARAM* workshopItem);

protected:
    virtual bool init() override;
    
private:
	void Buy(cocos2d::Node* sender);
    
	CWorkshopPopupDP(const WORKSHOPITEM_PARAM* workshopItem)
		: m_WorkshopItem(workshopItem)
		, m_BtnBuy(nullptr)
        , m_LevelProgressBar(nullptr){};
    virtual ~CWorkshopPopupDP(){};
    
private:
	const WORKSHOPITEM_PARAM* m_WorkshopItem;
	CMyButton* m_BtnBuy;
    CLevelProgressBar* m_LevelProgressBar;
};
