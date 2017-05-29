#pragma once
#include "../../Common/HSHUtility.h"

namespace ACHIEVEMENT_DATA_KEY {

    const std::string TOTAL_SCORE          = "TOTAL_SCORE";
    const std::string COIN_SCORE           = "COIN_SCORE";
    const std::string STAR_SCORE           = "STAR_SCORE";
    const std::string RUN_SCORE            = "RUN_SCORE";
    const std::string COMBO_SCORE          = "COMBO_SCORE";
    
    const std::string COIN_COUNT		   = "COIN_COUNT";
    const std::string STAR_COUNT		   = "STAR_COUNT";
    const std::string REVIVE_COUNT		   = "REVIVE_COUNT";
    const std::string COLLISION_COUNT      = "COLLISION_COUNT";
    
    const std::string GIANT_SCORE_TOTAL    = "GIANT_SCORE_TOTAL";
    const std::string GIANT_SCORE_BULLET   = "GIANT_SCORE_BULLET";
    const std::string GIANT_SCORE_MISSILE  = "GIANT_SCORE_MISSILE";
    const std::string GIANT_SCORE_STICK    = "GIANT_SCORE_STICK";
    const std::string MAGNET_SCORE         = "MAGNET_SCORE";
    const std::string BARRIER_SCORE        = "BARRIER_SCORE";
    
    const std::string GIANT_COUNT_TOTAL    = "GIANT_COUNT_TOTAL";
    const std::string GIANT_COUNT_BULLET   = "GIANT_COUNT_BULLET";
    const std::string GIANT_COUNT_MISSILE  = "GIANT_COUNT_MISSILE";
    const std::string GIANT_COUNT_STICK    = "GIANT_COUNT_STICK";
    const std::string MAGNET_COUNT         = "MAGNET_COUNT";
    const std::string BARRIER_COUNT        = "BARRIER_COUNT";
    
    const std::string COIN_ITEM_USE        = "COIN_ITEM_USE";
    const std::string STAR_ITEM_USE        = "STAR_ITEM_USE";
    const std::string BONUS_ITEM_USE       = "BONUS_ITEM_USE";
    const std::string GIANT_ITEM_USE       = "GIANT_ITEM_USE";
    const std::string MAGNET_ITEM_USE      = "MAGNET_ITEM_USE";
    const std::string BARRIER_ITEM_USE     = "BARRIER_ITEM_USE";
    
    const std::string CHARACTER_COLLECT    = "USER_CHARACTER_LIST";
    const std::string ROCKET_COLLECT       = "USER_ROCKET_LIST";
    
    const std::string CHARACTER_COUNT      = "USER_CHARACTER_LIST";
    const std::string CHARACTER_RARE_COUNT = "CHARACTER_RARE_COUNT";
    
    const std::string ROCKET_COUNT         = "USER_ROCKET_LIST";
    const std::string ROCKET_RARE_COUNT	   = "ROCKET_RARE_COUNT";
    
    const std::string USER_LEVEL           = "USER_LEVEL";
    const std::string ITEM_LEVEL           = "USER_ITEM_LEVEL";
    
    const std::string ROCKET_COIN_COUNT    = "ROCKET_COIN_COUNT";
    
    const std::string BEST_SCORE           = "USER_BEST_TOTAL_SCORE";
    const std::string BEST_COMBO           = "USER_BEST_COMBO";
    const std::string COIN                 = "USER_COIN";
    const std::string SAVED_RUN			   = "USER_SAVED_RUN";
    const std::string SAVED_SCORE		   = "USER_SAVED_SCORE";
    const std::string SAVED_REVIVE		   = "USER_SAVED_REVIVE";
    const std::string NORMAL_CLEAR_COUNT   = "USER_NORMAL_ACHIEVEMENT_COUNT";
    const std::string HIDDEN_CLEAR_COUNT   = "USER_HIDDEN_ACHIEVEMENT_COUNT";
};

class CAchievementClearChecker {
public:
    bool checkWithGlobal		(std::string key, int value, int& currentValue);
	bool checkWithSingleUserData(std::string key, int value, int& currentValue);
	bool checkWithItemExist		(std::string key, int value, int& currentValue);
	bool checkWithCount			(std::string key, int value, int& currentValue);
    bool checkWithItemParam     (std::string key, int itemIndex,
                                 int paramIndex,  int value, int& currentValue);
    bool checkWithContinuingType(std::string key, int value, int& currentValue);

	bool characterRareCountCheck(int value, int& currentValue);
	bool rocketRareCountCheck	(int value, int& currentValue);

    CAchievementClearChecker(){};
    virtual ~CAchievementClearChecker(){};
};