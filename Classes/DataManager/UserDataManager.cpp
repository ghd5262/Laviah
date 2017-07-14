#include "UserDataManager.h"
#include "DataManagerUtils.h"
#include "../Scene/GameScene.h"
#include "../MyUI/MyButton.h"
#include "../MyUI/Popup.h"
#include "../MyUI/Popup/EarnCoinPopup.h"
#include "../json/json.h"
#include "../DataManager/WorkshopItemDataManager.h"
#include "../SDKUtil/SDKUtil.h"
//#include "../Cloud/CloudManager.h"
#include "../SDKBOX/SDKBox.h"
//#include "../SDKBOX/Play/PlayManager.hpp"
#include "../Common/NoticeDefine.h"
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
, m_DataLoadFinished(false)
// 데이터 한번에 저장 및 로드를 위해 주석처리 - 2016. 9. 3
//: m_IsFirstRevisionCall(false)
//, m_IsDataLoadFinish(false)
{}

CUserDataManager::~CUserDataManager()
{
    auto cleanUserData = [=](sUSER_DATA& data){
        DATA_MANAGER_UTILS::mapDeleteAndClean(data._userDataListMap);
        
        for(auto paramArray : data._userDataParamListMap)
            DATA_MANAGER_UTILS::mapDeleteAndClean(*paramArray.second);
        
        DATA_MANAGER_UTILS::mapDeleteAndClean(data._userDataParamListMap);
    };
    
    cleanUserData(m_UserData);
}

