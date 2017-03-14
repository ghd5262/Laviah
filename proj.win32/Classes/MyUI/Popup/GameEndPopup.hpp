#pragma once
#include "../Popup.h"

class CMyButton;
class CGameEndPopup : public CPopup
{
public:
    static CGameEndPopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void End(cocos2d::Node* sender);
    
    CGameEndPopup(){};
    virtual ~CGameEndPopup(){};
};