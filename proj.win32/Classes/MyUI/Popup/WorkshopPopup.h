#pragma once
#include "../Popup.h"
#include "ui/UIScrollView.h"

class CMyButton;
class CUserCoinButton;
class CWorkshopPopup : public CPopup
{
public:
    static CWorkshopPopup* create();
    
protected:
	virtual bool init() override;
    
private:
	void End(cocos2d::Node* sender);
	void ScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type);
    
    CWorkshopPopup(){};
    virtual ~CWorkshopPopup(){};
};