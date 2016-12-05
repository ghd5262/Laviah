#pragma once
#include "Popup.h"

class CMyButton;
class CUILayer : public CPopup
{
public:
    static CUILayer* create();
    void CountDown(int count, std::string finMent = "0", const std::function<void(void)> &func = nullptr);
    
protected:
    virtual bool init() override;
    
private:
    void onPauseButton(cocos2d::Node* sender);
    void initItemTestButton();
    
    CUILayer()
    : m_PauseBtn(nullptr)
    , m_CountDownLabel(nullptr){};
    virtual ~CUILayer(){};
    
private:
    CMyButton* m_PauseBtn;
    cocos2d::Label* m_CountDownLabel;
};