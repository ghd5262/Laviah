#pragma once
#include "cocos2d.h"
#include "../../DataManager/WorkshopItemDataManager.h"
USING_NS_CC;

class CMyButton;
class CLevelProgressBar;
class CItemShopPopupDP : public cocos2d::ui::Widget
{
public:
    static CItemShopPopupDP* create(const WORKSHOPITEM_PARAM* workshopItem);
    void calculateRemainTime();
    
protected:
    virtual bool init() override;
    
private:
    void use();
    
    CItemShopPopupDP(const WORKSHOPITEM_PARAM* workshopItem)
    : m_WorkshopItem(workshopItem)
    , m_BtnUse(nullptr)
    , m_LevelProgressBar(nullptr){};
    virtual ~CItemShopPopupDP(){};
    
private:
    const WORKSHOPITEM_PARAM* m_WorkshopItem;
    CMyButton* m_BtnUse;
    CLevelProgressBar* m_LevelProgressBar;
};
