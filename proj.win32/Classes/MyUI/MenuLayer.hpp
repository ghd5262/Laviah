#pragma once
#include "Popup.h"

class CMyButton;
class CMenuLayer : public CPopup
{
public:
    static CMenuLayer* Instance();

	bool TouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	void TouchScreen(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

protected:
    virtual bool init() override;
    
private:
    
    CMenuLayer(){};
    virtual ~CMenuLayer();

private:
    static CMenuLayer* m_Instance;
};