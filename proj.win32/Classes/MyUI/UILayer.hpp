#pragma once
#include "Popup.h"
#include "../GameObject/Bullet/Bullet.h"
#include <array>
class CMyButton;
class CScoreUI;
class CItemProgress;
class CUILayer : public CPopup
{
    typedef std::vector<CItemProgress*> PROGRESS_LIST;

public:
    static CUILayer* Instance();
    void setItemTimer(eITEM_TYPE type, float limitTime);
    
    virtual void update(float delta) override;
    
protected:
    virtual bool init() override;
    
private:
    void stop();
    void play();
    void onPauseButton(cocos2d::Node* sender);
    void createItemTimerUI(std::string iconName, Color3B color);
    void initItemTestButton();
    
    
    CUILayer()
    : m_PauseBtn(nullptr)
    , m_StarScoreUI(nullptr)
    , m_CoinScoreUI(nullptr)
    , m_RunScoreUI(nullptr)
    , m_TimerRunningCount(0)
    , m_Pause(false){};
    virtual ~CUILayer();
    
private:
    static CUILayer* m_Instance;
    CMyButton* m_PauseBtn;
    CScoreUI* m_StarScoreUI;
    CScoreUI* m_CoinScoreUI;
    CScoreUI* m_RunScoreUI;
    PROGRESS_LIST m_ProgressList;
    std::array<cocos2d::Vec2, eITEM_TYPE_MAX> m_ProgressPosArray;
    int m_TimerRunningCount;
    bool m_Pause;
};