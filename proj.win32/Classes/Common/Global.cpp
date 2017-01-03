#include "Global.hpp"

Global::Global()
{
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

