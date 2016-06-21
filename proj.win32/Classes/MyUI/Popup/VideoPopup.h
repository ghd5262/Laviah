#pragma once
#include "../Popup.h"

class CMyButton;
class CVideoPopup : public CSpecificPopupBase
{
public:
	static CVideoPopup* create();

protected:
	virtual bool initVariable() override;

private:
	void End();
	void Video();
	void UseCoin();

	CVideoPopup()
		: m_btnEnd(nullptr)
		, m_btnVideo(nullptr)
		, m_btnUseCoin(nullptr)
		, m_BG(nullptr){};
	virtual ~CVideoPopup(){};

private:
	CMyButton* m_btnEnd;
	CMyButton* m_btnVideo;
	CMyButton* m_btnUseCoin;
	LayerColor* m_BG;
};