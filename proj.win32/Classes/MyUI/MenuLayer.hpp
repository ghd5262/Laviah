#pragma once
#include "Popup.h"

class CMyButton;
class CMenuLayer : public CPopup
{
public:
    static CMenuLayer* create();
    
	bool TouchScreen(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

protected:
    virtual bool init() override;
    
private:
    
    CMenuLayer(){};
    virtual ~CMenuLayer(){};

};