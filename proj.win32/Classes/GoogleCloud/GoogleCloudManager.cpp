#include "GoogleCloudManager.h"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/WorkshopItemDataManager.h"
#include "../json/json.h"
#include "../SDKUtil/SDKUtil.h"
using namespace std;

CGoogleCloudManager::CGoogleCloudManager()
	: m_isConnected(false)
{}

CGoogleCloudManager::~CGoogleCloudManager()
{}

CGoogleCloudManager* CGoogleCloudManager::Instance()
{
	static CGoogleCloudManager instance;
	return &instance;
}

void CGoogleCloudManager::GoogleCloudDataSave(std::string key, std::string valueJson)
{
    CCLOG("=======================GoogleCloudSave========================");
    CCLOG("Crypto Key : %s", key.c_str());
    CCLOG("Crypto Value : %s", valueJson.c_str());
    CCLOG("==============================================================");
    CSDKUtil::Instance()->GoogleCloudSave(key, valueJson);
}

void CGoogleCloudManager::GoogleCloudDataLoad(std::string key, std::string valueJson)
{
    CCLOG("=======================GoogleCloudLoad========================");
    CCLOG("Crypto Key : %s", key.c_str());
    CCLOG("Crypto Value : %s", valueJson.c_str());
    CCLOG("==============================================================");
    
    CUserDataManager::Instance()->convertJsonToUserData(key, valueJson);
    CUserDataManager::Instance()->callbackFirstRevision();
}
