#pragma once
#include "../../Popup.h"

class CMyButton;
class COptionLanguagePopup : public CPopup
{
public:
    static COptionLanguagePopup* create();
    
protected:
    virtual bool init() override;
    
private:
    CMyButton* createLanguageDP(std::string content, cocos2d::Vec2 pos);
    
    COptionLanguagePopup(){};
    virtual ~COptionLanguagePopup(){};
};