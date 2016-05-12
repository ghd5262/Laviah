#pragma once
#include "../Popup.h"

class CMyButton;
class CResultPopup : public CSpecificPopupBase
{
public:
	static CResultPopup* create();

protected:
	virtual bool initVariable() override;

private:
	void Play();
	void Reset();
	void GoHome();

	CResultPopup()
		: m_btnHome(nullptr)
		, m_btnPlay(nullptr)
		, m_btnReset(nullptr)
		, m_BG(nullptr){};
	virtual ~CResultPopup(){};

private:
	CMyButton* m_btnHome;
	CMyButton* m_btnReset;
	CMyButton* m_btnPlay;
	Sprite* m_BG;
};