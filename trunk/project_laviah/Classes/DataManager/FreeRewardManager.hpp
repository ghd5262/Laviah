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
    long long getNextRewardRemainTime();
    void FreeRewardLevelUP();
    
    CC_SYNTHESIZE(bool, m_RewardAble, RewardAble);
private:
    CFreeRewardManager();
    virtual ~CFreeRewardManager();
    
private:
    LIMIT_TIME_LIST m_LimitTimeList;
};
