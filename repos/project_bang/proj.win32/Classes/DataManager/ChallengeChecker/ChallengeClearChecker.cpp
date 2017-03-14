#include "ChallengeClearChecker.h"
#include "../UserDataManager.h"
#include "../ChallengeDataManager.hpp"
#include "../WorkshopItemDataManager.h"
#include "../CharacterDataManager.h"
#include "../DataManagerUtils.h"

using namespace cocos2d;

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

bool CChallengeClearChecker::checkWithContinuingType(std::string key, int value)
{
    return false;
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