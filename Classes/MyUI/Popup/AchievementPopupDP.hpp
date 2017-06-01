#pragma once
#include "cocos2d.h"
#include "../../Common/HSHUtility.h"

USING_NS_CC;
struct ACHIEVEMENT;
class CMyButton;
class CAchievementPopupDP : public cocos2d::ui::Widget
{
public:
    static CAchievementPopupDP* create(const ACHIEVEMENT* data);
    
    void Reward();
    
    //setter & getter
    const ACHIEVEMENT* getAchievementData() const { return m_AchievementData; }
protected:
    virtual bool init() override;
    
private:
    void contentUpdate();
    float getPercent(float value, float max);
    
    CAchievementPopupDP(const ACHIEVEMENT* data)
    : m_AchievementData(data)
    , m_RewardValue(0){}
    virtual ~CAchievementPopupDP(){};
    
private:
    typedef std::function<void(void)> UPDATE_LISTENER;
    std::vector<UPDATE_LISTENER> m_ListenerList;
    const ACHIEVEMENT* m_AchievementData;
    int m_RewardValue;
};
