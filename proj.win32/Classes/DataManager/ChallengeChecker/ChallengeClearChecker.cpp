#include "ChallengeClearChecker.h"
#include "../UserDataManager.h"
#include "../ChallengeDataManager.hpp"
#include "../WorkshopItemDataManager.h"
#include "../CharacterDataManager.h"
#include "../DataManagerUtils.h"

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

bool CChallengeClearChecker::checkWithSingleUserData(std::string key, int value)
{
	return (value <= CUserDataManager::Instance()->getUserData_Number(key));
}

bool CChallengeClearChecker::checkWithItemExist(std::string key, int value)
{
	return CUserDataManager::Instance()->getUserData_IsItemHave(key, value);
}

bool CChallengeClearChecker::checkWithCount(std::string key, int value)
{
	return (value <= CUserDataManager::Instance()->getUserData_List(key).size());
}

bool CChallengeClearChecker::characterRareCountCheck(int value)
{
	auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHARACTER_LIST);
	if (!list.size()) return false;

	auto rareList = DATA_MANAGER_UTILS::getNonPtrListByFunc([=](int index){
		auto data = CCharacterDataManager::Instance()->getCharacterByIndex(index);
		return (data->_grade == CHARACTER_GRADE::RARE);
	}, list);
	return rareList.size();
}

bool CChallengeClearChecker::rocketRareCountCheck(int value)
{
	return false;
}