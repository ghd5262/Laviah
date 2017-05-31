#pragma once
#include "cocos2d.h"
#include "../Popup.h"
#include <Queue>

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
    , m_AchievementLabelFront(nullptr)
    , m_AchievementLabelBack(nullptr)
    , m_Checkable(true){};
    virtual ~CTitleCompleteNoticePopup(){};
    
private:
    struct COMPLETED_ACHIEVEMENT{
        int _index;
        int _level;
        bool _isHidden;
        COMPLETED_ACHIEVEMENT(int index, int level, bool hidden)
        : _index(index)
        , _level(level)
        , _isHidden(hidden){};
    };
    cocos2d::Sprite* m_LayerBG;
    cocos2d::Label* m_AchievementLabelFront;
    cocos2d::Label* m_AchievementLabelBack;
    std::queue<COMPLETED_ACHIEVEMENT> m_ShowList;
    bool m_Checkable;
};