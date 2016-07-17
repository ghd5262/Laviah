#pragma once
#include "../Popup.h"

class CMyButton;
class CUserCoinButton;

class CGachaPopup : public CSpecificPopupBase
{
public:
	static CGachaPopup* create();

protected:
	virtual bool initVariable() override;

private:
	void PlayGacha();
	void End();

	CGachaPopup() 
		: m_BG(nullptr)
		, m_GachaBG(nullptr)
		, m_btnExit(nullptr)
		, m_btnUserCoin(nullptr)
	{};
	virtual ~CGachaPopup(){};

private:
	CUserCoinButton* m_btnUserCoin;
	CMyButton* m_btnExit;
	LayerColor* m_BG;
	LayerColor* m_GachaBG;
};