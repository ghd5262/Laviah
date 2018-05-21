#pragma once
#include "../../Common/HSHUtility.h"
#include "../AchievementDataManager.hpp"

namespace ACHIEVEMENT_REWARD_KEY {
    
	const std::string REWARD_COIN				= "REWARD_COIN";
	const std::string REWARD_CHARACTER			= "REWARD_CHARACTER";
    const std::string REWARD_COSTUME			= "REWARD_COSTUME";
    const std::string REWARD_ITEM               = "REWARD_ITEM";
    const std::string REWARD_ROCKET				= "REWARD_ROCKET";
	const std::string REWARD_PET				= "REWARD_PET";

    
	const std::string REWARD_COIN_RANDOM		= "REWARD_COIN_RANDOM";
	const std::string REWARD_CHARACTER_RANDOM	= "REWARD_CHARACTER_RANDOM";
    const std::string REWARD_COSTUME_RANDOM     = "REWARD_COSTUME_RANDOM";
    const std::string REWARD_ITEM_RANDOM        = "REWARD_ITEM_RANDOM";
	const std::string REWARD_ROCKET_RANDOM		= "REWARD_ROCKET_RANDOM";
	const std::string REWARD_PET_RANDOM			= "REWARD_PET_RANDOM";
};

class CAchievementRewarder{
public:
	sREWARD_DATA coinReward				(sREWARD_DATA data);
	sREWARD_DATA characterReward		(sREWARD_DATA data);
    sREWARD_DATA costumeReward          (sREWARD_DATA data);
    sREWARD_DATA itemReward             (sREWARD_DATA data);
	sREWARD_DATA rocketReward			(sREWARD_DATA data);
	sREWARD_DATA petReward				(sREWARD_DATA data);

	sREWARD_DATA coinRewardRandom		(sREWARD_DATA data);
	sREWARD_DATA characterRewardRandom	(sREWARD_DATA data);
    sREWARD_DATA costumeRewardRandom	(sREWARD_DATA data);
    sREWARD_DATA itemRewardRandom       (sREWARD_DATA data);
	sREWARD_DATA rocketRewardRandom		(sREWARD_DATA data);
	sREWARD_DATA petRewardRandom		(sREWARD_DATA data);

    CAchievementRewarder(){};
    virtual ~CAchievementRewarder(){};
};
