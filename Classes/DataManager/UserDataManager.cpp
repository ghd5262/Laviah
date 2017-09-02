#include "UserDataManager.h"
#include "DataManagerUtils.h"
#include "../Scene/GameScene.h"
#include "../MyUI/MyButton.h"
#include "../MyUI/Popup.h"
#include "../MyUI/Popup/EarnCoinPopup.h"
#include "../json/json.h"
#include "../DataManager/WorkshopItemDataManager.h"
#include "../DataManager/CharacterDataManager.h"
#include "../DataManager/UserLevelDataManager.hpp"
#include "../Download/DownloadManager.h"
#include "../SDKBOX/SDKBoxHeaders.h"
#include <algorithm>     /* qsort */

//#define JSONREADER_CREATE \
//	Json::Value root; \
//	Json::Reader reader; \
//	bool parsingSuccessful = reader.parse(valueJson, root); \
//	if (!parsingSuccessful) \
//	{ \
//		CCLOG("Error : Failed to parse configuration\n %s", reader.getFormatedErrorMessages().c_str()); \
//		return; \
//	}
//
//#define JSONWRITER_CREATE \
//	Json::Value root; \
//	Json::Value data; \
//	Json::StyledWriter writer;

// 오름차순 compare
int compare (int a, int b)
{
    return ( a < b );
}

namespace USERDATA{
	//d8d56d34a90de34339f8a174523adaae - texturepacker global key
	static const std::string CRYPTO_KEY			  = "sktjdgmlq1024!";
	static const std::string SINGLE_DATA_KEY	  = "userDefaultDatas_Number";
	static const std::string ARRAY_DATA_KEY		  = "userDefaultDatas_List";
    static const std::string PARAM_ARRAY_DATA_KEY = "userDefaultDatas_Param";
	static const std::string GOOGLE_DATA_KEY	  = "USER_DATA";
};

CUserDataManager::CUserDataManager()
: m_JsonUserDataFromXML("")
, m_JsonUserDataFromCloud("")
, m_DataLoadFinished(false){}

CUserDataManager::~CUserDataManager()
{
    this->userDataClean(m_UserData);
}

CUserDataManager* CUserDataManager::Instance()
{
    static CUserDataManager instance;
    return &instance;
}

void CUserDataManager::Initialize()
{
    this->initUserDefaultValue(m_UserData);
    this->LoadUserDataFromXML();
    this->setDataLoadFinished(true);
    this->setIsFirstPlay(false);
    
    auto version = Application::getInstance()->getVersion();
    this->setLastResourcesVersion(version);
}

bool CUserDataManager::getIsFirstPlay()
{
    std::string crypto_key = MakeCryptoString(USERDATA_KEY::FIRST_LOAD, USERDATA::CRYPTO_KEY);
    return UserDefault::getInstance()->getBoolForKey(crypto_key.c_str(), true);
}

void CUserDataManager::setIsFirstPlay(bool isFirst)
{
    std::string crypto_key = MakeCryptoString(USERDATA_KEY::FIRST_LOAD, USERDATA::CRYPTO_KEY);
    UserDefault::getInstance()->setBoolForKey(crypto_key.c_str(), isFirst);
}

std::string CUserDataManager::getLastResourcesVersion()
{
    std::string crypto_key = MakeCryptoString(USERDATA_KEY::LAST_RES_VERSION, USERDATA::CRYPTO_KEY);
    return UserDefault::getInstance()->getStringForKey(crypto_key.c_str(), "");
}

void CUserDataManager::setLastResourcesVersion(std::string version)
{
    std::string crypto_key = MakeCryptoString(USERDATA_KEY::LAST_RES_VERSION, USERDATA::CRYPTO_KEY);
    UserDefault::getInstance()->setStringForKey(crypto_key.c_str(), version);
}

tm* CUserDataManager::getLastSavedTime()
{
	std::string crypto_key = MakeCryptoString(USERDATA_KEY::LAST_SAVED_TIME, USERDATA::CRYPTO_KEY);
	time_t timer = UserDefault::getInstance()->getDoubleForKey(crypto_key.c_str(), true);
	return localtime(&timer);
}

long long CUserDataManager::getFreeRewardTimestamp()
{
    std::string crypto_key = MakeCryptoString(USERDATA_KEY::FREE_REWARD_TIMESTAMP, USERDATA::CRYPTO_KEY);
    return UserDefault::getInstance()->getDoubleForKey(crypto_key.c_str(), true);
}

