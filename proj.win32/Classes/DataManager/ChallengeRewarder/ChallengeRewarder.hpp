#pragma once
#include "../../Common/HSHUtility.h"

namespace CHALLENGE_REWARD_KEY {
    
    const std::string COIN         = "REWARD_COIN";
    const std::string CHARACTER    = "REWARD_CHARACTER";
    const std::string ROCKET       = "REWARD_ROCKET";
    const std::string PET          = "REWARD_PET";
    
};

class CChallengeRewarder{
public:
    void coinReward            (int value);
    
    CChallengeRewarder(){};
    virtual ~CChallengeRewarder(){};
};