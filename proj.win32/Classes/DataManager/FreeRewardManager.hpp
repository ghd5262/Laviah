#pragma once
#include "../Common/HSHUtility.h"
#include <vector>

class CFreeRewardManager
{
    typedef std::vector<long long> LIMIT_TIME_LIST;
public:
    static CFreeRewardManager* Instance();
    long long getLimitTimeByIndex(int index);
    long long getFreeRewardTimeLimit();
    void FreeRewardLevelUP();
    
private:
    CFreeRewardManager();
    virtual ~CFreeRewardManager();
    
private:
    LIMIT_TIME_LIST m_LimitTimeList;
};