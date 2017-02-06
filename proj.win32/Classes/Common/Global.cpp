#include "Global.hpp"
#include "../DataManager/ChallengeChecker/ChallengeClearChecker.h"

Global::Global()
{
    this->addVariableToList();
    this->Clear();
}

void Global::Clear()
{
    BONUSTIME = 0;
    COMBO = 0;
    CHALLENGE_CLEAR_COUNT = 0;
    TOTAL_SCORE = 0;
    STAR_SCORE = 0;
    COIN_SCORE = 0;
    RUN_SCORE = 0;
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
    
    addVariable( CHALLENGE_DATA_KEY::COMBO                 , COMBO                  );
    addVariable( CHALLENGE_DATA_KEY::TOTAL_SCORE           , TOTAL_SCORE            );
    addVariable( CHALLENGE_DATA_KEY::STAR_SCORE            , STAR_SCORE             );
    addVariable( CHALLENGE_DATA_KEY::COIN_SCORE            , COIN_SCORE             );
    addVariable( CHALLENGE_DATA_KEY::RUN_SCORE             , RUN_SCORE              );
    addVariable( CHALLENGE_DATA_KEY::STAR_COUNT            , STAR_COUNT             );
    addVariable( CHALLENGE_DATA_KEY::COIN_COUNT            , COIN_COUNT             );
    addVariable( CHALLENGE_DATA_KEY::REVIVE_COUNT          , REVIVE_COUNT           );

    addVariable( CHALLENGE_DATA_KEY::GIANT_SCORE_TOTAL     , GIANT_SCORE_TOTAL      );
    addVariable( CHALLENGE_DATA_KEY::GIANT_SCORE_BULLET    , GIANT_SCORE_BULLET     );
    addVariable( CHALLENGE_DATA_KEY::GIANT_SCORE_MISSILE   , GIANT_SCORE_MISSILE    );
    addVariable( CHALLENGE_DATA_KEY::GIANT_SCORE_STICK     , GIANT_SCORE_STICK      );
    addVariable( CHALLENGE_DATA_KEY::MAGNET_SCORE          , MAGNET_SCORE           );
    addVariable( CHALLENGE_DATA_KEY::BARRIER_SCORE         , BARRIER_SCORE          );
    
    addVariable( CHALLENGE_DATA_KEY::GIANT_COUNT_TOTAL     , GIANT_COUNT_TOTAL      );
    addVariable( CHALLENGE_DATA_KEY::GIANT_COUNT_BULLET    , GIANT_COUNT_BULLET     );
    addVariable( CHALLENGE_DATA_KEY::GIANT_COUNT_MISSILE   , GIANT_COUNT_MISSILE    );
    addVariable( CHALLENGE_DATA_KEY::GIANT_COUNT_STICK     , GIANT_COUNT_STICK      );
    addVariable( CHALLENGE_DATA_KEY::MAGNET_COUNT          , MAGNET_COUNT           );
    addVariable( CHALLENGE_DATA_KEY::BARRIER_COUNT         , BARRIER_COUNT          );
    
    addVariable( CHALLENGE_DATA_KEY::COIN_ITEM_USE         , COIN_ITEM_USE          );
    addVariable( CHALLENGE_DATA_KEY::STAR_ITEM_USE         , STAR_ITEM_USE          );
    addVariable( CHALLENGE_DATA_KEY::BONUS_ITEM_USE        , BONUS_ITEM_USE         );
    addVariable( CHALLENGE_DATA_KEY::GIANT_ITEM_USE        , GIANT_ITEM_USE         );
    addVariable( CHALLENGE_DATA_KEY::MAGNET_ITEM_USE       , MAGNET_ITEM_USE        );
    addVariable( CHALLENGE_DATA_KEY::BARRIER_ITEM_USE      , BARRIER_ITEM_USE       );
}