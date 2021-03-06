#pragma once
#include "cocos2d.h"
#include <vector>
#include "../Common/HSHUtility.h"

class CCountDown : public cocos2d::Node{
    typedef std::function<void(cocos2d::Node*)> LAST_CALLBACK;
    
public:
    static CCountDown* create();
    CCountDown* addLastEventListner(const LAST_CALLBACK &callback);
    CCountDown* setLastContent(std::string content);
    CCountDown* setInterval(float interval);
    CCountDown* setMaxNumber(int number);
    CCountDown* setMinNumber(int number);
    CCountDown* setCountUp(bool up);
    CCountDown* setFont(cocos2d::Color4B fontColor, int fontSize);
    CCountDown* setFontName(std::string fontName);
    CCountDown* setCleanUpAtTheLast(bool clean);
    CCountDown* setLabelPosition(cocos2d::Vec2 pos);
    CCountDown* setLabelAnchorPoint(cocos2d::Vec2 anchorPoint);
    CCountDown* show(cocos2d::Node* parent, int zOrder = 0);
    
    virtual void update(float delta) override;
    virtual bool init() override;
    
    void Pause(){ m_IsPause = true; };
    void Resume(){ m_IsPause = false; };
    void Reset();
    
private:
    void updateContent();
    void processEventListener();
    
    CCountDown()
    : m_NumberLabel(nullptr)
    , m_FontColor(cocos2d::Color3B::BLACK)
    , m_Interval(1.f)
    , m_Time(0.f)
    , m_MaxNumber(0)
    , m_MinNumber(0)
    , m_CurrentNumber(0)
    , m_FontSize(50)
    , m_FontName(FONT::MALGUNBD)
    , m_LastContent("")
    , m_IsCountUP(false)
    , m_IsPause(false)
    , m_CleanUP(false){};
    virtual ~CCountDown(){};
    
private:
    std::vector<LAST_CALLBACK> m_EventList;
    cocos2d::Label* m_NumberLabel;
    cocos2d::Color4B m_FontColor;
    float m_Interval;
    float m_Time;
    int m_MaxNumber;
    int m_MinNumber;
    int m_CurrentNumber;
    int m_FontSize;
    std::string m_FontName;
    std::string m_LastContent;
    cocos2d::Vec2 m_Position;
    cocos2d::Vec2 m_AnchorPoint;
    bool m_IsCountUP;
    bool m_IsPause;
    bool m_CleanUP;
};