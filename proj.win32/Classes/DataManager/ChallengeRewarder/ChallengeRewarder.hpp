#pragma once
#include "../../Common/HSHUtility.h"
#include "../ChallengeDataManager.hpp"

namespace CHALLENGE_REWARD_KEY {
    
	const std::string REWARD_COIN				= "REWARD_COIN";
	const std::string REWARD_CHARACTER			= "REWARD_CHARACTER";
	const std::string REWARD_ROCKET				= "REWARD_ROCKET";
	const std::string REWARD_PET				= "REWARD_PET";

	const std::string REWARD_COIN_RANDOM		= "REWARD_COIN_RANDOM";
	const std::string REWARD_CHARACTER_RANDOM	= "REWARD_CHARACTER_RANDOM";
	const std::string REWARD_ROCKET_RANDOM		= "REWARD_ROCKET_RANDOM";
	const std::string REWARD_PET_RANDOM			= "REWARD_PET_RANDOM";
};

class CChallengeRewarder{
public:
	sREWARD_DATA coinReward				(sREWARD_DATA data);
	sREWARD_DATA characterReward		(sREWARD_DATA data);
	sREWARD_DATA RocketReward			(sREWARD_DATA data);
	sREWARD_DATA PetReward				(sREWARD_DATA data);

	sREWARD_DATA coinRewardRandom		(sREWARD_DATA data);
	sREWARD_DATA characterRewardRandom	(sREWARD_DATA data);
	sREWARD_DATA RocketRewardRandom		(sREWARD_DATA data);
	sREWARD_DATA PetRewardRandom		(sREWARD_DATA data);

    CChallengeRewarder(){};
    virtual ~CChallengeRewarder(){};
};