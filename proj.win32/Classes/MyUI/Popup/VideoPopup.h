#pragma once
#include "../Popup.h"

class CMyButton;
class CUserCoinButton;

class CVideoPopup : public CSpecificPopupBase
{
public:
	static CVideoPopup* create();

protected:
	virtual bool initVariable() override;

private:
	void End(Node* sender);
	void Video(Node* sender);
	void UseCoin(Node* sender);
	void Resume();

	CVideoPopup()
		: m_btnEnd(nullptr)
		, m_btnVideo(nullptr)
		, m_btnUseCoin(nullptr)
		, m_btnUserCoin(nullptr)
		, m_BG(nullptr){};
	virtual ~CVideoPopup(){};

private:
	CMyButton* m_btnEnd;
	CMyButton* m_btnVideo;
	CMyButton* m_btnUseCoin;
	CUserCoinButton* m_btnUserCoin;
	LayerColor* m_BG;
};