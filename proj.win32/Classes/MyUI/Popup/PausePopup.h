#pragma once
#include "../Popup.h"
#include "ui/UIPageView.h"
#include "ui/UIImageView.h"

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
    void GameExit();
    void Help();

	CPausePopup()
		: m_btnHome(nullptr)
		, m_btnPlay(nullptr)
		, m_btnReset(nullptr)
        , m_btnExit(nullptr)
        , m_btnHelp(nullptr)
		, m_BG(nullptr)
        , m_PauseBG(nullptr){};
	virtual ~CPausePopup(){};

private:
	CMyButton* m_btnHome;
	CMyButton* m_btnReset;
	CMyButton* m_btnPlay;
    CMyButton* m_btnExit;
    CMyButton* m_btnHelp;
	LayerColor* m_BG;
    LayerColor* m_PauseBG;
};