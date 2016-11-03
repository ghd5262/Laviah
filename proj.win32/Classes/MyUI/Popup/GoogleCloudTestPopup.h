#pragma once
#include "../Popup.h"
#include "ui/UIScrollView.h"

class CMyButton;
class CGoogleCloudTestPopup : public CSpecificPopupBase
{
public:
	static CGoogleCloudTestPopup* create();

protected:
	virtual bool initVariable() override;

private:
	void End(Node* sender);
	void Select(cocos2d::Ref* dp);
    void ScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type);
    
	CGoogleCloudTestPopup()
		: m_btnEnd(nullptr)
		, m_BG(nullptr)
		, m_ScrollBack(nullptr){};
	virtual ~CGoogleCloudTestPopup(){};

private:
	CMyButton* m_btnEnd;
	LayerColor* m_BG;
	LayerColor* m_ScrollBack;
};