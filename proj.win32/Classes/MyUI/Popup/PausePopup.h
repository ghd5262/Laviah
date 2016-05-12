#pragma once
#include "../Popup.h"

class CMyButton;
class CPausePopup : public CSpecificPopupBase
{
public:
	static CPausePopup* create();

protected:
	virtual bool initVariable() override;

private:
	void Play();
	void Reset();
	void GoHome();

	CPausePopup()
		: m_btnHome(nullptr)
		, m_btnPlay(nullptr)
		, m_btnReset(nullptr)
		, m_BG(nullptr){};
	virtual ~CPausePopup(){};

private:
	CMyButton* m_btnHome;
	CMyButton* m_btnReset;
	CMyButton* m_btnPlay;
	Sprite* m_BG;
};