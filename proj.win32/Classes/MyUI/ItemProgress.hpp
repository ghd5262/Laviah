#pragma once
#include "cocos2d.h"
#include <vector>
#include "../Common/HSHUtility.h"

class CItemProgress : public cocos2d::Node{
    typedef std::function<void(cocos2d::Node*)> LAST_CALLBACK;
    
public:
    static CItemProgress* create();
    CItemProgress* addLastEventListner(const LAST_CALLBACK &callback);
    CItemProgress* setIcon(std::string icon);
    CItemProgress* setBarColor(cocos2d::Color3B color);
    CItemProgress* setDirection(bool isRight);
    CItemProgress* setCleanUpAtTheLast(bool clean);
    CItemProgress* setBarPosition(cocos2d::Vec2 pos);
    CItemProgress* setBarAnchorPoint(cocos2d::Vec2 anchorPoint);
    CItemProgress* show(cocos2d::Node* parent, int zOrder = 0);
    
    virtual void update(float delta) override;
    virtual bool init() override;
    
    void Reset();
    
    CC_SYNTHESIZE(float, m_LimitTime, LimitTime);
    CC_SYNTHESIZE(bool, m_IsPause, IsPause);
private:
    void processEventListner();
    float getPercent(float value, float max);
    
    CItemProgress()
    : m_ProgressBar(nullptr)
    , m_BarColor(cocos2d::Color3B::WHITE)
    , m_IconName("")
    , m_Time(0.f)
    , m_LimitTime(0.f)
    , m_IsDirectionRight(false)
    , m_IsPause(true)
    , m_CleanUP(false){};
    virtual ~CItemProgress(){};
    
private:
    std::vector<LAST_CALLBACK> m_EventList;
    cocos2d::ProgressTimer* m_ProgressBar;
    cocos2d::Color3B m_BarColor;
    cocos2d::Vec2 m_Position;
    cocos2d::Vec2 m_AnchorPoint;
    std::string m_IconName;
    float m_Time;
    bool m_IsDirectionRight;
    bool m_CleanUP;
};