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
	void ReviveByCoin(cocos2d::Node* sender);
    void ReviveByVideo(cocos2d::Node* videoBtn);
	void Resume();
        
	CVideoPopup()
    : m_ReviveButtonTouched(false)
    , m_IsEnded(false){};
	virtual ~CVideoPopup(){};
    
private:
    CCountDown* m_CountDown;
    bool m_ReviveButtonTouched;
    bool m_IsEnded;
};
