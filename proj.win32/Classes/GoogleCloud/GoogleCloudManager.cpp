#include "GoogleCloudManager.h"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/WorkshopItemDataManager.h"
#include "../json/json.h"
using namespace std;



CGoogleCloudManager::CGoogleCloudManager()
	: m_isConnected(false)
{
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_DATA_SAVE_REVISION", std::bind(&CUserDataManager::DataLoad_UserDataSaveRevision, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_LEVEL", std::bind(&CUserDataManager::DataLoad_UserLevel, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_COIN", std::bind(&CUserDataManager::DataLoad_UserCoin, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_HEALTH_IDX", std::bind(&CUserDataManager::DataLoad_UserHealth, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_STAR_LIMIT_TIME_IDX", std::bind(&CUserDataManager::DataLoad_UserStarItemLimitTime, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_COIN_LIMIT_TIME_IDX", std::bind(&CUserDataManager::DataLoad_UserCoinItemLimitTime, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_GIANT_LIMIT_TIME_IDX", std::bind(&CUserDataManager::DataLoad_UserGiantItemLimitTime, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_MAGNET_LIMIT_TIME_IDX", std::bind(&CUserDataManager::DataLoad_UserMagnetItemLimitTime, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_BONUS_LIMIT_TIME_IDX", std::bind(&CUserDataManager::DataLoad_UserBonusItemLimitTime, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_MAGNET_LIMIT_SIZE_IDX", std::bind(&CUserDataManager::DataLoad_UserMagnetItemLimitRadius, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_BEST_TOTAL_SCORE", std::bind(&CUserDataManager::DataLoad_UserBestTotalScore, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_BEST_COMBO", std::bind(&CUserDataManager::DataLoad_UserBestCombo, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_CUR_CHARACTER", std::bind(&CUserDataManager::DataLoad_UserCurrentCharacter, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_CUR_PET", std::bind(&CUserDataManager::DataLoad_UserCurrentPet, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_CUR_ROCKET", std::bind(&CUserDataManager::DataLoad_UserCurrentRocket, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_CUR_CHALLENGE", std::bind(&CUserDataManager::DataLoad_UserCurrentChallenge, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_CHARACTER_LIST", std::bind(&CUserDataManager::DataLoad_UserCharacterList, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_PET_LIST", std::bind(&CUserDataManager::DataLoad_UserPetList, CUserDataManager::Instance(), std::placeholders::_1)));
	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_ROCKET_LIST", std::bind(&CUserDataManager::DataLoad_UserRocketList, CUserDataManager::Instance(), std::placeholders::_1)));
}

CGoogleCloudManager::~CGoogleCloudManager()
{
}

CGoogleCloudManager* CGoogleCloudManager::Instance()
{
	static CGoogleCloudManager instance;
	return &instance;
}

void CGoogleCloudManager::GoogleCloudDataSave(std::string key, std::string value)
{

}

void CGoogleCloudManager::GoogleCloudDataLoad(std::string key, std::string value)
{
	auto func = m_DataLoadFuncList.find(key)->second;
	if (func != nullptr){

		CCLOG("===========================GoogleCloudLoad============================");
		CCLOG("Key : %s", key.c_str());
		CCLOG("Value : %s", value.c_str());
		CCLOG("======================================================================");

		func(value);
	}
	else{
		CCLOG("There is no function associated with that key. : %s", key.c_str());
		CCASSERT(false, "Wrong key");
	}
}
