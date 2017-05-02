#pragma once
#include "cocos2d.h"
#include <vector>
#include "../Common/HSHUtility.h"
#include "../DataManager/AchievementDataManager.hpp"

class CAchievementProgressBar : public cocos2d::Node{
    typedef std::function<void(cocos2d::Node*)> LAST_CALLBACK;
    
public:
    static CAchievementProgressBar* create();
    CAchievementProgressBar* addLastEventListner(const LAST_CALLBACK &callback);
    CAchievementProgressBar* setLabelVisible(bool visible);
    CAchievementProgressBar* setBarBGColor(cocos2d::Color4B color);
    CAchievementProgressBar* setBarColor(cocos2d::Color3B color);
    CAchievementProgressBar* setBarPosition(cocos2d::Vec2 pos);
    CAchievementProgressBar* setBarAnchorPoint(cocos2d::Vec2 anchorPoint);
    CAchievementProgressBar* show(cocos2d::Node* parent, int zOrder = 0);
    
    virtual void update(float delta) override;
    virtual bool init() override;
    
    void Reset();
    
    CC_SYNTHESIZE(int, m_GoalValue, GoalValue);
    CC_SYNTHESIZE(int, m_CurrentValue, CurrentValue);
private:
    void setCompletionUI();
    void processEventListener();
    float getPercent(float value, float max);
    
    CAchievementProgressBar()
    : m_AchievementData(nullptr)
    , m_BarBG(nullptr)
    , m_ProgressBar(nullptr)
    , m_TitleLabel(nullptr)
    , m_BarBGColor(COLOR::TRANSPARENT_ALPHA)
    , m_BarColor(cocos2d::Color3B::WHITE)
    , m_CurrentValue(0)
    , m_GoalValue(0)
    , m_CurrentAchievementIndex(0)
    , m_Complete(false)
    , m_LabelVisible(true){};
    virtual ~CAchievementProgressBar(){};
    
private:
    std::vector<LAST_CALLBACK> m_EventList;
    const sACHIEVEMENT_PARAM* m_AchievementData;
    cocos2d::LayerColor* m_BarBG;
    cocos2d::ProgressTimer* m_ProgressBar;
    cocos2d::Label* m_TitleLabel;
    cocos2d::Color4B m_BarBGColor;
    cocos2d::Color3B m_BarColor;
    cocos2d::Vec2 m_Position;
    cocos2d::Vec2 m_AnchorPoint;
    int m_CurrentAchievementIndex;
    bool m_Complete;
    bool m_LabelVisible;
};