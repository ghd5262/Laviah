#pragma once
#include "../Popup.h"

class CMyButton;
class CUserCoinButton;
class CCountDown;

class CVideoPopup : public CPopup
{
public:
	static CVideoPopup* create();

protected:
	virtual bool init() override;

private:
	void End(cocos2d::Node* sender);
	void Video(cocos2d::Node* sender);
	void UseCoin(cocos2d::Node* sender);
	void Resume();

	CVideoPopup(){};
	virtual ~CVideoPopup(){};
    
private:
    CCountDown* m_CountDown;
};