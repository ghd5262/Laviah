#pragma once
#include "../../Common/HSHUtility.h"

#define REWARD_COIN                    "REWARD_COIN"

class CChallengeRewarder{
public:
    void coinReward            (int value);
    
    CChallengeRewarder(){};
    virtual ~CChallengeRewarder(){};
};