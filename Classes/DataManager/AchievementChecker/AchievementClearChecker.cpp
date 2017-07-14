#include "AchievementClearChecker.h"
#include "../UserDataManager.h"
#include "../AchievementDataManager.hpp"
#include "../WorkshopItemDataManager.h"
#include "../CharacterDataManager.h"
#include "../DataManagerUtils.h"
#include "../RocketDataManager.hpp"

using namespace cocos2d;

bool CAchievementClearChecker::checkWithGlobal(std::string key, int value)
{
    return value <= GVALUE->getVariable(key);
}

bool CAchievementClearChecker::checkWithSingleUserData(std::string key, int value)
{
	return (value <= CUserDataManager::Instance()->getUserData_Number(key));
}

bool CAchievementClearChecker::checkWithItemExist(std::string key, int value)
{
	return CUserDataManager::Instance()->getUserData_IsItemHave(key, value);
}

bool CAchievementClearChecker::checkWithCount(std::string key, int value)
{
	return (value <= CUserDataManager::Instance()->getUserData_List(key).size());
}

bool CAchievementClearChecker::checkWithItemParam(std::string key, int itemIndex,
                                                  int paramIndex, int value)
{
    return (value <= CUserDataManager::Instance()->getUserData_ParamData(key,
                                                                         itemIndex,
                                                                         paramIndex, 0));
}

bool CAchievementClearChecker::checkWithContinuingType(std::string key, int value)
{
    return false;
}

bool CAchievementClearChecker::characterRareCountCheck(int value, int& currentValue)
{
	auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHARACTER_LIST);
	if (!list.size()) return false;

	auto rareList = DATA_MANAGER_UTILS::getNonPtrListByFunc([=](int index){
		auto data = CCharacterDataManager::Instance()->getCharacterByIndex(index);
		return (data->_grade == CHARACTER_GRADE::RARE);
	}, list);
    
    currentValue += rareList.size();
	return (value <= rareList.size());
}

bool CAchievementClearChecker::rocketRareCountCheck(int value, int& currentValue)
{
//    auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::ROCKET_LIST);
//    if (!list.size()) return false;
//    
//    auto rareList = DATA_MANAGER_UTILS::getNonPtrListByFunc([=](int index){
//        auto data = CRocketDataManager::Instance()->getRocketByIndex(index);
//        return (data->_grade == ROCKET::RARE);
//    }, list);
//    
//    currentValue += rareList.size();
//    return (value <= rareList.size());
    return false;
}
