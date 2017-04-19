#pragma once
#include "cocos2d.h"
#include "RewardPopup.h"
#include "../Popup.h"
#include "../../DataManager/ChallengeDataManager.hpp"

class CRewardPopupDP : public CPopup
{
public:
    static CRewardPopupDP* create(const sREWARD_DATA reward);
    
    //setter & getter
	const sREWARD_DATA getRewardData() const { return m_Reward; }
    
protected:
    virtual bool init() override;
    
private:
    void goldReward();
    void characterReward();
    
	CRewardPopupDP(const sREWARD_DATA reward)
		: m_Reward(reward){};
    virtual ~CRewardPopupDP(){};
    
private:
	const sREWARD_DATA m_Reward;
};