float CUserDataManager::getItemValueByItemIndex(int itemIndex)
{
    auto skillIndex     = CWorkshopItemDataManager::Instance()->getSkillIndexByItemIndex(itemIndex);
    return this->getItemValueBySkillIndex(skillIndex);
}

float CUserDataManager::getItemValueBySkillIndex(int skillIndex)
{
    auto characterIndex = this->getUserData_Number(USERDATA_KEY::CHARACTER);
    auto characterValue = CCharacterDataManager::Instance()->getDefaultValueBySkillIndex(characterIndex, skillIndex);
    
    auto workShopLevel  = this->getUserData_ParamData(USERDATA_KEY::ITEM_LEVEL,
                                                      skillIndex,
                                                      PARAM_WORKSHOP::ITEM_LEVEL,
                                                      0);
    auto workShopData   = CWorkshopItemDataManager::Instance()->getItemDataByIndex(skillIndex);
    auto userValue      = workShopData->_valuePerLevel * workShopLevel;
    return characterValue + userValue;
}

#pragma mark -
#pragma mark [ interface function getter ]
int CUserDataManager::getUserData_Number(std::string key)
{
    if(m_UserData._userDataIntMap.find(key) != m_UserData._userDataIntMap.end()){
        auto value = m_UserData._userDataIntMap.find(key)->second;
        return value;
    }
    CCLOG("There is no user data key : %s", key.c_str());
    CCASSERT(false, "Wrong Key");
}

ARRAY_DATA CUserDataManager::getUserData_List(std::string key)
{
    auto list = getUserData_ListRef(key);
    return *list;
}

PARAM_DATA_ARRAY CUserDataManager::getUserData_ParamList(std::string key)
{
    auto paramList = getUserData_ParamListRef(key);
    return *paramList;
}

int CUserDataManager::getUserData_ParamData(std::string key, int index, int paramIdx, int defaultValue)
{
    // Find the list as the key.
    auto list = this->getUserData_ParamList(key);
    
    // Find the PARAM_DATA as the index.
    auto iter = list.find(index);
    if(iter == list.end())
        return defaultValue;
    
    // Find the parameter as the paramIdx.
    auto paramDataList = iter->second;
    if(paramDataList->size() <= paramIdx)
        return defaultValue;
    
    return paramDataList->at(paramIdx);
}

bool CUserDataManager::getUserData_IsItemExist(std::string key, int itemIdx)
{
    auto list = this->getUserData_ParamList(key);
    
    // Find the PARAM_DATA as the index.
    auto iter = list.find(itemIdx);
    if(iter == list.end()) return false;
    else                   return true;
}

bool CUserDataManager::getUserData_IsItemExistWithParam(std::string key, int paramIdx, int value)
{
    auto list   = this->getUserData_ParamList(key);
    auto picked = DATA_MANAGER_UTILS::getMapByFunc([=](PARAM_DATA* data){
        if(data->size() <= paramIdx) return false;
        return (data->at(paramIdx) == value);
    }, list);
    return (picked.size());
}


#pragma mark -
#pragma mark [ interface function setter ]

void CUserDataManager::LoadUserDataFromXML()
{
    std::string crypto_key = MakeCryptoString(USERDATA::GOOGLE_DATA_KEY, USERDATA::CRYPTO_KEY);
    std::string crypto_value = UserDefault::getInstance()->getStringForKey(crypto_key.c_str(), "");
    std::string decrypto_value = MakeCryptoString(crypto_value, USERDATA::CRYPTO_KEY);
    this->convertJsonToUserData(m_UserData, decrypto_value);
}

void CUserDataManager::LoadUserDataFromCloud()
{
    std::string crypto_key = MakeCryptoString(USERDATA::GOOGLE_DATA_KEY, USERDATA::CRYPTO_KEY);
    CPlayManager::Instance()->DataLoad([=](std::string data){
        std::string decrypto_value = MakeCryptoString(data, USERDATA::CRYPTO_KEY);
        this->convertJsonToUserData(m_UserData, decrypto_value);
        
        CGameScene::getGameScene()->CreateAlertPopup()
        ->setPositiveButton([=](Node* sender){}, TRANSLATE("BUTTON_OK"))
        ->setMessage(TRANSLATE("DATA_LOAD_SUCCEED"))
        ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
        
    }, crypto_key);
}

