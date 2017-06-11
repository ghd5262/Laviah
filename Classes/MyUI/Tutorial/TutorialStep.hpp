#pragma once
#include "../Popup.h"

class CMyButton;
class CTutorialStep : public CPopup {
    typedef std::function<void(CTutorialStep*)>        SINGLE_LISTENER;
    typedef std::function<void(float, CTutorialStep*)> UPDATE_LISTENER;
    
public:
    static CTutorialStep* create();
    CTutorialStep* addBeginListener(const SINGLE_LISTENER& listener);
    CTutorialStep* addUpdateListener(const UPDATE_LISTENER& listener);
    CTutorialStep* addEndListener(const SINGLE_LISTENER& listener);
    CTutorialStep* addButton(CMyButton* button);
    CTutorialStep* addMessageBox(std::string message);
    CTutorialStep* setMessageBoxPosition(cocos2d::Vec2 position);
    CTutorialStep* setTailPosition(cocos2d::Vec2 tailPosition);
    CTutorialStep* SaveStepEnable(bool enable);
    CTutorialStep* build(std::string key);
    
    void Begin();
    void Update(float delta);
    void End();
    
    std::string getTutorialKey() const { return m_TutorialKey; }
    
    CC_SYNTHESIZE_READONLY(cocos2d::LayerColor*, m_MessageLayer, MessageLayer);
    CC_SYNTHESIZE(float, m_Time, Time);
private:
    void createMessageBox();
    void callListener(SINGLE_LISTENER listener);
    void clear();
    
    CTutorialStep()
    : m_BeginListener(nullptr)
    , m_EndListener(nullptr)
    , m_UpdateListener(nullptr)
    , m_Button(nullptr)
    , m_MessageLayer(nullptr)
    , m_MessageBoxPosition(cocos2d::Vec2::ZERO)
    , m_MessageBoxTailPosition(cocos2d::Vec2::ZERO)
    , m_TutorialKey("")
    , m_Message("")
    , m_Time(0.f)
    , m_SaveStepEnable(false){}

public:
    virtual ~CTutorialStep(){};

private:
    SINGLE_LISTENER m_BeginListener;
    SINGLE_LISTENER m_EndListener;
    UPDATE_LISTENER m_UpdateListener;
    CMyButton* m_Button;
    cocos2d::Vec2 m_MessageBoxPosition;
    cocos2d::Vec2 m_MessageBoxTailPosition;
    std::string m_TutorialKey;
    std::string m_Message;
    bool m_SaveStepEnable;
};