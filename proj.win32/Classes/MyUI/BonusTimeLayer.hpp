#pragma once
#include "Popup.h"

class CMyButton;
class CScoreUI;
class CBonusTimeLayer : public CPopup{
public:
    static CBonusTimeLayer* create();
    virtual void update(float delta) override;
    
protected:
    virtual bool init() override;
    
private:
    void stop();
    void play();
    void onPauseButton(cocos2d::Node* sender);
    
    CBonusTimeLayer()
    : m_PauseBtn(nullptr)
    , m_StarScoreUI(nullptr)
    , m_CoinScoreUI(nullptr)
    , m_RunScoreUI(nullptr)
    , m_Pause(false){};
    virtual ~CBonusTimeLayer(){};
    
private:
    CMyButton* m_PauseBtn;
    CScoreUI* m_StarScoreUI;
    CScoreUI* m_CoinScoreUI;
    CScoreUI* m_RunScoreUI;
    bool m_Pause;};