#include "ChallengeClearChecker.h"
#include "../UserDataManager.h"
#include "../ChallengeDataManager.hpp"
#include "../WorkshopItemDataManager.h"

using namespace cocos2d;

bool CChallengeClearChecker::continuingTypeCheck(int index)
{
    auto savedDataList = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_VALUE_LIST);
    auto challengeList = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_LIST);
    auto challengeData = CChallengeDataManager::Instance()->getChallengeByIndex(challengeList->at(index));
    
    auto fail = [=](std::string msg){
        CCLOG("%s", msg.c_str());
        CCASSERT(false, StringUtils::format("%s", msg.c_str()).c_str());
        return false;
    };
    
    if(challengeData->_materialList.size() <= 0)
        return fail("There is no mtrl values.");
    
    
    if(challengeData->_materialList.size() > 1)
        return fail("Continuing type challenges can not have two or more mtrl values.");
    
    for(auto material : challengeData->_materialList)
    {
        if(material.second <= savedDataList->at(index))
        {
            return true;
        }
    }
    return false;
}

bool CChallengeClearChecker::coinScoreCheck(int value)      { return value >= GLOBAL->COIN_SCORE; }
bool CChallengeClearChecker::starScoreCheck(int value)      { return value >= GLOBAL->STAR_SCORE; }
bool CChallengeClearChecker::runScoreCheck(int value)       { return value >= GLOBAL->RUN_SCORE; }

bool CChallengeClearChecker::bestScoreCheck(int value) {
    auto bestScore = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BEST_SCORE);
    return (value >= bestScore);
}

bool CChallengeClearChecker::bestComboCheck(int value) {
    auto bestCombo = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BEST_COMBO);
    return (value >= bestCombo);
}

bool CChallengeClearChecker::characterCollectCheck(int value) {
    return CUserDataManager::Instance()->getUserData_IsItemHave(USERDATA_KEY::CHARACTER_LIST, value);
}

bool CChallengeClearChecker::rocketCollectCheck(int value) {
    return CUserDataManager::Instance()->getUserData_IsItemHave(USERDATA_KEY::ROCKET_LIST, value);
}

bool CChallengeClearChecker::characterCountCheck(int value) {
    auto count = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHARACTER_LIST)->size();
    return (value >= count);
}

bool CChallengeClearChecker::rocketCountCheck(int value) {
    auto count = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::ROCKET_LIST)->size();
    return (value >= count);
}

bool CChallengeClearChecker::userLevelCheck(int value) {
    auto level = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL);
    return (value >= level);
}

bool workshopLevelCheck(std::string key, int material) {
    auto level = CUserDataManager::Instance()->getUserData_Number(key);
    return (material <= level);
}

bool CChallengeClearChecker::coinItemLevelCheck(int value) {
    return workshopLevelCheck(USERDATA_KEY::ITEM_TIME_COIN, value);
}

bool CChallengeClearChecker::starItemLevelCheck(int value) {
    return workshopLevelCheck(USERDATA_KEY::ITEM_TIME_STAR, value);
}

bool CChallengeClearChecker::bonusItemLevelCheck(int value) {
    return workshopLevelCheck(USERDATA_KEY::ITEM_TIME_BOUNS, value);
}

bool CChallengeClearChecker::giantItemLevelCheck(int value) {
    return workshopLevelCheck(USERDATA_KEY::ITEM_TIME_GIANT, value);
}

bool CChallengeClearChecker::magnetItemLevelCheck(int value) {
    return workshopLevelCheck(USERDATA_KEY::ITEM_TIME_MAGNET, value);
}

bool CChallengeClearChecker::magnetSizeLevelCheck(int value) {
    return workshopLevelCheck(USERDATA_KEY::ITEM_SIZE_MAGNET, value);
}

bool CChallengeClearChecker::giantScoreTotal(int value)     { return value >= GLOBAL->GIANT_SCORE_TOTAL; }
bool CChallengeClearChecker::giantScoreBullet(int value)    { return value >= GLOBAL->GIANT_SCORE_BULLET; }
bool CChallengeClearChecker::giantScoreMissile(int value)   { return value >= GLOBAL->GIANT_SCORE_MISSILE; }
bool CChallengeClearChecker::giantScoreStick(int value)     { return value >= GLOBAL->GIANT_SCORE_STICK; }
bool CChallengeClearChecker::magnetScore(int value)         { return value >= GLOBAL->MAGNET_SCORE; }
bool CChallengeClearChecker::barrierScore(int value)        { return value >= GLOBAL->BARRIER_SCORE; }

bool CChallengeClearChecker::giantCountTotal(int value)     { return value >= GLOBAL->GIANT_COUNT_TOTAL; }
bool CChallengeClearChecker::giantCountBullet(int value)    { return value >= GLOBAL->GIANT_COUNT_BULLET; }
bool CChallengeClearChecker::giantCountMissile(int value)   { return value >= GLOBAL->GIANT_COUNT_MISSILE; }
bool CChallengeClearChecker::giantCountStick(int value)     { return value >= GLOBAL->GIANT_COUNT_STICK; }
bool CChallengeClearChecker::magnetCount(int value)         { return value >= GLOBAL->MAGNET_COUNT; }
bool CChallengeClearChecker::barrierCount(int value)        { return value >= GLOBAL->BARRIER_COUNT; }

bool CChallengeClearChecker::coinItemUse(int value)         { return value >= GLOBAL->COIN_ITEM_USE; }
bool CChallengeClearChecker::starItemUse(int value)         { return value >= GLOBAL->STAR_ITEM_USE; }
bool CChallengeClearChecker::bonusItemUse(int value)        { return value >= GLOBAL->BONUS_ITEM_USE; }
bool CChallengeClearChecker::giantItemUse(int value)        { return value >= GLOBAL->GIANT_ITEM_USE; }
bool CChallengeClearChecker::magnetItemUse(int value)       { return value >= GLOBAL->MAGNET_ITEM_USE; }
bool CChallengeClearChecker::barrierItemUse(int value)      { return value >= GLOBAL->BARRIER_ITEM_USE; }

bool CChallengeClearChecker::comboCheck(int value)          { return value >= GLOBAL->COMBO; }

bool CChallengeClearChecker::coinCheck(int value) {
    auto coin = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::COIN);
    return (value >= coin);
}
