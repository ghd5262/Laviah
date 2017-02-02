#pragma once
#include "cocos2d.h"
#include <vector>
#include "../Common/HSHUtility.h"
#include "../DataManager/ChallengeDataManager.hpp"

class CChallengeProgressBar : public cocos2d::Node{
    typedef std::function<void(cocos2d::Node*)> LAST_CALLBACK;
    
public:
    static CChallengeProgressBar* create();
    CChallengeProgressBar* addLastEventListner(const LAST_CALLBACK &callback);
    CChallengeProgressBar* setBarBGColor(cocos2d::Color4B color);
    CChallengeProgressBar* setBarColor(cocos2d::Color3B color);
    CChallengeProgressBar* setBarPosition(cocos2d::Vec2 pos);
    CChallengeProgressBar* setBarAnchorPoint(cocos2d::Vec2 anchorPoint);
    CChallengeProgressBar* show(cocos2d::Node* parent, int zOrder = 0);
    
    virtual void update(float delta) override;
    virtual bool init() override;
    
    void Reset();
    
    CC_SYNTHESIZE(int, m_GoalValue, GoalValue);
    CC_SYNTHESIZE(int, m_CurrentValue, CurrentValue);
private:
    void setCompletionUI();
    void processEventListener();
    float getPercent(float value, float max);
    
    CChallengeProgressBar()
    : m_ChallengeData(nullptr)
    , m_ProgressBar(nullptr)
    , m_TitleLabel(nullptr)
    , m_BarBGColor(COLOR::TRANSPARENT_ALPHA)
    , m_BarColor(cocos2d::Color3B::WHITE)
    , m_CurrentValue(0)
    , m_GoalValue(0)
    , m_CurrentChallengeIndex(0)
    , m_Complete(false){};
    virtual ~CChallengeProgressBar(){};
    
private:
    std::vector<LAST_CALLBACK> m_EventList;
    const sCHALLENGE_PARAM* m_ChallengeData;
    cocos2d::ProgressTimer* m_ProgressBar;
    cocos2d::Label* m_TitleLabel;
    cocos2d::Color4B m_BarBGColor;
    cocos2d::Color3B m_BarColor;
    cocos2d::Vec2 m_Position;
    cocos2d::Vec2 m_AnchorPoint;
    int m_CurrentChallengeIndex;
    bool m_Complete;
};