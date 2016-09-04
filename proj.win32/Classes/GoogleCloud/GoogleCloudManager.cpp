#include "GoogleCloudManager.h"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/WorkshopItemDataManager.h"
#include "../json/json.h"
#include "../SDKUtil/SDKUtil.h"
using namespace std;

CGoogleCloudManager::CGoogleCloudManager()
	: m_isConnected(false)
    , m_GoogleCloudDataLoadCount(0)
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
    
	CUserDataManager::Instance()->googleCloudDataLoad(valueJson);

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
