#include "Global.hpp"
#include "../DataManager/AchievementChecker/AchievementClearChecker.h"

Global* Global::CurrentValue = nullptr;
Global* Global::SavedValue = nullptr;

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
    LAST_SAVED_POINT = 0;
    
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
    CURRENT_PLANET = 0;
}

Global* Global::Instance()
{
    if(CurrentValue == nullptr) CurrentValue = new Global();
    return CurrentValue;
}

Global* Global::SavedData()
{
    if(SavedValue == nullptr) SavedValue = new Global();
    return SavedValue;
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

int Global::getPercent(float value, float max)
{
    if(value != 0 && max != 0)
        return (int)std::min(100.f, (value / max) * 100.f);
    return 0;
}

void Global::CopyData(Global* data)
{
    BONUSTIME            = data->BONUSTIME;
    TOTAL_SCORE          = data->TOTAL_SCORE;
    COIN_SCORE           = data->COIN_SCORE;
    STAR_SCORE           = data->STAR_SCORE;
    RUN_SCORE            = data->RUN_SCORE;
    COMBO_SCORE          = data->COMBO_SCORE;
    COMBO_LEVEL          = data->COMBO_LEVEL;
    BEST_COMBO           = data->BEST_COMBO;
    STAGE_LEVEL          = data->STAGE_LEVEL;
    NOTICE_LEVEL         = data->NOTICE_LEVEL;
    DEAD_TYPE            = data->DEAD_TYPE;
    LAST_SAVED_POINT     = data->LAST_SAVED_POINT;
    COIN_COUNT           = data->COIN_COUNT;
    STAR_COUNT           = data->STAR_COUNT;
    REVIVE_COUNT         = data->REVIVE_COUNT;
    GIANT_SCORE_TOTAL    = data->GIANT_SCORE_TOTAL;
    GIANT_SCORE_BULLET   = data->GIANT_SCORE_BULLET;
    GIANT_SCORE_MISSILE  = data->GIANT_SCORE_MISSILE;
    GIANT_SCORE_STICK    = data->GIANT_SCORE_STICK;
    MAGNET_SCORE         = data->MAGNET_SCORE;
    BARRIER_SCORE        = data->BARRIER_SCORE;
    GIANT_COUNT_TOTAL    = data->GIANT_COUNT_TOTAL;
    GIANT_COUNT_BULLET   = data->GIANT_COUNT_BULLET;
    GIANT_COUNT_MISSILE  = data->GIANT_COUNT_MISSILE;
    GIANT_COUNT_STICK    = data->GIANT_COUNT_STICK;
    MAGNET_COUNT         = data->MAGNET_COUNT;
    BARRIER_COUNT        = data->BARRIER_COUNT;
    COIN_ITEM_USE        = data->COIN_ITEM_USE;
    STAR_ITEM_USE        = data->STAR_ITEM_USE;
    BONUS_ITEM_USE       = data->BONUS_ITEM_USE;
    GIANT_ITEM_USE       = data->GIANT_ITEM_USE;
    MAGNET_ITEM_USE      = data->MAGNET_ITEM_USE;
    BARRIER_ITEM_USE     = data->BARRIER_ITEM_USE;
    NORMAL_ACHIEVEMENT_CLEAR_COUNT  = data->NORMAL_ACHIEVEMENT_CLEAR_COUNT;
    HIDDEN_ACHIEVEMENT_CLEAR_COUNT  = data->HIDDEN_ACHIEVEMENT_CLEAR_COUNT;
    STAR_BARRIER_COUNT   = data->STAR_BARRIER_COUNT;
    COIN_BARRIER_COUNT   = data->COIN_BARRIER_COUNT;
    CURRENT_CHARACTER    = data->CURRENT_CHARACTER;
    CURRENT_COSTUME      = data->CURRENT_COSTUME;
    CURRENT_PLANET       = data->CURRENT_PLANET;
}
