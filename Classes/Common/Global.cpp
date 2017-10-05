#include "Global.hpp"
#include "../DataManager/AchievementChecker/AchievementClearChecker.h"

Global::Global()
{
    this->addVariableToList();
    this->Clear();
}

void Global::Clear()
{
    BONUSTIME = 0;
    
    TOTAL_SCORE = 0;
    COIN_SCORE = 0;
    STAR_SCORE = 0;
    RUN_SCORE = 0;
    COMBO_SCORE = 0;
    COMBO_LEVEL = 1;
    BEST_COMBO = 0;
    STAGE_LEVEL = 0;
    NOTICE_LEVEL = 0;
    DEAD_TYPE = 0;
    REVIVED = 0;
    
    COIN_COUNT = 0;
	STAR_COUNT = 0;
	REVIVE_COUNT = 0;
    
    GIANT_SCORE_TOTAL = 0;
    GIANT_SCORE_BULLET = 0;
    GIANT_SCORE_MISSILE = 0;
    GIANT_SCORE_STICK = 0;
    MAGNET_SCORE = 0;
    BARRIER_SCORE = 0;
    
    GIANT_COUNT_TOTAL = 0;
    GIANT_COUNT_BULLET = 0;
    GIANT_COUNT_MISSILE = 0;
    GIANT_COUNT_STICK = 0;
    MAGNET_COUNT = 0;
    BARRIER_COUNT = 0;
    
    COIN_ITEM_USE = 0;
    STAR_ITEM_USE = 0;
    BONUS_ITEM_USE = 0;
    GIANT_ITEM_USE = 0;
    MAGNET_ITEM_USE = 0;
    BARRIER_ITEM_USE = 0;
    
    NORMAL_ACHIEVEMENT_CLEAR_COUNT = 0;
    HIDDEN_ACHIEVEMENT_CLEAR_COUNT = 0;
    
    STAR_BARRIER_COUNT = 0;
    COIN_BARRIER_COUNT = 0;
    
    CURRENT_CHARACTER = 0;
    CURRENT_COSTUME = 0;
}

Global* Global::Instance()
{
    static Global instance;
    return &instance;
}

int Global::getVariable(std::string key)
{
    auto var = m_VariableList.find(key);
    if(var == std::end(m_VariableList))
        return 0;
    return var->second;
}

void Global::addVariableToList()
{
    auto addVariable = [=](std::string key, int &var){
        m_VariableList.emplace(std::pair<std::string, int&>(key, var));
    };
    
    addVariable( ACHIEVEMENT_DATA_KEY::TOTAL_SCORE           , TOTAL_SCORE            );
    addVariable( ACHIEVEMENT_DATA_KEY::COIN_SCORE            , COIN_SCORE             );
    addVariable( ACHIEVEMENT_DATA_KEY::STAR_SCORE            , STAR_SCORE             );
    addVariable( ACHIEVEMENT_DATA_KEY::RUN_SCORE             , RUN_SCORE              );
    addVariable( ACHIEVEMENT_DATA_KEY::COMBO_SCORE           , COMBO_SCORE            );

    addVariable( ACHIEVEMENT_DATA_KEY::COIN_COUNT            , COIN_COUNT             );
    addVariable( ACHIEVEMENT_DATA_KEY::STAR_COUNT            , STAR_COUNT             );
    addVariable( ACHIEVEMENT_DATA_KEY::REVIVE_COUNT          , REVIVE_COUNT           );

    addVariable( ACHIEVEMENT_DATA_KEY::GIANT_SCORE_TOTAL     , GIANT_SCORE_TOTAL      );
    addVariable( ACHIEVEMENT_DATA_KEY::GIANT_SCORE_BULLET    , GIANT_SCORE_BULLET     );
    addVariable( ACHIEVEMENT_DATA_KEY::GIANT_SCORE_MISSILE   , GIANT_SCORE_MISSILE    );
    addVariable( ACHIEVEMENT_DATA_KEY::GIANT_SCORE_STICK     , GIANT_SCORE_STICK      );
    addVariable( ACHIEVEMENT_DATA_KEY::MAGNET_SCORE          , MAGNET_SCORE           );
    addVariable( ACHIEVEMENT_DATA_KEY::BARRIER_SCORE         , BARRIER_SCORE          );
    
    addVariable( ACHIEVEMENT_DATA_KEY::GIANT_COUNT_TOTAL     , GIANT_COUNT_TOTAL      );
    addVariable( ACHIEVEMENT_DATA_KEY::GIANT_COUNT_BULLET    , GIANT_COUNT_BULLET     );
    addVariable( ACHIEVEMENT_DATA_KEY::GIANT_COUNT_MISSILE   , GIANT_COUNT_MISSILE    );
    addVariable( ACHIEVEMENT_DATA_KEY::GIANT_COUNT_STICK     , GIANT_COUNT_STICK      );
    addVariable( ACHIEVEMENT_DATA_KEY::MAGNET_COUNT          , MAGNET_COUNT           );
    addVariable( ACHIEVEMENT_DATA_KEY::BARRIER_COUNT         , BARRIER_COUNT          );
    
    addVariable( ACHIEVEMENT_DATA_KEY::COIN_ITEM_USE         , COIN_ITEM_USE          );
    addVariable( ACHIEVEMENT_DATA_KEY::STAR_ITEM_USE         , STAR_ITEM_USE          );
    addVariable( ACHIEVEMENT_DATA_KEY::BONUS_ITEM_USE        , BONUS_ITEM_USE         );
    addVariable( ACHIEVEMENT_DATA_KEY::GIANT_ITEM_USE        , GIANT_ITEM_USE         );
    addVariable( ACHIEVEMENT_DATA_KEY::MAGNET_ITEM_USE       , MAGNET_ITEM_USE        );
    addVariable( ACHIEVEMENT_DATA_KEY::BARRIER_ITEM_USE      , BARRIER_ITEM_USE       );
    
    addVariable( ACHIEVEMENT_DATA_KEY::ACHIEVEMENT_N_CLEAR, NORMAL_ACHIEVEMENT_CLEAR_COUNT);
    addVariable( ACHIEVEMENT_DATA_KEY::ACHIEVEMENT_H_CLEAR, HIDDEN_ACHIEVEMENT_CLEAR_COUNT);
    
    addVariable( ACHIEVEMENT_DATA_KEY::STAR_BARRIER_COUNT    , STAR_BARRIER_COUNT     );
    addVariable( ACHIEVEMENT_DATA_KEY::COIN_BARRIER_COUNT    , COIN_BARRIER_COUNT     );
    addVariable( ACHIEVEMENT_DATA_KEY::CURRENT_CHARACTER     , CURRENT_CHARACTER      );
    addVariable( ACHIEVEMENT_DATA_KEY::CURRENT_COSTUME       , CURRENT_COSTUME        );
}
