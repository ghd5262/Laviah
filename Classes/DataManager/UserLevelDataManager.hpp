#pragma once
#include <iostream>
#include <vector>
#include "../json/json.h"

struct USER_LEVEL{
    int _index;
    int _levelExp;
    int _sumExp;
    
    USER_LEVEL()
    : _index(0)
    , _levelExp(0)
    , _sumExp(0){}
};

typedef std::vector<USER_LEVEL> LEVEL_LIST;
class CUserLevelDataManager{
public:
    static CUserLevelDataManager* Instance();
    bool IsMaxLevel(int level);
    bool LevelUpCheck(int oldLevel, int newExp);
    USER_LEVEL getLevelDataByIndex(int index);

private:
    void initWithJson(std::string fileName);
    void addLevelDataToList(LEVEL_LIST& list,
                            const Json::Value& json);
    
    CUserLevelDataManager();
    ~CUserLevelDataManager(){};
    
private:
    LEVEL_LIST m_LevelList;
};
