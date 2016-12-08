#pragma once
#include "Popup.h"

class CMyButton;
class CCountDown;
class CUILayer : public CPopup
{
public:
    static CUILayer* create();
    virtual void update(float delta) override;
    void Resume();
    void Pause();
    
protected:
    virtual bool init() override;
    
private:
    void onPauseButton(cocos2d::Node* sender);
    void initItemTestButton();
    
    CUILayer()
    : m_CountDown(nullptr)
    , m_PauseBtn(nullptr)
    , m_Pause(false){};
    virtual ~CUILayer(){};
    
private:
    CCountDown* m_CountDown;
    CMyButton* m_PauseBtn;
    bool m_Pause;
};