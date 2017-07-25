#pragma once
#include "Popup.h"

class CMyButton;
class CMenuLayer : public CPopup
{
public:
    static CMenuLayer* Instance();

    void AchievementButtonState(bool enable, bool newIcon = false);
    
	bool TouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	void TouchScreen(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

protected:
    virtual bool init() override;
    
private:
    
    CMenuLayer()
    : m_AchievementButton(nullptr)
    , m_NewIcon(nullptr){};
    virtual ~CMenuLayer();

private:
    static CMenuLayer* m_Instance;
    CMyButton* m_AchievementButton;
    cocos2d::Sprite* m_NewIcon;
};
