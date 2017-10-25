#pragma once
#include "../Popup.h"

class CMyButton;
class CUserCoinButton;
class CCountDown;

class CRevivePopup : public CPopup
{
public:
    static CRevivePopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void end();
    void reviveByFree();
    void reviveByVideo(cocos2d::Node* sender);
    void gameResume();
    
    CRevivePopup()
    : m_ReviveButtonTouched(false)
    , m_IsEnded(false)
    , m_FreeTimer(nullptr)
    , m_VideoTimer(nullptr){};
    virtual ~CRevivePopup(){};
    
private:
    ProgressTimer* m_FreeTimer;
    ProgressTimer* m_VideoTimer;
    CCountDown* m_CountDown;
    bool m_ReviveButtonTouched;
    bool m_IsEnded;
};
