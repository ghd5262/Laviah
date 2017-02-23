#pragma once
#include "cocos2d.h"
#include "../Popup.h"

class CTitleCompleteNoticePopup : public CPopup
{
public:
    static CTitleCompleteNoticePopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void checkChallengeCompleteOnRealTime();
    void show();
    
    CTitleCompleteNoticePopup()
    : m_LayerBG(nullptr)
    , m_ChallengeLabel(nullptr)
    , m_Checkable(true){};
    virtual ~CTitleCompleteNoticePopup(){};
    
private:
    cocos2d::Sprite* m_LayerBG;
    cocos2d::Label* m_ChallengeLabel;
    bool m_Checkable;
};