void CUserDataManager::SaveUserData(bool saveToCloud/* = false*/, bool autoSave/* = false*/)
{
    this->setSaveRevision(getUserData_Number(USERDATA_KEY::DATA_REVISION) + 1);
    std::string jsonString   = "";
    this->convertUserDataToJson(m_UserData, jsonString);
    
    // crypto
    std::string crypto_key   = MakeCryptoString(USERDATA::GOOGLE_DATA_KEY, USERDATA::CRYPTO_KEY);
    std::string crypto_value = MakeCryptoString(jsonString, USERDATA::CRYPTO_KEY);
    
    // get saved data
    std::string savedValue   = UserDefault::getInstance()->getStringForKey(crypto_key.c_str(), "");
    if(crypto_value == savedValue) return;
    
    UserDefault::getInstance()->setStringForKey(crypto_key.c_str(), crypto_value);
    
    if (saveToCloud) {
        
        CPlayManager::Instance()->DataSave([=](std::string data){
            time_t time;
            std::time(&time);
            this->setLastSavedTime(time);
            
            if(!autoSave)
            {
                CGameScene::getGameScene()->CreateAlertPopup()
                ->setPositiveButton([=](Node* sender){}, TRANSLATE("BUTTON_OK"))
                ->setMessage(TRANSLATE("DATA_SAVE_SUCCEED"))
                ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
            }
        }, crypto_key.c_str(), crypto_value);
    }
}

void CUserDataManager::setSaveRevision(int value)
{
    auto iter = m_UserData._userDataIntMap.find(USERDATA_KEY::DATA_REVISION);
    if(iter != m_UserData._userDataIntMap.end())
        iter->second = value;
}

void CUserDataManager::setUserData_Number(std::string key, int value)
{
    bool save = true;
    auto iter = m_UserData._userDataIntMap.find(key);
    if(iter == m_UserData._userDataIntMap.end())
        m_UserData._userDataIntMap.emplace(std::pair<std::string, int>(key, value));
    else {
        if(iter->second != value) iter->second = value;
        else                      save = false;
    }
    
    if(save) this->SaveUserData();
}

void CUserDataManager::setUserData_NumberAdd(std::string key, int value)
{
    auto origin = this->getUserData_Number(key);
    this->setUserData_Number(key, origin + value);
}

void CUserDataManager::setUserData_ItemExist(std::string key, int itemIdx)
{
    auto paramListMap = this->getUserData_ParamListRef(key);
    auto iter = paramListMap->find(itemIdx);
    if (iter == paramListMap->end()) {
        // Add param to list.
        auto paramData = new PARAM_DATA();
        paramListMap->emplace(std::pair<int, PARAM_DATA*>(itemIdx, paramData));
    }
    
    this->SaveUserData();
}

// TODO: refactoring - initUserDefaultValue 부분과 비슷한 부분이 있다.
void CUserDataManager::setUserData_ItemParam(std::string key, int itemIdx, int paramIdx, int value)
{
    PARAM_DATA* paramData = nullptr;
    
    // Find the PARAM_DATA as the index.
    auto paramListMap  = this->getUserData_ParamListRef(key);
    auto iter = paramListMap->find(itemIdx);
    if (iter == paramListMap->end()) {
        // Add param to list.
        paramData  = new PARAM_DATA();
        paramListMap->emplace(std::pair<int, PARAM_DATA*>(itemIdx, paramData));
    }
    else paramData = iter->second;
    
    // Find the parameter as the paramIdx.
    if(paramData->size() <= paramIdx)
        paramData->resize(paramIdx+1);
    
    if(paramData->at(paramIdx) != value)
        paramData->at(paramIdx) = value;
    else
        return;
    
    this->SaveUserData();
}

void CUserDataManager::setUserData_Reset()
{
    this->userDataClean(m_UserData);
    this->initUserDefaultValue(m_UserData);
   	this->SaveUserData();
}

void CUserDataManager::setLastSavedTime(long long unixTime)
{
    UserDefault::getInstance()->setDoubleForKey(USERDATA_KEY::LAST_SAVED_TIME.c_str(), unixTime);
}

