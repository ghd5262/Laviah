#pragma once
#include "cocos2d.h"
#include "../Popup.h"

struct ACHIEVEMENT;
class CTitleCompleteNoticePopup : public CPopup
{
public:
    static CTitleCompleteNoticePopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void checkAchievementCompleteOnRealTime();
    void show();
    
    CTitleCompleteNoticePopup()
    : m_LayerBG(nullptr)
    , m_NewIcon(nullptr)
    , m_AchievementLabelFront(nullptr)
    , m_AchievementLabelBack(nullptr)
    , m_Checkable(true){};
    virtual ~CTitleCompleteNoticePopup(){};
    
private:
    cocos2d::Sprite* m_LayerBG;
    cocos2d::Sprite* m_NewIcon;
    cocos2d::Label* m_AchievementLabelFront;
    cocos2d::Label* m_AchievementLabelBack;
    bool m_Checkable;
};
