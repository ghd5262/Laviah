#pragma once
#include "../Popup.h"
#include "ui/UITextField.h"

class CMyButton;
class TextField;
class CCouponPopup : public CPopup
{
public:
    static CCouponPopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void input();
    void cancel();
    void createOneBtnPopup(std::string text);
    
    CCouponPopup()
    : m_TextField(nullptr){};
    virtual ~CCouponPopup(){};
    
private:
    cocos2d::ui::TextField* m_TextField;
};