void CUserDataManager::setFreeRewardTimestamp(long long timestamp)
{
    UserDefault::getInstance()->setDoubleForKey(USERDATA_KEY::FREE_REWARD_TIMESTAMP.c_str(), timestamp);
}

bool CUserDataManager::CoinUpdate(int value)
{
    if((getUserData_Number(USERDATA_KEY::COIN) + value) >= 0) {
        this->setUserData_NumberAdd(USERDATA_KEY::COIN, value);
        return true;
    }
    
    CGameScene::getGameScene()->CreateAlertPopup()
    ->setPositiveButton([=](Node* sender){}, TRANSLATE("BUTTON_OK"))
    ->setMessage(TRANSLATE("ALERT_COIN_LESS"))
    ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
    
    return false;
}

void CUserDataManager::ExpAdd(int exp)
{
    auto oldExp   = this->getUserData_Number(USERDATA_KEY::EXP);
    auto oldLevel = this->getUserData_Number(USERDATA_KEY::LEVEL);
    
    auto newExp   = oldExp + exp;
    if(CUserLevelDataManager::Instance()->LevelUpCheck(oldLevel, newExp))
    {
        this->setUserData_Number(USERDATA_KEY::EXP, 0);
        this->setUserData_Number(USERDATA_KEY::LEVEL, oldLevel + 1);
    }
    else{
        this->setUserData_Number(USERDATA_KEY::EXP, newExp);
    }
}

int CUserDataManager::getUserDataSequenceFromList(std::string key, int itemIndex)
{
    auto curList = CUserDataManager::Instance()->getUserData_List(key);
    auto iter = std::find(curList.begin(), curList.end(), itemIndex);
    auto sequence = std::distance(curList.begin(), iter);
    return int(sequence);
}

void CUserDataManager::initUserDefaultValue(sUSER_DATA &data)
{
    // userDefaultDataList.json 읽음
    std::string jsonFile   = FileUtils::getInstance()->fullPathForFilename("userDefaultDataList.json");
    std::string jsonString = FileUtils::getInstance()->getStringFromFile(jsonFile);
    size_t pos = jsonString.rfind("}");
    jsonString = jsonString.substr(0, pos + 1);
    this->convertJsonToUserData(data, jsonString);
}

void CUserDataManager::convertJsonToUserData(sUSER_DATA &data, std::string json)
{
    // If there is no saved data. Use default data.
    if(json == "")
        return;
    
    Json::Value  root;
    Json::Reader reader;
    
    bool parsingSuccessful = reader.parse(json, root);
    if (!parsingSuccessful)
    {
        CCASSERT(false, MakeString("parser failed : \n %s", json.c_str()).c_str());
        return;
    }
    CCLOG("strUserDefaultDataList JSON : \n %s\n", json.c_str());
    
    // initializing single data ( number data )
    {
        auto singleDataArray = root[USERDATA::SINGLE_DATA_KEY];
        for (auto singleData : singleDataArray)
        {
            auto key         = singleData["dataKey"].asString();
            auto value       = singleData["value"].asInt();
            auto iter        = data._userDataIntMap.find(key);
            
            if(iter == data._userDataIntMap.end())
                data._userDataIntMap.emplace(std::pair<std::string, int>(key, value));
            else iter->second = value;
        }
    }
    
    // initializing array data ( list data )
    {
        auto listDataArray   = root[USERDATA::ARRAY_DATA_KEY];
        for (auto listData : listDataArray)
        {
            auto key         = listData["dataKey"].asString();
            auto valueArray  = listData["value"];
            auto iter        = data._userDataListMap.find(key);
            ARRAY_DATA* emptyList = nullptr;
            
            if(iter == data._userDataListMap.end()){
                emptyList    = new ARRAY_DATA();
                data._userDataListMap.emplace(std::pair<std::string, ARRAY_DATA*>(key, emptyList));
            }
            else emptyList   = iter->second;
            
            // clean list
            emptyList->clear();
            if(valueArray.isNull()) continue;
            for(auto value : valueArray)
                emptyList->emplace_back(value.asInt());
        }
    }
    
    // initializing param data
    {
        auto paramDataArray  = root[USERDATA::PARAM_ARRAY_DATA_KEY];
        for (auto paramData : paramDataArray)
        {
            auto key         = paramData["dataKey"].asString();
            auto paramArray  = paramData["value"];
            auto iter        = data._userDataParamListMap.find(key);
            PARAM_DATA_ARRAY* emptyParamArray = nullptr;
            
            if(iter == data._userDataParamListMap.end()){
                emptyParamArray = new PARAM_DATA_ARRAY();
                data._userDataParamListMap.emplace(std::pair<std::string, PARAM_DATA_ARRAY*>(key, emptyParamArray));
            }
            else emptyParamArray = iter->second;
            
            // clean list
            DATA_MANAGER_UTILS::mapDeleteAndClean(*emptyParamArray);
            if(paramArray.isNull()) continue;
            for(auto param : paramArray){
                auto emptyParam = new PARAM_DATA();
                auto index      = 0;
                
                emptyParamArray->emplace(std::pair<int, PARAM_DATA*>(param[index].asInt(), emptyParam));
                
                for(index = 1; index < param.size(); index++)
                    emptyParam->emplace_back(param[index].asInt());
            }
        }
    }
}

