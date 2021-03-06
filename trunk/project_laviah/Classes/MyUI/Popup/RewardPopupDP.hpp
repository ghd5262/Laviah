#pragma once
#include "cocos2d.h"
#include "../Popup.h"
#include "../../DataManager/AchievementDataManager.hpp"

class CRewardPopupDP : public CPopup
{
public:
    static CRewardPopupDP* create();
    CRewardPopupDP* setExitCallback(std::function<void()> listener);
    CRewardPopupDP* setRewardData(sREWARD_DATA reward);
    CRewardPopupDP* setIsUFOEnable(bool enable);
    CPopup* show(Node* parent = nullptr, int zOrder = 0);
    
    //setter & getter
    const sREWARD_DATA getRewardData() const { return m_Reward; }
    
private:
    void exitWithDelay(float delay);
    void goldReward();
    void characterReward();
    void costumeReward();
    
    void characterCreator1(cocos2d::Sprite* character);
    void characterCreator2(cocos2d::Sprite* character);
    void characterCreator3(cocos2d::Sprite* character);
    
    CRewardPopupDP()
    : m_ExitListener(nullptr)
    , m_IsUFOEnabled(false){};
    virtual ~CRewardPopupDP(){};
    
private:
    sREWARD_DATA m_Reward;
    std::function<void()> m_ExitListener;
    bool m_IsUFOEnabled;
};
