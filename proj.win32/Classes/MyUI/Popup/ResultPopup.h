#pragma once
#include "../Popup.h"

class CMyButton;
class CUserCoinButton;
class CResultPopup : public CSpecificPopupBase
{
public:
	static CResultPopup* create();

protected:
	virtual bool initVariable() override;

private: 
	void Reset(Node* sender);
	void GoHome(Node* sender);

	CResultPopup()
		: m_btnHome(nullptr)
		, m_btnReset(nullptr)
		, m_btnUserCoin(nullptr)
		, m_BG(nullptr){};
	virtual ~CResultPopup(){};

private:
	CMyButton* m_btnHome;
	CMyButton* m_btnReset;
	CUserCoinButton* m_btnUserCoin;
	LayerColor* m_BG;
};