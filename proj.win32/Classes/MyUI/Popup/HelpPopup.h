#pragma once
#include "../Popup.h"

class CMyButton;
class CHelpPopup : public CSpecificPopupBase
{
public:
	static CHelpPopup* create();

protected:
	virtual bool initVariable() override;

private:
	void End();

	CHelpPopup()
		: m_btnEnd(nullptr)
		, m_BG(nullptr){};
	virtual ~CHelpPopup(){};

private:
	CMyButton* m_btnEnd;
	LayerColor* m_BG;
};