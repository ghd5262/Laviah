#include "ChallengeClearChecker.h"
#include "../UserDataManager.h"
#include "../ChallengeDataManager.hpp"
#include "../WorkshopItemDataManager.h"

using namespace cocos2d;

bool CChallengeClearChecker::continuingTypeCheck(int index)
{
    /*auto savedDataList = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_VALUE_LIST);
    auto challengeList = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_LIST);
    auto challengeData = CChallengeDataManager::Instance()->getChallengeByIndex(challengeList.at(index));
    
    if(challengeData->_materialValue <= savedDataList.at(index))
    {
        return true;
    }
    
    return false;*/
	return false;
}

bool CChallengeClearChecker::checkWithGlobal(std::string key, int value)
{
    auto global = GLOBAL->getVariable(key);
    return value <= global;
}

bool CChallengeClearChecker::bestScoreCheck(int value) {
    auto bestScore = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BEST_SCORE);
    return (value <= bestScore);
}

bool CChallengeClearChecker::bestComboCheck(int value) {
    auto bestCombo = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BEST_COMBO);
    return (value <= bestCombo);
}

bool CChallengeClearChecker::characterCollectCheck(int value) {
    return CUserDataManager::Instance()->getUserData_IsItemHave(USERDATA_KEY::CHARACTER_LIST, value);
}

bool CChallengeClearChecker::rocketCollectCheck(int value) {
    return CUserDataManager::Instance()->getUserData_IsItemHave(USERDATA_KEY::ROCKET_LIST, value);
}

bool CChallengeClearChecker::characterCountCheck(int value) {
    auto count = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHARACTER_LIST).size();
	return (value <= count);
}

bool CChallengeClearChecker::rocketCountCheck(int value) {
    auto count = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::ROCKET_LIST).size();
	return (value <= count);
}

bool CChallengeClearChecker::userLevelCheck(int value) {
    auto level = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL);
	return (value <= level);
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

bool CChallengeClearChecker::coinCheck(int value) {
    auto coin = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::COIN);
	return (value <= coin);
}
