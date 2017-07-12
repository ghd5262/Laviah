#include "GoogleCloudManager.h"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/WorkshopItemDataManager.h"
#include "../json/json.h"
#include "../SDKUtil/SDKUtil.h"
#include <time.h>
using namespace std;

CGoogleCloudManager::CGoogleCloudManager()
	: m_isConnected(false)
	//, m_GoogleCloudDataLoadCount(0)
	, m_LastSavedTime(0LL)
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
    
//	CUserDataManager::Instance()->googleCloudDataLoad(valueJson);

	// 데이터 한번에 저장 및 로드를 위해 주석처리 - 2016. 9. 3
    /*CUserDataManager::Instance()->convertJsonToUserData(key, valueJson);
    CUserDataManager::Instance()->callbackFirstRevision();
    CUserDataManager::Instance()->overwriteXmlByGoogleCloud(key, valueJson);
    m_GoogleCloudDataLoadCount++;
    
    if(m_GoogleCloudDataLoadCount >= CUserDataManager::Instance()->getKeyList().size())
    {
        CCLOG("Data load finished from google cloud");
        CSDKUtil::Instance()->Toast("Load finished from google cloud");
        CUserDataManager::Instance()->setIsDataLoadFinish(true);
    }*/
}

void CGoogleCloudManager::AddDataToAutoSaveList(std::string key, std::string valueJson)
{
	CCLOG("=========================DataAutoSave=========================");
	CCLOG("Crypto Key : %s", key.c_str());
	CCLOG("Crypto Value : %s", valueJson.c_str());
	CCLOG("==============================================================");

	CSDKUtil::Instance()->AddDataToAutoSaveList(key, valueJson);
}

void CGoogleCloudManager::setLastSavedTime(long long unixTime)
{
	m_LastSavedTime = unixTime;
	time_t timer = m_LastSavedTime;
	auto tm = localtime(&timer);
	CUserDataManager::Instance()->setLastSavedTime(m_LastSavedTime);

	CCLOG("The last of save time %lld", m_LastSavedTime);
	CCLOG("YY : %d MM : %d DD : %d hh : %d mm : %d ss : %d", 
		tm->tm_year + 1900,
		tm->tm_mon + 1,
		tm->tm_mday,
		tm->tm_hour,
		tm->tm_min,
		tm->tm_sec);
}
