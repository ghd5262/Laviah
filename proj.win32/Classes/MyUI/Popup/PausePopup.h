#pragma once
#include "../Popup.h"
#include "ui/UIPageView.h"
#include "ui/UIImageView.h"

class CMyButton;
class CUserCoinButton;
class CPausePopup : public CSpecificPopupBase
{
public:
	static CPausePopup* create();

protected:
	virtual bool initVariable() override;

private:
	void Play(Node* sender);
	void Reset(Node* sender);
	void GoHome(Node* sender);
	void GameExit(Node* sender);
	void Help(Node* sender);

	CPausePopup()
		: m_btnHome(nullptr)
		, m_btnPlay(nullptr)
		, m_btnReset(nullptr)
        , m_btnExit(nullptr)
        , m_btnHelp(nullptr)
		, m_btnUserCoin(nullptr)
		, m_BG(nullptr)
        , m_PauseBG(nullptr){};
	virtual ~CPausePopup(){};

private:
	CUserCoinButton* m_btnUserCoin;
	CMyButton* m_btnHome;
	CMyButton* m_btnReset;
	CMyButton* m_btnPlay;
    CMyButton* m_btnExit;
    CMyButton* m_btnHelp;
	LayerColor* m_BG;
    LayerColor* m_PauseBG;
};