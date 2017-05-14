#pragma once
#include "cocos2d.h"
#include "../../Common/HSHUtility.h"

USING_NS_CC;
struct sACHIEVEMENT_PARAM;
class CAchievementPopupDP : public cocos2d::ui::Widget
{
public:
    static CAchievementPopupDP* create(const sACHIEVEMENT_PARAM* achievement, int level, int maxLevel);
    
    void Share();
    void Notice();
    void Invite();
    
    //setter & getter
    const sACHIEVEMENT_PARAM* getAchievementParam() const { return m_Achievement; }
    
    CC_SYNTHESIZE(std::function<void(int)>, m_SelectDPListener, SelectDPListener)
    
protected:
    virtual bool init() override;
    
private:
    
    CAchievementPopupDP(const sACHIEVEMENT_PARAM* achievement, int level, int maxLevel)
    : m_Achievement(achievement)
    , m_AchievementLevel(level)
    , m_AchievementMaxLevel(maxLevel)
    , m_SelectDPListener(nullptr){};
    virtual ~CAchievementPopupDP(){};
    
private:
    const sACHIEVEMENT_PARAM* m_Achievement;
    int m_AchievementLevel;
    int m_AchievementMaxLevel;
};
