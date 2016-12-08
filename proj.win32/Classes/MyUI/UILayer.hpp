#pragma once
#include "Popup.h"

class CMyButton;
class CCountDown;
class CScoreUI;
class CGameScene;
class CUILayer : public CPopup
{
public:
    static CUILayer* create();
    virtual void update(float delta) override;
    void Resume();
    void Pause();
    
protected:
    virtual bool init() override;
    
private:
    void onPauseButton(cocos2d::Node* sender);
    void initItemTestButton();
    
    CUILayer()
    : m_CountDown(nullptr)
    , m_PauseBtn(nullptr)
    , m_StarScoreUI(nullptr)
    , m_CoinScoreUI(nullptr)
    , m_RunScoreUI(nullptr)
    , m_GameScene(nullptr)
    , m_Pause(false){};
    virtual ~CUILayer(){};
    
private:
    CCountDown* m_CountDown;
    CMyButton* m_PauseBtn;
    CScoreUI* m_StarScoreUI;
    CScoreUI* m_CoinScoreUI;
    CScoreUI* m_RunScoreUI;
    CGameScene* m_GameScene;
    bool m_Pause;
};