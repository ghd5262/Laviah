#pragma once
#include "cocos2d.h"
#include <vector>
#include "../Common/HSHUtility.h"
struct STAGE;
class CStageProgressBar : public cocos2d::Node{
    typedef std::function<void(cocos2d::Node*)> LAST_CALLBACK;
    
public:
    static CStageProgressBar* create();
    CStageProgressBar* addLastEventListner(const LAST_CALLBACK &callback);
    CStageProgressBar* setBarBGColor(cocos2d::Color4B color);
    CStageProgressBar* setBarColor(cocos2d::Color3B color);
    CStageProgressBar* setBarPosition(cocos2d::Vec2 pos);
    CStageProgressBar* setProgressBar(std::string name);
    CStageProgressBar* setLabelVisible(bool visible);
    CStageProgressBar* setBarAnchorPoint(cocos2d::Vec2 anchorPoint);
    CStageProgressBar* show(cocos2d::Node* parent, int zOrder = 0);
    
    void Update(float delta);
    virtual bool init() override;
    
    void Reset();
    
    CC_SYNTHESIZE(int, m_GoalValue, GoalValue);
    CC_SYNTHESIZE(int, m_CurrentValue, CurrentValue);
private:
    void setCompletionUI();
    void processEventListener();
    
    CStageProgressBar()
    : m_BarBG(nullptr)
    , m_ProgressBar(nullptr)
    , m_TitleLabel(nullptr)
    , m_BarBGColor(COLOR::TRANSPARENT_ALPHA)
    , m_BarColor(cocos2d::Color3B::WHITE)
    , m_CurrentValue(0)
    , m_GoalValue(0)
    , m_ProgressBarName("")
    , m_Complete(false)
    , m_LabelVisible(true){};
    virtual ~CStageProgressBar(){};
    
private:
    std::vector<LAST_CALLBACK> m_EventList;
    std::vector<cocos2d::Node*> m_SavePointList;
    const STAGE* m_StageData;
    cocos2d::LayerColor* m_BarBG;
    cocos2d::ProgressTimer* m_ProgressBar;
    cocos2d::Label* m_TitleLabel;
    cocos2d::Color4B m_BarBGColor;
    cocos2d::Color3B m_BarColor;
    cocos2d::Vec2 m_Position;
    cocos2d::Vec2 m_AnchorPoint;
    std::string m_ProgressBarName;
    bool m_Complete;
    bool m_LabelVisible;
};
