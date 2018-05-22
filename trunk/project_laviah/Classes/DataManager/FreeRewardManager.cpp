#include "FreeRewardManager.hpp"
#include "UserDataManager.h"

using namespace cocos2d;

CFreeRewardManager::CFreeRewardManager()
: m_RewardAble(false)
{
    m_LimitTimeList.emplace_back(0LL);
    m_LimitTimeList.emplace_back(300LL);
    m_LimitTimeList.emplace_back(900LL);
    m_LimitTimeList.emplace_back(1800LL);
    m_LimitTimeList.emplace_back(3600LL);
    m_LimitTimeList.emplace_back(10800LL);
    m_LimitTimeList.emplace_back(23400LL);
}

CFreeRewardManager::~CFreeRewardManager(){}

CFreeRewardManager* CFreeRewardManager::Instance()
{
    static CFreeRewardManager instance;
    return &instance;
}

long long CFreeRewardManager::getLimitTimeByIndex(int index)
{
    if(m_LimitTimeList.size() <= index) return 23400LL;
    
    return m_LimitTimeList.at(index);
}

long long CFreeRewardManager::getFreeRewardTimeLimit()
{
    auto currentLevel = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::FREE_REWARD_LEVEL);
    return this->getLimitTimeByIndex(currentLevel);
}

long long CFreeRewardManager::getNextRewardRemainTime()
{
    auto currentLevel = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::FREE_REWARD_LEVEL);
    return this->getLimitTimeByIndex(currentLevel + 1);
}

void CFreeRewardManager::FreeRewardLevelUP()
{
    auto currentLevel = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::FREE_REWARD_LEVEL);
    if(currentLevel >= m_LimitTimeList.size()-1) return;
    
    CUserDataManager::Instance()->setUserData_NumberAdd(USERDATA_KEY::FREE_REWARD_LEVEL, 1);
}