CUserDataManager* CUserDataManager::Instance()
{
    static CUserDataManager instance;
    return &instance;
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

bool CUserDataManager::getIsFirstPlay()
{
    std::string crypto_key = MakeCryptoString(USERDATA_KEY::FIRST_LOAD, USERDATA::CRYPTO_KEY);
    return UserDefault::getInstance()->getBoolForKey(crypto_key.c_str(), true);
}

tm* CUserDataManager::getLastSavedTime()
{
	std::string crypto_key = MakeCryptoString(USERDATA_KEY::LAST_SAVED_TIME, USERDATA::CRYPTO_KEY);
	time_t timer = UserDefault::getInstance()->getDoubleForKey(crypto_key.c_str(), true);
	return localtime(&timer);
}

long long CUserDataManager::getLastTimestamp()
{
    std::string crypto_key = MakeCryptoString(USERDATA_KEY::LAST_TIMESTAMP, USERDATA::CRYPTO_KEY);
    return UserDefault::getInstance()->getDoubleForKey(crypto_key.c_str(), true);
}

long long CUserDataManager::getFreeRewardTimestamp()
{
    std::string crypto_key = MakeCryptoString(USERDATA_KEY::FREE_REWARD_TIMESTAMP, USERDATA::CRYPTO_KEY);
    return UserDefault::getInstance()->getDoubleForKey(crypto_key.c_str(), true);
}

void CUserDataManager::UserDataLoad()
{
    this->initUserDefaultValue(m_UserData);
    //TODO: FIX 이부분 문제 요지 (기기를 바꾸고 첫번째 플레이 시 구글 연결이 안된 경우 아래 루틴을 탄다.)
	if (this->getIsFirstPlay() &&
        CPlayManager::Instance()->IsLoggedIn() &&
        CSDKUtil::Instance()->getIsNetworkConnect())
	{
        CSDKUtil::Instance()->Toast("This is first play");
        
		// 첫 로드일 경우 구글 클라우드 로드를 기다린다.
		this->dataLoadFromCloud();
		this->dataLoadFromXML();
	}
	else
	{
        CSDKUtil::Instance()->Toast("This is not first play");
        
		// 첫 로드가 아닐 경우 xml에서 로드
		this->dataLoadFromXML();
		this->convertJsonToUserData(m_UserData, m_JsonUserDataFromXML);
        this->setDataLoadFinished(true);
        __NotificationCenter::getInstance()->postNotification(NOTICE::USERDATA_LOAD_FINISH, NULL);
	}
}

void CUserDataManager::dataLoadFromXML()
{
	std::string crypto_key = MakeCryptoString(USERDATA::GOOGLE_DATA_KEY, USERDATA::CRYPTO_KEY);
	std::string crypto_value = UserDefault::getInstance()->getStringForKey(crypto_key.c_str(), "");
	std::string decrypto_value = MakeCryptoString(crypto_value, USERDATA::CRYPTO_KEY);

	m_JsonUserDataFromXML = decrypto_value;
}

void CUserDataManager::dataLoadFromCloud()
{
	std::string crypto_key = MakeCryptoString(USERDATA::GOOGLE_DATA_KEY, USERDATA::CRYPTO_KEY);
	       
	// google sdk와 통신후 CloudDataLoad 호출됨
	// CloudDataLoad 호출 후에는 revision 비교 후 더 높은 것을 채택함
    CPlayManager::Instance()->DataLoad([=](std::string data){
        this->setUserData_CloudSaved(data);
    }, crypto_key);
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
    CSDKUtil::Instance()->Toast("Overwrite to xml");
    UserDefault::getInstance()->setStringForKey(USERDATA::GOOGLE_DATA_KEY.c_str(), valueJson);
}

bool CUserDataManager::isCloudRevisionHigher()
{
	int googleRevision = 0;
	int xmlRevision = 0;
	std::string key = USERDATA_KEY::DATA_REVISION;
	if (m_JsonUserDataFromCloud != "")
	{
		Json::Value googleJsonRoot;
		Json::Reader googleJsonReader;

		bool parsingSuccessful = googleJsonReader.parse(m_JsonUserDataFromCloud, googleJsonRoot);
		if (!parsingSuccessful)
		{
			CCASSERT(false, MakeString("parser failed : \n %s", m_JsonUserDataFromCloud.c_str()).c_str());
			return false;
		}
		CCLOG("UserData JSON : \n %s\n", m_JsonUserDataFromCloud.c_str());

		const Json::Value dataArray = googleJsonRoot["data"];
		googleRevision = dataArray[key.c_str()].asInt();
	}
	if (m_JsonUserDataFromXML != "")
	{
		Json::Value xmlJsonRoot;
		Json::Reader xmlJsonReader;

		bool parsingSuccessful = xmlJsonReader.parse(m_JsonUserDataFromXML, xmlJsonRoot);
		if (!parsingSuccessful)
		{
			CCASSERT(false, MakeString("parser failed : \n %s", m_JsonUserDataFromXML.c_str()).c_str());
			return false;
		}
		CCLOG("UserData JSON : \n %s\n", m_JsonUserDataFromXML.c_str());

		const Json::Value dataArray = xmlJsonRoot["data"];
		xmlRevision = dataArray[key.c_str()].asInt();
	}

    CCLOG("Compare revision - Google : %d vs XML : %d", googleRevision, xmlRevision);
    return (googleRevision > xmlRevision);
}

void CUserDataManager::saveUserDataToCloud(std::string key, std::string data, bool forceSave/*= false*/)
{
    if (CSDKUtil::Instance()->getIsNetworkConnect()){
        CPlayManager::Instance()->DataSave([=](std::string data){
            
            CGameScene::getGameScene()->CreateAlertPopup()
            ->setPositiveButton([=](Node* sender){}, TRANSLATE("BUTTON_OK"))
            ->setMessage("저장이 완료되었습니다.")
            ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
            
        }, key.c_str(), data);
    }
    else
    {
        if (!forceSave) return; //auto save
        
        CGameScene::getGameScene()->CreateAlertPopup()
        ->setPositiveButton([=](Node* sender){
            CSDKUtil::Instance()->setNetworkConnectSavedFunc([=](){
                this->saveUserDataToCloud(key, data, forceSave);
            });
            CSDKUtil::Instance()->IsNetworkConnect();
        }, TRANSLATE("BUTTON_OK"))
        ->setNegativeButton([=](Node* sender){}, TRANSLATE("BUTTON_CANCEL"))
        ->setMessage(TRANSLATE("OPTION_DATASAVE_NETWORK_CHECK"))
        ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
    }
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

void CUserDataManager::sortUserDataList(std::string key, const LIST_COMPARE& compare)
{
    auto list = CUserDataManager::Instance()->getUserData_ListRef(key);
    std::sort(list->begin(), list->end(), compare);
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

bool CUserDataManager::getUserData_IsItemHave(std::string key, int itemIdx)
{
    if(m_UserData._userDataListMap.find(key) == m_UserData._userDataListMap.end()){
        CCLOG("There is no list with this key %s", key.c_str());
        CCASSERT(false, "Wrong Key");
    }
    
    auto itemList = m_UserData._userDataListMap.find(key)->second;
    size_t listSize = itemList->size();
    
    if (listSize > 0)
    {
        for(auto item : *itemList)
        {
            if(item == itemIdx)
            return true;
        }
    }
    return false;
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

void CUserDataManager::SaveUserDataAutomatically()
{
    if(!this->getDataLoadFinished()) return;
    
    if(this->getUserData_Number(USERDATA_KEY::DATA_SAVE_AUTO))
        this->SaveUserData(true);
}

void CUserDataManager::SaveUserData(bool saveToCloud/* = false*/, bool forceSave/* = false*/)
{
    this->setSaveRevision(getUserData_Number(USERDATA_KEY::DATA_REVISION) + 1);
    std::string jsonString   = "";
    this->convertUserDataToJson(m_UserData, jsonString);
    
    // crypto
    std::string crypto_key   = MakeCryptoString(USERDATA::GOOGLE_DATA_KEY, USERDATA::CRYPTO_KEY);
    std::string crypto_value = MakeCryptoString(jsonString, USERDATA::CRYPTO_KEY);
    
    UserDefault::getInstance()->setStringForKey(crypto_key.c_str(), crypto_value);
    
    if (saveToCloud) {
        CSDKUtil::Instance()->setNetworkConnectSavedFunc([=](){
            this->saveUserDataToCloud(crypto_key, crypto_value, forceSave);
        });
        CSDKUtil::Instance()->IsNetworkConnect();
    }
}

void CUserDataManager::setSaveRevision(int value)
{
    auto iter = m_UserData._userDataIntMap.find(USERDATA_KEY::DATA_REVISION);
    if(iter != m_UserData._userDataIntMap.end())
        iter->second = value;
}

void CUserDataManager::setUserData_CloudSaved(std::string cryptoValue)
{
    std::string decrypto_value = MakeCryptoString(cryptoValue, USERDATA::CRYPTO_KEY);
    
    m_JsonUserDataFromCloud = decrypto_value;
    
    if (this->isCloudRevisionHigher())
    {
        CSDKUtil::Instance()->Toast("cloud revision");
        
        this->convertJsonToUserData(m_UserData, m_JsonUserDataFromCloud);
        this->overwriteXmlByCloud(m_JsonUserDataFromCloud);
        UserDefault::getInstance()->setBoolForKey(USERDATA_KEY::FIRST_LOAD.c_str(), false);
    }
    else
    {
        CSDKUtil::Instance()->Toast("xml revision");
        
        this->convertJsonToUserData(m_UserData, m_JsonUserDataFromXML);
    }
    this->setDataLoadFinished(true);
    __NotificationCenter::getInstance()->postNotification(NOTICE::USERDATA_LOAD_FINISH, NULL);
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

void CUserDataManager::setUserData_ItemGet(std::string key, int itemIdx)
{
    if(this->getUserData_IsItemHave(key, itemIdx))
    {
        CCLOG("Item get : Already have %s index %d", key.c_str(), itemIdx);
        return;
    }
    
    ARRAY_DATA* arrayData = nullptr;
	auto iter = m_UserData._userDataListMap.find(key);
    if (iter == m_UserData._userDataListMap.end()){
        arrayData = new ARRAY_DATA();
        m_UserData._userDataListMap.emplace(std::pair<std::string, ARRAY_DATA*>(key, arrayData));
    }
    else arrayData = iter->second;
    
    arrayData->push_back(itemIdx);
    
    this->sortUserDataList(key, compare);
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
    
    paramData->at(paramIdx) = value;
    
    this->SaveUserData();
}

void CUserDataManager::setUserData_ItemRemove(std::string key, int itemIdx)
{
	if (!this->getUserData_IsItemHave(key, itemIdx))
	{
		CCLOG("Item remove : Do not have %s index %d", key.c_str(), itemIdx);
		return;
	}

	auto itemList = m_UserData._userDataListMap.find(key);
	if (itemList != m_UserData._userDataListMap.end()){
		auto list = itemList->second;
		list->erase(std::remove(std::begin(*list), std::end(*list), itemIdx), std::end(*list));
        this->sortUserDataList(key, compare);
    }
	this->SaveUserData();
}

void CUserDataManager::setUserData_ItemRemoveAll(std::string key)
{
    auto itemList = m_UserData._userDataListMap.find(key);
    if (itemList != m_UserData._userDataListMap.end()){
        auto list = itemList->second;
        list->clear();
    }
	this->SaveUserData();
}

void CUserDataManager::setUserData_Reset()
{
    this->initUserDefaultValue(m_UserData);
   	this->SaveUserData();
}

void CUserDataManager::setLastSavedTime(long long unixTime)
{
    UserDefault::getInstance()->setDoubleForKey(USERDATA_KEY::LAST_SAVED_TIME.c_str(), unixTime);
}

void CUserDataManager::setLastTimestamp(long long timestamp)
{
    UserDefault::getInstance()->setDoubleForKey(USERDATA_KEY::LAST_TIMESTAMP.c_str(), timestamp);
}

void CUserDataManager::setFreeRewardTimestamp(long long timestamp)
{
    UserDefault::getInstance()->setDoubleForKey(USERDATA_KEY::FREE_REWARD_TIMESTAMP.c_str(), timestamp);
}

bool CUserDataManager::CoinUpdate(int value)
{
    bool result = false;
    if (value > 0)
    {
        result = true;
        this->setUserData_NumberAdd(USERDATA_KEY::COIN, value);
    }
    else
    {
        result = false;
        if ((value * -1) > getUserData_Number(USERDATA_KEY::COIN) )
        {
            Size visibleSize = Director::getInstance()->getVisibleSize();
            
            CEarnCoinPopup::create()
            ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
            ->setPopupPosition(visibleSize / 2)
            ->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
            ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
            ->show(Director::getInstance()->getRunningScene(), ZORDER::POPUP);
        }
        else
        {
            result = true;
            this->setUserData_NumberAdd(USERDATA_KEY::COIN, value);
        }
    }
    return result;
}

void CUserDataManager::ExpAdd(int exp)
{
    auto oldExp   = this->getUserData_Number(USERDATA_KEY::EXP);
    auto oldLevel = this->getUserData_Number(USERDATA_KEY::LEVEL);
    
    auto newExp   = oldExp + exp;
    auto newlevel = std::max(0, newExp / 300);
    
    this->setUserData_Number(USERDATA_KEY::EXP, newExp);
    
    if(oldLevel < newlevel)
        this->setUserData_Number(USERDATA_KEY::LEVEL, newlevel);
}

int CUserDataManager::getUserDataSequenceFromList(std::string key, int itemIndex)
{
    auto curList = CUserDataManager::Instance()->getUserData_List(key);
    auto iter = std::find(curList.begin(), curList.end(), itemIndex);
    auto sequence = std::distance(curList.begin(), iter);
    return int(sequence);
}


// 데이터 한번에 저장 및 로드를 위해 주석처리 - 2016. 9. 3
///* google login을 시도한 후에 호출됨 */
//void CUserDataManager::GoogleLoginResult()
//{
//    CCLOG("GoogleLoginResult : Called");
//    if (CCloudManager::Instance()->getIsConnected())
//    {
//        CCLOG("GoogleLoginResult : Call google revision function");
//        
//        // 리비전 비교 위해 함수 호출
//        std::string crypto_key = MakeCryptoString(USERDATA_KEY::DATA_REVISION, USERDATA::CRYPTO_KEY);
//        CSDKUtil::Instance()->CloudLoad(crypto_key.c_str());
//        
//        m_IsFirstRevisionCall = true;
//    }
//    else{
//        CCLOG("GoogleLoginResult : Data load from xml google cloud didn't connected");
//        
//        // XML로부터 데이터 로드
//        dataLoadFromXML();
//    }
//}
//
//#pragma mark -
//#pragma mark [ interface function getter ]
//int CUserDataManager::getUserData_Number(std::string key)
//{
//    if(m_UserData._userDataIntMap.find(key) != m_UserData._userDataIntMap.end()){
//        auto value = m_UserData._userDataIntMap.find(key)->second;
//        return value;
//    }
//    CCLOG("There is no user data key : %s", key.c_str());
//    CCASSERT(false, "Wrong Key");
//}
//
//DATA_LIST* CUserDataManager::getUserData_List(std::string key)
//{
//    if(m_UserData._userDataListMap.find(key) != m_UserData._userDataListMap.end()){
//        auto list = m_UserData._userDataListMap.find(key)->second;
//        return list;
//    }
//    CCLOG("There is no user data key : %s", key.c_str());
//    CCASSERT(false, "Wrong Key");
//}
//
//bool CUserDataManager::getUserData_IsItemHave(std::string key, int itemIdx)
//{
//    if(m_UserData._userDataListMap.find(key) == m_UserData._userDataListMap.end()){
//        CCLOG("There is no list with this key %s", key.c_str());
//        CCASSERT(false, "Wrong Key");
//    }
//    
//    auto itemList = m_UserData._userDataListMap.find(key)->second;
//    size_t listSize = itemList->size();
//    
//    if (listSize > 0)
//    {
//        for(auto item : *itemList)
//        {
//            if(item == itemIdx)
//            return true;
//        }
//    }
//    return false;
//}
//
//float CUserDataManager::getItemCurrentValue(std::string key)
//{
//    WORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByKey(key.c_str());
//    float limitTime = item._valuePerLevel * getUserData_Number(key.c_str());
//    return limitTime;
//}
//
//#pragma mark -
//#pragma mark [ interface function setter ]
//void CUserDataManager::setSaveRevision(int value)
//{
//    if(m_UserData._userDataIntMap.find(USERDATA_KEY::DATA_REVISION) != m_UserData._userDataIntMap.end())
//        m_UserData._userDataIntMap.find(USERDATA_KEY::DATA_REVISION)->second = value;
//    
//    convertUserDataToJson_Revision();
//}
//
//void CUserDataManager::setUserData_Number(std::string key, int value)
//{
//    if(m_UserData._userDataIntMap.find(key) != m_UserData._userDataIntMap.end())
//        m_UserData._userDataIntMap.find(key)->second = value;
//    
//    convertUserDataToJson_Number(key);
//}
//
//void CUserDataManager::setUserData_ItemGet(std::string key, int itemIdx)
//{
//    if(m_UserData._userDataListMap.find(key) != m_UserData._userDataListMap.end()){
//        auto list = m_UserData._userDataListMap.find(key)->second;
//
//        list->push_back(itemIdx);
//        
//        std::sort(list->begin(), list->end(), compare);
//    }
//    convertUserDataToJson_List(key);
//}
//
//bool CUserDataManager::CoinUpdate(int value)
//{
//    bool result = false;
//    if (value > 0)
//    {
//        result = true;
//        setUserData_Number(USERDATA_KEY::COIN, getUserData_Number(USERDATA_KEY::COIN) + value);
//    }
//    else
//    {
//        result = false;
//        if ((value * -1) > getUserData_Number(USERDATA_KEY::COIN) )
//        {
//            Size visibleSize = Director::getInstance()->getVisibleSize();
//            Vec2 origin = Director::getInstance()->getVisibleOrigin();
//            
//            auto popup = CPopup::createWithSpecificFormat(CEarnCoinPopup::create(), POPUPEFFECT_none)->show(Director::getInstance()->getRunningScene(), 102);
//            popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
//                                    origin.x + visibleSize.height * 0.5f));
//            popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//        }
//        else
//        {
//            result = true;
//            setUserData_Number(USERDATA_KEY::COIN, getUserData_Number(USERDATA_KEY::COIN) + value);
//        }
//    }
//    return result;
//}
//
//#pragma mark -
//#pragma mark [ private function ]
//void CUserDataManager::callbackFirstRevision() {
//    
//    if(m_IsFirstRevisionCall){
//        dataLoad();
//        m_IsFirstRevisionCall = false;
//    }
//    
//}
//
//void CUserDataManager::dataLoad()
//{
//    /* saveRevision이 더 높은 데이터로 로딩한다.
//     * 게임중 인터넷이 끊기면 구글클라우드에는 저장하지 않기 때문에 대부분 xml의 revision이 높을 것이다.
//     * 하지만 게임을 지웠다가 다시 설치할 경우엔 클라우드의 데이터를 가져와야한다. */
//    
//    int xmlRevision = 0;
//    std::string crypto_key = MakeCryptoString("USER_DATA_SAVE_REVISION", USERDATA::CRYPTO_KEY);
//    auto valueJson = UserDefault::getInstance()->getStringForKey(crypto_key.c_str(), "");
//    if (valueJson != ""){
//        JSONREADER_CREATE
//        xmlRevision = root["data"].asInt();
//    }
//    
//    int googleRevision = getUserData_Number(USERDATA_KEY::DATA_REVISION);
//    
//    CCLOG("xmlRevision : %u googleRevision : %u", xmlRevision, googleRevision);
//    
//    
//    if (xmlRevision >= googleRevision)
//    {
//        dataLoadFromXML();
//    }
//    else{
//        dataLoadFromCloud();
//        /* 구글 클라우드의 Revision이 높을때(새로 다운 받았을 경우)
//         * 마침 인터넷이 연결되어 있지 않다면 데이터가 날아갈 수 있다.(연결되어 있지 않다면 xml에서 로드 하기때문)
//         * 때문에 게임을 새로 설치한 경우에는 항상 로그인을 하도록 하고 XML에 모두 저장한다. */
//    }
//}
//
//void CUserDataManager::dataLoadFromXML()
//{
//    for(auto keyInfo : m_UserDataKeyList){
//        std::string crypto_key = MakeCryptoString(keyInfo.first.c_str(), USERDATA::CRYPTO_KEY);
//        convertJsonToUserData(crypto_key.c_str(), UserDefault::getInstance()->getStringForKey(crypto_key.c_str(), ""));
//    }
//    
//    CCLOG("Data load finished from xml");
//    //CSDKUtil::Instance()->Toast("Load finished from xml");
//    m_IsDataLoadFinish = true;
//}
//
//void CUserDataManager::dataLoadFromCloud()
//{
//    for(auto keyInfo : m_UserDataKeyList)
//    {
//        std::string crypto_key = MakeCryptoString(keyInfo.first.c_str(), USERDATA::CRYPTO_KEY);
//        
//        // google sdk와 통신후 convertJsonToUserData 호출됨
//        CSDKUtil::Instance()->CloudLoad(crypto_key.c_str());
//    }
//}
//
//void CUserDataManager::convertJsonToUserData(std::string key, std::string valueJson)
//{
//    // 복호화
//    std::string decrypto_key = MakeCryptoString(key, USERDATA::CRYPTO_KEY);
//    std::string decrypto_value = MakeCryptoString(valueJson, USERDATA::CRYPTO_KEY);
//    
//    CCLOG("=======================CloudLoad========================");
//    CCLOG("Decrypto Key : %s", decrypto_key.c_str());
//    CCLOG("Decrypto Value : %s", decrypto_value.c_str());
//    CCLOG("==============================================================");
//    
//    if(m_UserDataKeyList.find(decrypto_key) != m_UserDataKeyList.end()){
//        if(m_UserDataKeyList.find(decrypto_key)->second == USERDATA::SINGLE_DATA_KEY)
//        {
//            convertJsonToUserData_Number(decrypto_key, decrypto_value);
//        }
//        else if(m_UserDataKeyList.find(decrypto_key)->second == USERDATA::ARRAY_DATA_KEY)
//        {
//            convertJsonToUserData_List(decrypto_key, decrypto_value);
//        }
//    }
//}
//
//void CUserDataManager::convertJsonToUserData_Number(std::string key, std::string valueJson)
//{
//    if (valueJson != ""){
//        JSONREADER_CREATE
//        int data = root["data"].asInt();
//        CCLOG("%s : %d", key.c_str(), data);
//        if(m_UserData._userDataIntMap.find(key) != m_UserData._userDataIntMap.end())
//            m_UserData._userDataIntMap.find(key)->second = data;
//    }
//    else{
//        CCLOG("This is the first time to play.");
//        CCLOG("%s : %d", key.c_str(), m_UserData._userDataIntMap.find(key)->second);
//    }
//}
//
//void CUserDataManager::convertJsonToUserData_List(std::string key, std::string valueJson)
//{
//    DATA_LIST* list;
//    
//    if(m_UserData._userDataListMap.find(key) == m_UserData._userDataListMap.end()){
//        CCLOG("There is no list with this key %s", key.c_str());
//        CCASSERT(false, "Wrong Key");
//    }
//    
//    list = m_UserData._userDataListMap.find(key)->second;
//    CCLOG("%s : %s", key.c_str(), valueJson.c_str());
//    
//    if (valueJson != ""){
//        JSONREADER_CREATE
//        const Json::Value dataArray = root["data"];
//        for(auto element : dataArray)
//        {
//            list->push_back(element.asInt());
//        }
//    }
//    else{
//        CCLOG("This is the first time to play.");
//        list->push_back(0);
//    }
//}
//
//void CUserDataManager::convertUserDataToJson_Number(std::string key)
//{
//    JSONWRITER_CREATE
//    root["data"] = getUserData_Number(key);
//    std::string dataStr = writer.write(root);
//    
//    // 암호화
//    std::string crypto_key = MakeCryptoString(key, USERDATA::CRYPTO_KEY);
//    std::string crypto_value = MakeCryptoString(dataStr, USERDATA::CRYPTO_KEY);
//    
//    UserDefault::getInstance()->setStringForKey(crypto_key.c_str(), crypto_value);
//    setSaveRevision(getUserData_Number(USERDATA_KEY::DATA_REVISION) + 1);
//    
//    if (CCloudManager::Instance()->getIsConnected())
//        CCloudManager::Instance()->CloudDataSave(crypto_key.c_str(), crypto_value);
//}
//
//void CUserDataManager::convertUserDataToJson_List(std::string key)
//{
//    JSONWRITER_CREATE
//    Json::Value jsonItemList;
//    auto list = getUserData_List(key);
//    
//    for(auto data : *list)
//    {
//        jsonItemList.append(static_cast<int>(data));
//    }
//    root["data"] = jsonItemList;
//    std::string dataStr = writer.write(root);
//    
//    // 암호화
//    std::string crypto_key = MakeCryptoString(key, USERDATA::CRYPTO_KEY);
//    std::string crypto_value = MakeCryptoString(dataStr, USERDATA::CRYPTO_KEY);
//    
//    UserDefault::getInstance()->setStringForKey(crypto_key.c_str(), crypto_value);
//    setSaveRevision(getUserData_Number(USERDATA_KEY::DATA_REVISION) + 1);
//    
//    if (CCloudManager::Instance()->getIsConnected())
//        CCloudManager::Instance()->CloudDataSave(crypto_key.c_str(), crypto_value);
//}
//
//void CUserDataManager::convertUserDataToJson_Revision()
//{
//    std::string key = USERDATA_KEY::DATA_REVISION;
//    
//    JSONWRITER_CREATE
//    root["data"] = getUserData_Number(key);
//    std::string dataStr = writer.write(root);
//    
//    // 암호화
//    std::string crypto_key = MakeCryptoString(key, USERDATA::CRYPTO_KEY);
//    std::string crypto_value = MakeCryptoString(dataStr, USERDATA::CRYPTO_KEY);
//    
//    UserDefault::getInstance()->setStringForKey(crypto_key.c_str(), crypto_value);
//    
//    if (CCloudManager::Instance()->getIsConnected())
//        CCloudManager::Instance()->CloudDataSave(crypto_key.c_str(), crypto_value);
//}
//
//void CUserDataManager::addKey(std::string keyKind, std::string key)
//{
//    m_UserDataKeyList.emplace(std::pair<std::string, std::string>(key, keyKind));
//    CCLOG("Kind : %s Add Key : %s ", keyKind.c_str(), key.c_str());
//}
//
//void CUserDataManager::overwriteXmlByCloud(std::string key, std::string valueJson)
//{
//    CCLOG("Overwrite Xml by Google cloud data - key : %s value : %s ", key.c_str(), valueJson.c_str());
//    UserDefault::getInstance()->setStringForKey(key.c_str(), valueJson);
//}
