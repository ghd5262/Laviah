#include "AchievementClearChecker.h"
#include "../UserDataManager.h"
#include "../AchievementDataManager.hpp"
#include "../WorkshopItemDataManager.h"
#include "../CharacterDataManager.h"
#include "../DataManagerUtils.h"
#include "../RocketDataManager.hpp"

using namespace cocos2d;

bool CAchievementClearChecker::checkWithGlobal(std::string key, int value, bool isFit)
{
    if(isFit) return value == GVALUE->getVariable(key);
    else      return value <= GVALUE->getVariable(key);
}

bool CAchievementClearChecker::checkWithSingleUserData(std::string key, int value, bool isFit)
{
    if(isFit) return (value == CUserDataManager::Instance()->getUserData_Number(key));
    else      return (value <= CUserDataManager::Instance()->getUserData_Number(key));
}

bool CAchievementClearChecker::checkWithItemExist(std::string key, int value)
{
	return CUserDataManager::Instance()->getUserData_IsItemExist(key, value);
}

bool CAchievementClearChecker::checkWithCount(std::string key, int value)
{
	return (value <= CUserDataManager::Instance()->getUserData_ParamList(key).size());
}

bool CAchievementClearChecker::checkWithItemParam(std::string key, int itemIndex,
                                                  int paramIndex, int value, bool isFit)
{
    auto userValue = CUserDataManager::Instance()->getUserData_ParamData(key, itemIndex,
                                                                         paramIndex, 0);
    if(isFit) return (value == userValue);
    else      return (value <= userValue);
}

bool CAchievementClearChecker::checkWithContinuingType(std::string key, int value)
{
    return false;
}

bool CAchievementClearChecker::characterRareCountCheck(int value, int& currentValue)
{
//	auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHARACTER_LIST);
//	if (!list.size()) return false;
//
//	auto rareList = DATA_MANAGER_UTILS::getNonPtrListByFunc([=](int index){
//		auto data = CCharacterDataManager::Instance()->getCharacterByIndex(index);
//		return (data->_grade == CHARACTER_GRADE::RARE);
//	}, list);
//    
//    currentValue += rareList.size();
//	return (value <= rareList.size());
    return false;
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
