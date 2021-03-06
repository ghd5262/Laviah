#pragma once

#include <stdio.h>
#include <map>
class Global{
public:
    static Global* Instance();
    static Global* SavedData();
    void Clear();
    int getVariable(std::string key);
    void CopyData(Global* data);
    
    static int getPercent(float value, float max);

private:
    void addVariableToList();
    
    Global();
    ~Global(){};
    
public:
    std::map<std::string, int&> m_VariableList;
    
    int BONUSTIME;
    
    int TOTAL_SCORE;
    int COIN_SCORE;
    int STAR_SCORE;
    int RUN_SCORE;
    int COMBO_SCORE;
    int COMBO_LEVEL;
    int BEST_COMBO;
    int STAGE_LEVEL;
    int NOTICE_LEVEL;
    int DEAD_TYPE;
    int LAST_SAVED_POINT;
    
    int COIN_COUNT;
    int STAR_COUNT;
    int REVIVE_COUNT;
    
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
    
    int NORMAL_ACHIEVEMENT_CLEAR_COUNT;
    int HIDDEN_ACHIEVEMENT_CLEAR_COUNT;
    
    int STAR_BARRIER_COUNT;
    int COIN_BARRIER_COUNT;
    
    int CURRENT_CHARACTER;
    int CURRENT_COSTUME;
    int CURRENT_PLANET;
    
private:
    static Global* CurrentValue;
    static Global* SavedValue;
};

#define GVALUE Global::Instance()
#define GSAVED Global::SavedData()
