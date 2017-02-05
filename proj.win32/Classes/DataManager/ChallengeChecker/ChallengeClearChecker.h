#pragma once
#include "../../Common/HSHUtility.h"

namespace CHALLENGE_DATA_KEY {

    const std::string TOTAL_SCORE         = "TOTAL_SCORE";
    const std::string COIN_SCORE          = "COIN_SCORE";
    const std::string STAR_SCORE          = "STAR_SCORE";
    const std::string RUN_SCORE           = "RUN_SCORE";
    const std::string BEST_SCORE          = "BEST_SCORE";
    const std::string BEST_COMBO          = "BEST_COMBO";
	const std::string COIN_COUNT		  = "COIN_COUNT";
	const std::string STAR_COUNT		  = "STAR_COUNT";
	const std::string REVIVE_COUNT		  = "REVIVE_COUNT";

    const std::string CHARACTER_COLLECT   = "CHARACTER_COLLECT";
    const std::string ROCKET_COLLECT      = "ROCKET_COLLECT";
    
    const std::string CHARACTER_COUNT     = "CHARACTER_COUNT";
	const std::string CHARACTER_RARE_COUNT= "CHARACTER_RARE_COUNT";
    const std::string ROCKET_COUNT        = "ROCKET_COUNT";
	const std::string ROCKET_RARE_COUNT	  = "ROCKET_RARE_COUNT";

    
    const std::string USER_LEVEL          = "USER_LEVEL";
    const std::string COIN_ITEM_LEVEL     = "COIN_ITEM_LEVEL";
    const std::string STAR_ITEM_LEVEL     = "STAR_ITEM_LEVEL";
    const std::string BONUS_ITEM_LEVEL    = "BONUS_ITEM_LEVEL";
    const std::string GIANT_ITEM_LEVEL    = "GIANT_ITEM_LEVEL";
    const std::string MAGNET_ITEM_LEVEL   = "MAGNET_ITEM_LEVEL";
    const std::string MAGNET_SIZE_LEVEL   = "MAGNET_SIZE_LEVEL";
    
    const std::string GIANT_SCORE_TOTAL   = "GIANT_SCORE_TOTAL";
    const std::string GIANT_SCORE_BULLET  = "GIANT_SCORE_BULLET";
    const std::string GIANT_SCORE_MISSILE = "GIANT_SCORE_MISSILE";
    const std::string GIANT_SCORE_STICK   = "GIANT_SCORE_STICK";
    const std::string MAGNET_SCORE        = "MAGNET_SCORE";
    const std::string BARRIER_SCORE       = "BARRIER_SCORE";
    
    const std::string GIANT_COUNT_TOTAL   = "GIANT_COUNT_TOTAL";
    const std::string GIANT_COUNT_BULLET  = "GIANT_COUNT_BULLET";
    const std::string GIANT_COUNT_MISSILE = "GIANT_COUNT_MISSILE";
    const std::string GIANT_COUNT_STICK   = "GIANT_COUNT_STICK";
    const std::string MAGNET_COUNT        = "MAGNET_COUNT";
    const std::string BARRIER_COUNT       = "BARRIER_COUNT";
    
    const std::string COIN_ITEM_USE       = "COIN_ITEM_USE";
    const std::string STAR_ITEM_USE       = "STAR_ITEM_USE";
    const std::string BONUS_ITEM_USE      = "BONUS_ITEM_USE";
    const std::string GIANT_ITEM_USE      = "GIANT_ITEM_USE";
    const std::string MAGNET_ITEM_USE     = "MAGNET_ITEM_USE";
    const std::string BARRIER_ITEM_USE    = "BARRIER_ITEM_USE";
    
    const std::string ROCKET_COIN_COUNT   = "ROCKET_COIN_COUNT";
    const std::string COMBO               = "COMBO";
    const std::string COIN                = "COIN";
	const std::string SAVED_RUN			  = "SAVED_RUN";
	const std::string SAVED_SCORE		  = "SAVED_SCORE";
	const std::string SAVED_REVIVE		  = "SAVED_REVIVE";

};

class CChallengeClearChecker {
public:
    bool continuingTypeCheck   (int index);
    bool checkWithGlobal       (std::string key, int value);
    
    bool bestScoreCheck        (int value);
    bool bestComboCheck        (int value);
    
    bool characterCollectCheck (int value);
    bool rocketCollectCheck    (int value);
    bool characterCountCheck   (int value);
    bool rocketCountCheck      (int value);
    
    bool userLevelCheck        (int value);
    bool coinItemLevelCheck    (int value);
    bool starItemLevelCheck    (int value);
    bool bonusItemLevelCheck   (int value);
    bool giantItemLevelCheck   (int value);
    bool magnetItemLevelCheck  (int value);
    bool magnetSizeLevelCheck  (int value);
    
    bool coinCheck             (int value);

    CChallengeClearChecker(){};
    virtual ~CChallengeClearChecker(){};
};