void CUserDataManager::convertUserDataToJson(sUSER_DATA &data, std::string &valueJson)
{
    Json::StyledWriter writer;
    Json::Value root;
    
    // add single data ( number data )
    {
        for(auto singleData : data._userDataIntMap)
        {
            Json::Value singleJsonValue;
            singleJsonValue["dataKey"] = singleData.first;
            singleJsonValue["value"]   = singleData.second;
            
            root[USERDATA::SINGLE_DATA_KEY].append(singleJsonValue);
        }
    }
    
    // add array data ( list data )
    {
        for(auto listData : data._userDataListMap)
        {
            Json::Value listJsonValue;
            listJsonValue["dataKey"]   = listData.first;
            
            for(auto index : *listData.second)
                listJsonValue["value"].append(static_cast<int>(index));
            
            root[USERDATA::ARRAY_DATA_KEY].append(listJsonValue);
        }
    }
    
    // add param data
    {
        for(auto paramListData : data._userDataParamListMap)
        {
            Json::Value paramListJsonValue;
            paramListJsonValue["dataKey"]  = paramListData.first;
            
            for(auto paramData : *paramListData.second)
            {
                //Append index
                auto index = paramData.first;

                Json::Value paramJsonValue;
                paramJsonValue.append(index);
                for(auto param : *paramData.second)
                    paramJsonValue.append(static_cast<int>(param));
                
                paramListJsonValue["value"].append(paramJsonValue);
            }
            root[USERDATA::PARAM_ARRAY_DATA_KEY].append(paramListJsonValue);
        }
    }
    
    valueJson = writer.write(root);
}

void CUserDataManager::overwriteXmlByCloud(std::string valueJson)
{
    CCLOG("Overwrite to xml");
    UserDefault::getInstance()->setStringForKey(USERDATA::GOOGLE_DATA_KEY.c_str(), valueJson);
}

ARRAY_DATA* CUserDataManager::getUserData_ListRef(std::string key)
{
    if(m_UserData._userDataListMap.find(key) != m_UserData._userDataListMap.end()){
        auto list = m_UserData._userDataListMap.find(key)->second;
        return list;
    }
    CCLOG("There is no user data key : %s", key.c_str());
    CCASSERT(false, "Wrong Key");
}

PARAM_DATA_ARRAY* CUserDataManager::getUserData_ParamListRef(std::string key)
{
    if(m_UserData._userDataParamListMap.find(key) != m_UserData._userDataParamListMap.end()){
        auto list = m_UserData._userDataParamListMap.find(key)->second;
        return list;
    }
    CCLOG("There is no user data key : %s", key.c_str());
    CCASSERT(false, "Wrong Key");
}

void CUserDataManager::userDataClean(sUSER_DATA &data)
{
    DATA_MANAGER_UTILS::mapDeleteAndClean(data._userDataListMap);
    
    for(auto paramArray : data._userDataParamListMap)
        DATA_MANAGER_UTILS::mapDeleteAndClean(*paramArray.second);
    
    DATA_MANAGER_UTILS::mapDeleteAndClean(data._userDataParamListMap);
}

void CUserDataManager::sortUserDataList(std::string key, const LIST_COMPARE& compare)
{
    auto list = CUserDataManager::Instance()->getUserData_ListRef(key);
    std::sort(list->begin(), list->end(), compare);
}
