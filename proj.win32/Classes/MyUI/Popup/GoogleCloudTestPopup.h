#pragma once
#include "../Popup.h"
#include "ui/UIScrollView.h"

class CMyButton;
class CGoogleCloudTestPopup : public CPopup
{
public:
	static CGoogleCloudTestPopup* create();

protected:
	virtual bool init() override;

private:
	void End(Node* sender);
	void Select(cocos2d::Ref* dp);
    void ScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type);
    
    CGoogleCloudTestPopup(){}
    virtual ~CGoogleCloudTestPopup(){};
};