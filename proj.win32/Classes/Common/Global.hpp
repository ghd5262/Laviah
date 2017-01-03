#pragma once

#include <stdio.h>

class Global{
public:
    static Global* Instance();
    void Clear();
    
private:
    Global();
    ~Global(){};
    
public:
    int BONUSTIME;        
    int COMBO;         
    int CHALLENGE_CLEAR_COUNT;
    int TOTAL_SCORE;
    int STAR_SCORE;
    int COIN_SCORE;
    int RUN_SCORE;
    
    int GIANT_SCORE_TOTAL;
    int GIANT_SCORE_BULLET;
    int GIANT_SCORE_MISSILE;
    int GIANT_SCORE_STICK;
    int MAGNET_SCORE;
    int BARRIER_SCORE;
    
    int GIANT_COUNT_TOTAL;
    int GIANT_COUNT_BULLET;
    int GIANT_COUNT_MISSILE;
    int GIANT_COUNT_STICK;
    int MAGNET_COUNT;
    int BARRIER_COUNT;
    
    int COIN_ITEM_USE;
    int STAR_ITEM_USE;
    int BONUS_ITEM_USE;
    int GIANT_ITEM_USE;
    int MAGNET_ITEM_USE;
    int BARRIER_ITEM_USE;
};

#define GLOBAL Global::Instance()