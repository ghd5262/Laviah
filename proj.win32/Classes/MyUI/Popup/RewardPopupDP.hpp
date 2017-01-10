#pragma once
#include "cocos2d.h"
#include "../Popup.h"
#include "../../DataManager/ChallengeDataManager.hpp"

class CRewardPopupDP : public CPopup
{
public:
    static CRewardPopupDP* create(const sCHALLENGE_PARAM challenge);
    
    //setter & getter
    const sCHALLENGE_PARAM getChallengeParam() const { return m_Challenge; }
    
protected:
    virtual bool init() override;
    
private:
    void initReward();
    CRewardPopupDP(const sCHALLENGE_PARAM challenge)
    : m_Challenge(challenge){};
    virtual ~CRewardPopupDP(){};
    
private:
    const sCHALLENGE_PARAM m_Challenge;
};