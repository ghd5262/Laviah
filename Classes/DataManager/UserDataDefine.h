#pragma once

namespace USERDATA_KEY {
    const std::string FREE_REWARD_LEVEL         = "USER_DATA_FREE_REWARD_LEVEL";
    const std::string FREE_REWARD_TIMESTAMP     = "USER_DATA_FREE_REWARD_TIMESTAMP";
    const std::string DAILY_RESET_SAVED_TIME    = "USER_DATA_DAILY_RESET_SAVED_TIME";
    const std::string WEEKLY_RESET_SAVED_TIME   = "USER_DATA_WEEKLY_RESET_SAVED_TIME";
    const std::string LAST_SAVED_TIME		    = "USER_DATA_LAST_SAVED_TIME";
    const std::string FIRST_LOAD			    = "USER_DATA_FIRSTLOAD";
    const std::string LAST_RES_VERSION			= "USER_DATA_LAST_RES_VERSION";

    const std::string DATA_REVISION				= "USER_DATA_SAVE_REVISION";
    const std::string DATA_SAVE_AUTO            = "USER_DATA_SAVE_AUTO";
    
    const std::string LEVEL                     = "USER_LEVEL";
    const std::string EXP                       = "USER_EXP";
    const std::string RANK                      = "USER_RANK";
    const std::string CHARACTER                 = "USER_CUR_CHARACTER";
    const std::string COIN                      = "USER_COIN";
    
    const std::string SAVED_RUN					= "USER_SAVED_RUN";
    const std::string SAVED_SCORE				= "USER_SAVED_SCORE";
    const std::string SAVED_REVIVE				= "USER_SAVED_REVIVE";
    
    const std::string PET                       = "USER_CUR_PET";
    const std::string ROCKET                    = "USER_CUR_ROCKET";
    const std::string PLANET                    = "USER_CUR_PLANET";
    
    const std::string BEST_COMBO                = "USER_BEST_COMBO";
    const std::string BEST_SCORE                = "USER_BEST_TOTAL_SCORE";
    
    const std::string SELECT_ITEM               = "USER_CUR_SELECT_ITEM";
    const std::string LAST_COM_ACHIEVEMENT      = "USER_LAST_COM_ACHIEVEMENT";
    
    const std::string BGM_VOLUME                = "USER_BGM_VOLUME";
    const std::string EFFECT_VOLUME             = "USER_EFFECT_VOLUME";
    
    const std::string CURRENT_LANGUAGE          = "USER_CURRENT_LANGUAGE";
    
    const std::string NORMAL_CLEAR_COUNT        = "USER_NORMAL_ACHIEVEMENT_COUNT";
    const std::string HIDDEN_CLEAR_COUNT        = "USER_HIDDEN_ACHIEVEMENT_COUNT";
    
    const std::string PLAY_COUNT                = "USER_PLAY_COUNT";
    const std::string APPRECIATE_PLANET         = "USER_APPRECIATE_PLANET";
    const std::string CHANGE_TO_COIN            = "USER_CHANGE_TO_COIN";
    const std::string CHANGE_TO_STAR            = "USER_CHANGE_TO_STAR";
    const std::string PHOTO_SHARE               = "USER_PHOTO_SHARE";
    const std::string ROCKET_CONTROLL           = "USER_ROCKET_CONTROLL";
    const std::string FACEBOOK_COUNT            = "USER_FACEBOOK_COUNT";
    const std::string DEAD_BY_MISSILE           = "USER_DEAD_BY_MISSILE";
    
    const std::string CHARACTER_LIST            = "USER_CHARACTER_LIST";
    const std::string COSTUME_LIST              = "USER_COSTUME_LIST";
    const std::string ROCKET_LIST               = "USER_ROCKET_LIST";
    const std::string PET_LIST                  = "USER_PET_LIST";
    const std::string ITEM_LEVEL                = "USER_ITEM_LEVEL";
    const std::string NORMAL_ACHIEVEMENT_LIST   = "USER_NORMAL_ACHIEVEMENT_LIST";
    const std::string HIDDEN_ACHIEVEMENT_LIST   = "USER_HIDDEN_ACHIEVEMENT_LIST";
};


enum PARAM_CHARACTER{
    COSTUME_INDEX           = 0,
    
    COIN_ITEM_USE           = 1,
    STAR_ITEM_USE           = 2,
    GIANT_ITEM_USE          = 3,
    MAGNET_ITEM_USE         = 4,
    BARRIER_ITEM_USE        = 5,
    
    COIN_COUNT_DUR_C_ITEM   = 6,
    STAR_COUNT_DUR_S_ITEM   = 7,
    BU_HIT_COUNT_DUR_G_ITEM = 8,
    MI_HIT_COUNT_DUR_G_ITEM = 9,
    STAR_COUNT_DUR_M_ITEM   = 10,
    COIN_COUNT_DUR_M_ITEM   = 11,
    DEF_COUNT_DUR_B_ITEM    = 12,
    
    PLAY_COUNT              = 13,
    REVIVE_COUNT            = 14,
    DEAD_COUNT              = 15,
    DEAD_BY_MISSILE_COUNT   = 16,
    DEAD_BY_STAND_COUNT     = 17,
    DEAD_BY_STICK_COUNT     = 18,
    
    CHARACTER_PARAM_MAX
};

///< 0: star, 1: coin, 2: barrier, 3: magnet, 4: magnet-r, 5: giant, 6: giant-r
enum PARAM_WORKSHOP{
    ITEM_LEVEL      = 0,
};

enum PARAM_ACHIEVEMENT_NORMAL{
    NORMAL_LEVEL    = 0,
    NORMAL_STATE    = 1, ///< 0: non-completed, 1: running, 2: finished, 3: completed
};

enum PARAM_ACHIEVEMENT_HIDDEN{
    HIDDEN_LEVEL    = 0,
    HIDDEN_STATE    = 1,
};
