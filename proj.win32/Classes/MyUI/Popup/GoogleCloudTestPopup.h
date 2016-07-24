#pragma once
#include "../Popup.h"

class CMyButton;
class CGoogleCloudTestPopup : public CSpecificPopupBase
{
public:
	static CGoogleCloudTestPopup* create();

protected:
	virtual bool initVariable() override;

private:
	void End();
	void Select(cocos2d::Ref* dp);

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