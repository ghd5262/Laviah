#include "GoogleCloudManager.h"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/WorkshopItemDataManager.h"
#include "../json/json.h"
using namespace std;

CGoogleCloudManager::CGoogleCloudManager()
	: m_isConnected(false)
{
//    auto keyList =  CUserDataManager::Instance()->getKeyList();
    
      // userDataLoad_Number 에 key 와 value를 전달하도록하는 함수포인터 저장
//    for(auto key : keyList)
//    {
//        m_DataLoadFuncList.emplace(std::pair<string, FUNC>
//                                   (key.c_str(),std::bind(&CUserDataManager::userDataLoad_Number,
//                                                CUserDataManager::Instance(),
//                                                          placeholders::_1,
//                                                          placeholders::_1)));
//    }
//
//    
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_DATA_SAVE_REVISION", std::bind(&CUserDataManager::dataLoad_UserDataSaveRevision, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_LEVEL", std::bind(&CUserDataManager::dataLoad_UserLevel, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_COIN", std::bind(&CUserDataManager::dataLoad_UserCoin, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_HEALTH_IDX", std::bind(&CUserDataManager::dataLoad_UserHealth, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_STAR_LIMIT_TIME_IDX", std::bind(&CUserDataManager::dataLoad_UserStarItemLimitTime, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_COIN_LIMIT_TIME_IDX", std::bind(&CUserDataManager::dataLoad_UserCoinItemLimitTime, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_GIANT_LIMIT_TIME_IDX", std::bind(&CUserDataManager::dataLoad_UserGiantItemLimitTime, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_MAGNET_LIMIT_TIME_IDX", std::bind(&CUserDataManager::dataLoad_UserMagnetItemLimitTime, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_BONUS_LIMIT_TIME_IDX", std::bind(&CUserDataManager::dataLoad_UserBonusItemLimitTime, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_MAGNET_LIMIT_SIZE_IDX", std::bind(&CUserDataManager::dataLoad_UserMagnetItemLimitRadius, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_BEST_TOTAL_SCORE", std::bind(&CUserDataManager::dataLoad_UserBestTotalScore, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_BEST_COMBO", std::bind(&CUserDataManager::dataLoad_UserBestCombo, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_CUR_CHARACTER", std::bind(&CUserDataManager::dataLoad_UserCurrentCharacter, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_CUR_PET", std::bind(&CUserDataManager::dataLoad_UserCurrentPet, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_CUR_ROCKET", std::bind(&CUserDataManager::dataLoad_UserCurrentRocket, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_CUR_CHALLENGE", std::bind(&CUserDataManager::dataLoad_UserCurrentChallenge, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_CHARACTER_LIST", std::bind(&CUserDataManager::dataLoad_UserCharacterList, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_PET_LIST", std::bind(&CUserDataManager::dataLoad_UserPetList, CUserDataManager::Instance(), std::placeholders::_1)));
//	m_DataLoadFuncList.emplace(std::pair<string, FUNC>("USER_ROCKET_LIST", std::bind(&CUserDataManager::dataLoad_UserRocketList, CUserDataManager::Instance(), std::placeholders::_1)));
}

CGoogleCloudManager::~CGoogleCloudManager()
{
}

CGoogleCloudManager* CGoogleCloudManager::Instance()
{
	static CGoogleCloudManager instance;
	return &instance;
}

void CGoogleCloudManager::GoogleCloudDataSave(std::string key, std::string valueJson)
{

}

void CGoogleCloudManager::GoogleCloudDataLoad(std::string key, std::string valueJson)
{
    CCLOG("===========================GoogleCloudLoad============================");
    CCLOG("Crypto Key : %s", key.c_str());
    CCLOG("Crypto Value : %s", valueJson.c_str());
    CCLOG("======================================================================");
    
    CUserDataManager::Instance()->userDataLoad(key, valueJson);
}
