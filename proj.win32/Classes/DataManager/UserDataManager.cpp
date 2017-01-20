#include "UserDataManager.h"
#include "../Scene/GameScene.h"
#include "../MyUI/MyButton.h"
#include "../MyUI/Popup.h"
#include "../MyUI/Popup/EarnCoinPopup.h"
#include "../json/json.h"
#include "../DataManager/WorkshopItemDataManager.h"
#include "../SDKUtil/SDKUtil.h"
#include "../GoogleCloud/GoogleCloudManager.h"
#include "../Common/NoticeDefine.h"
#include <algorithm>     /* qsort */

#define JSONREADER_CREATE \
	Json::Value root; \
	Json::Reader reader; \
	bool parsingSuccessful = reader.parse(valueJson, root); \
	if (!parsingSuccessful) \
	{ \
		CCLOG("Error : Failed to parse configuration\n %s", reader.getFormatedErrorMessages().c_str()); \
		return; \
	}

#define JSONWRITER_CREATE \
	Json::Value root; \
	Json::Value data; \
	Json::StyledWriter writer;

// 오름차순 compare
int compare (int a, int b)
{
    return ( a < b );
}

namespace USERDATA{
	//d8d56d34a90de34339f8a174523adaae - texturepacker global key
	static const std::string CRYPTO_KEY			= "sktjdgmlq1024!";
	static const std::string SINGLE_DATA_KEY	= "userDefaultDatas_Number";
	static const std::string ARRAY_DATA_KEY		= "userDefaultDatas_List";
	static const std::string GOOGLE_DATA_KEY	= "USER_DATA";
};

CUserDataManager::CUserDataManager()
: m_JsonUserDataFromXML("")
, m_JsonUserDataFromGoogleCloud("")
// 데이터 한번에 저장 및 로드를 위해 주석처리 - 2016. 9. 3
//: m_IsFirstRevisionCall(false)
//, m_IsDataLoadFinish(false)
{}

CUserDataManager::~CUserDataManager()
{
	auto deleteList = [=](std::map<std::string, ARRAY_DATA*> list){
		for (auto dataList : list)
		{
			if (dataList.second) delete dataList.second;
		}
	};

	deleteList(m_UserData._userDataListMap);
	deleteList(m_UserDefaultData._userDataListMap);
}

CUserDataManager* CUserDataManager::Instance()
{
    static CUserDataManager instance;
    return &instance;
}

void CUserDataManager::initUserDefaultValue(sUSER_DATA &data)
{
    Json::Value root;
    Json::Reader reader;
    
    // userDefaultDataList.json 읽음
    std::string strUserDefaultDataList = FileUtils::getInstance()->fullPathForFilename("userDefaultDataList.json");
    std::string userDefaultDataListClearData = FileUtils::getInstance()->getStringFromFile(strUserDefaultDataList);
    size_t pos = userDefaultDataListClearData.rfind("}");
    userDefaultDataListClearData = userDefaultDataListClearData.substr(0, pos + 1);
    
    bool parsingSuccessful = reader.parse(userDefaultDataListClearData, root);
    if (!parsingSuccessful)
    {
        CCASSERT(false, MakeString("parser failed : \n %s", userDefaultDataListClearData.c_str()).c_str());
        return;
    }
    CCLOG("strUserDefaultDataList JSON : \n %s\n", userDefaultDataListClearData.c_str());
    
    
    const Json::Value numberDataArray = root[USERDATA::SINGLE_DATA_KEY];
    
    for (int dataCount = 0; dataCount < numberDataArray.size(); ++dataCount)
    {
        const Json::Value valueItem = numberDataArray[dataCount];
        
        std::string dataKey = valueItem["dataKey"].asString();
        int defaultValue = valueItem["defaultValue"].asInt();

		data._userDataIntMap.emplace(std::pair<std::string, int>(dataKey, defaultValue));
    }
    
    const Json::Value listDataArray = root[USERDATA::ARRAY_DATA_KEY];
    
    for (int dataCount = 0; dataCount < listDataArray.size(); ++dataCount)
    {
        const Json::Value valueItem = listDataArray[dataCount];
        
        std::string dataKey = valueItem["dataKey"].asString();
        const Json::Value defaultValueArray = valueItem["defaultValue"];
       
		auto emptyList = new ARRAY_DATA();        
		data._userDataListMap.emplace(std::pair<std::string, ARRAY_DATA*>(dataKey, emptyList));

        for(int valueCount = 0; valueCount < defaultValueArray.size(); valueCount++)
            emptyList->emplace_back(defaultValueArray[valueCount].asInt());
    }
}

void CUserDataManager::initUserDataKey(sUSER_DATA &data)
{
	for (auto singleData : data._userDataIntMap)
		this->addKey(USERDATA::SINGLE_DATA_KEY, singleData.first);
	

	for (auto arrayData : data._userDataListMap)
		this->addKey(USERDATA::ARRAY_DATA_KEY, arrayData.first);


	if (!m_UserDataKeyList.size())
	{
		CCLOG("WARNNING : There is no key of game data");
		CCASSERT(false, "No Key");
	}
}

void CUserDataManager::initSingleUserDataWithDefaultValue(std::string key)
{
	m_UserData._userDataIntMap[key] = m_UserDefaultData._userDataIntMap[key];
}

void CUserDataManager::initArrayUserDataWithDefaultValue(std::string key)
{
	auto userArrayData = m_UserData._userDataListMap[key];
	auto userDefaultArrayData = m_UserDefaultData._userDataListMap[key];
	if (userDefaultArrayData->size() > 0){
		userArrayData->resize(userDefaultArrayData->size());
		std::copy(std::begin(*userDefaultArrayData), std::end(*userDefaultArrayData), std::begin(*userArrayData));
	}
}

bool CUserDataManager::getIsFirstPlay()
{
    std::string crypto_key = MakeCryptoString(USERDATA_KEY::FIRST_LOAD, USERDATA::CRYPTO_KEY);
    return UserDefault::getInstance()->getBoolForKey(crypto_key.c_str(), true);
}

void CUserDataManager::setLastSavedTime(long long unixTime)
{
	UserDefault::getInstance()->setDoubleForKey(USERDATA_KEY::LAST_SAVED_TIME.c_str(), unixTime);
}

tm* CUserDataManager::getLastSavedTime()
{
	std::string crypto_key = MakeCryptoString(USERDATA_KEY::LAST_SAVED_TIME, USERDATA::CRYPTO_KEY);
	time_t timer = UserDefault::getInstance()->getDoubleForKey(crypto_key.c_str(), true);
	return localtime(&timer);
}

void CUserDataManager::UserDataLoad()
{
    this->initUserDefaultValue(m_UserDefaultData);
    this->initUserDataKey(m_UserDefaultData);
    
	if (getIsFirstPlay() &&
        CGoogleCloudManager::Instance()->getIsConnected() &&
        CSDKUtil::Instance()->getIsNetworkConnect())
	{
        CSDKUtil::Instance()->Toast("This is first play");
        
		// 첫 로드일 경우 구글 클라우드 로드를 기다린다.
		dataLoadFromGoogleCloud();
		dataLoadFromXML();
	}
	else
	{
        CSDKUtil::Instance()->Toast("This is not first play");
        
		// 첫 로드가 아닐 경우 xml에서 로드
		dataLoadFromXML();
		convertJsonToUserData(m_UserData, m_JsonUserDataFromXML);
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

void CUserDataManager::dataLoadFromGoogleCloud()
{
	std::string crypto_key = MakeCryptoString(USERDATA::GOOGLE_DATA_KEY, USERDATA::CRYPTO_KEY);
	       
	// google sdk와 통신후 googleCloudDataLoad 호출됨
	// googleCloudDataLoad 호출 후에는 revision 비교 후 더 높은 것을 채택함
	CSDKUtil::Instance()->GoogleCloudLoad(crypto_key.c_str());
}

void CUserDataManager::googleCloudDataLoad(std::string cryptoValue)
{
	std::string decrypto_value = MakeCryptoString(cryptoValue, USERDATA::CRYPTO_KEY);

	m_JsonUserDataFromGoogleCloud = decrypto_value;

	if (isGoogleRevisionHigher())
	{
        CSDKUtil::Instance()->Toast("Google revision");
        
		convertJsonToUserData(m_UserData, m_JsonUserDataFromGoogleCloud);
		overwriteXmlByGoogleCloud(m_JsonUserDataFromGoogleCloud);
        UserDefault::getInstance()->setBoolForKey(USERDATA_KEY::FIRST_LOAD.c_str(), false);
	}
	else
	{
        CSDKUtil::Instance()->Toast("xml revision");
        
		convertJsonToUserData(m_UserData, m_JsonUserDataFromXML);
	}
    
    __NotificationCenter::getInstance()->postNotification(NOTICE::USERDATA_LOAD_FINISH, NULL);
}

void CUserDataManager::convertJsonToUserData(sUSER_DATA &data, std::string valueJson)
{
	if (valueJson == "")
	{
        this->initUserDefaultValue(m_UserData);
		CCLOG("This is the first time to load. use default value");
		return;
	}

	Json::Value root;
	Json::Reader reader;
	
	bool parsingSuccessful = reader.parse(valueJson, root);
	if (!parsingSuccessful)
	{
		CCASSERT(false, MakeString("parser failed : \n %s", valueJson.c_str()).c_str());
		return;
	}
	CCLOG("UserData JSON : \n %s\n", valueJson.c_str());

	const Json::Value dataArray = root["data"];

	for (auto keyInfo : m_UserDataKeyList)
	{
		std::string keyKind = keyInfo.second;
		std::string key = keyInfo.first;

		if (keyKind == USERDATA::SINGLE_DATA_KEY)
		{
			auto value = dataArray[key.c_str()];
			auto iter = data._userDataIntMap.find(key);
			if (iter == data._userDataIntMap.end())
				data._userDataIntMap.emplace(std::pair<std::string, int>(key, 0));

			/** if there isn't data about the key.
			* set user data from the default data. */
			if (value.isNull())
			{
				this->initSingleUserDataWithDefaultValue(key);
				continue;
			}

			data._userDataIntMap[key] = value.asInt();
		}
		else if (keyKind == USERDATA::ARRAY_DATA_KEY)
		{
			const Json::Value arrayValue = dataArray[key.c_str()];

			auto iter = data._userDataListMap.find(key);
			if (iter == data._userDataListMap.end()){
				auto emptyList = new ARRAY_DATA();
				data._userDataListMap.emplace(std::pair<std::string, ARRAY_DATA*>(key, emptyList));
			}
			/** if there isn't data about the key.
			 * set user data from the default data. */
			if (arrayValue.isNull())
			{
				this->initArrayUserDataWithDefaultValue(key);
				continue;
			}

			for (auto value : arrayValue)
			{
				int itemIdx = value.asInt();

				if (getUserData_IsItemHave(key, itemIdx))
				{
					CCLOG("Item get : Already have %s", key.c_str());
					continue;
				}

				data._userDataListMap[key]->push_back(itemIdx);
				this->sortUserDataList(key, compare);
			}
		}
	}
}

bool CUserDataManager::isGoogleRevisionHigher()
{
	int googleRevision = 0;
	int xmlRevision = 0;
	std::string key = USERDATA_KEY::DATA_REVISION;
	if (m_JsonUserDataFromGoogleCloud != "")
	{
		Json::Value googleJsonRoot;
		Json::Reader googleJsonReader;

		bool parsingSuccessful = googleJsonReader.parse(m_JsonUserDataFromGoogleCloud, googleJsonRoot);
		if (!parsingSuccessful)
		{
			CCASSERT(false, MakeString("parser failed : \n %s", m_JsonUserDataFromGoogleCloud.c_str()).c_str());
			return false;
		}
		CCLOG("UserData JSON : \n %s\n", m_JsonUserDataFromGoogleCloud.c_str());

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
	if (googleRevision > xmlRevision)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CUserDataManager::SaveUserData(bool saveToCloud/* = false*/, bool forceSave/* = false*/)
{
	setSaveRevision(getUserData_Number(USERDATA_KEY::DATA_REVISION) + 1);
	std::string jsonString = "";
	convertUserDataToJson(jsonString);

	// crypto
	std::string crypto_key = MakeCryptoString(USERDATA::GOOGLE_DATA_KEY, USERDATA::CRYPTO_KEY);
	std::string crypto_value = MakeCryptoString(jsonString, USERDATA::CRYPTO_KEY);

	UserDefault::getInstance()->setStringForKey(crypto_key.c_str(), crypto_value);
	CGoogleCloudManager::Instance()->AddDataToAutoSaveList(crypto_key.c_str(), crypto_value);

	if (saveToCloud) {
		CSDKUtil::Instance()->setNetworkConnectSavedFunc([=](){
			this->saveUserDataToGoogleCloud(crypto_key, crypto_value, forceSave);
		});
		CSDKUtil::Instance()->IsNetworkConnect();
	}
}

void CUserDataManager::overwriteXmlByGoogleCloud(std::string valueJson)
{
    CSDKUtil::Instance()->Toast("Overwrite to xml");
    UserDefault::getInstance()->setStringForKey(USERDATA::GOOGLE_DATA_KEY.c_str(), valueJson);
}

void CUserDataManager::addKey(std::string keyKind, std::string key)
{
    m_UserDataKeyList.emplace(std::pair<std::string, std::string>(key, keyKind));
    CCLOG("Kind : %s Add Key : %s ", keyKind.c_str(), key.c_str());
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

float CUserDataManager::getItemCurrentValue(std::string key)
{
    sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByKey(key.c_str());
    float limitTime = item._valuePerLevel * getUserData_Number(key.c_str());
    return limitTime;
}


#pragma mark -
#pragma mark [ interface function setter ]
void CUserDataManager::setSaveRevision(int value)
{
    if(m_UserData._userDataIntMap.find(USERDATA_KEY::DATA_REVISION) != m_UserData._userDataIntMap.end())
        m_UserData._userDataIntMap.find(USERDATA_KEY::DATA_REVISION)->second = value;
}

void CUserDataManager::setUserData_Number(std::string key, int value)
{
    if(m_UserData._userDataIntMap.find(key) != m_UserData._userDataIntMap.end())
        m_UserData._userDataIntMap.find(key)->second = value;
    
    SaveUserData();
}

void CUserDataManager::setUserData_ItemGet(std::string key, int itemIdx)
{
    if(getUserData_IsItemHave(key, itemIdx))
    {
        CCLOG("Item get : Already have %s index %d", key.c_str(), itemIdx);
        return;
    }
    
	auto itemList = m_UserData._userDataListMap.find(key);
	if (itemList != m_UserData._userDataListMap.end()){
		auto list = itemList->second;

        list->push_back(itemIdx);
        
        this->sortUserDataList(key, compare);
    }
	SaveUserData();
}

void CUserDataManager::setUserData_ItemRemove(std::string key, int itemIdx)
{
	if (!getUserData_IsItemHave(key, itemIdx))
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
	SaveUserData();
}

void CUserDataManager::setUserData_ItemRemoveAll(std::string key)
{
    auto itemList = m_UserData._userDataListMap.find(key);
    if (itemList != m_UserData._userDataListMap.end()){
        auto list = itemList->second;
        list->clear();
    }
	SaveUserData();
}

bool CUserDataManager::CoinUpdate(int value)
{
    bool result = false;
    if (value > 0)
    {
        result = true;
        setUserData_Number(USERDATA_KEY::COIN, getUserData_Number(USERDATA_KEY::COIN) + value);
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
            setUserData_Number(USERDATA_KEY::COIN, getUserData_Number(USERDATA_KEY::COIN) + value);
        }
    }
    return result;
}

int CUserDataManager::getUserDataSequenceFromList(std::string key, int itemIndex)
{
    auto curList = CUserDataManager::Instance()->getUserData_List(key);
    auto iter = std::find(curList.begin(), curList.end(), itemIndex);
    auto sequence = std::distance(curList.begin(), iter);
    return int(sequence);
}

void CUserDataManager::convertUserDataToJson(std::string &valueJson)
{
	JSONWRITER_CREATE

	Json::Value dataArray = root["data"];

	for (auto keyInfo : m_UserDataKeyList)
	{
		std::string keyKind = keyInfo.second;
		std::string key = keyInfo.first;
		Json::Value userData;
		if (keyKind == USERDATA::SINGLE_DATA_KEY)
		{
			dataArray[key.c_str()] = getUserData_Number(key);
		}
		else if (keyKind == USERDATA::ARRAY_DATA_KEY)
		{
			Json::Value jsonItemList;
			auto list = getUserData_List(key);

			for (auto data : list)
			{
				jsonItemList.append(static_cast<int>(data));
			}
			dataArray[key.c_str()] = jsonItemList;
		}
	}
	root["data"] = dataArray;
	valueJson = writer.write(root);
}

void CUserDataManager::saveUserDataToGoogleCloud(std::string key, std::string data, bool forceSave/*= false*/)
{
	if (CSDKUtil::Instance()->getIsNetworkConnect())
		CGoogleCloudManager::Instance()->GoogleCloudDataSave(key.c_str(), data);
	else
	{
		if (!forceSave) return;

		Size visibleSize = Director::getInstance()->getVisibleSize();
		auto gameScene = CGameScene::getGameScene();

		CPopup::create()
			->setPositiveButton([=](Node* sender){
			CSDKUtil::Instance()->setNetworkConnectSavedFunc([=](){
				this->saveUserDataToGoogleCloud(key, data, forceSave);
			});
			CSDKUtil::Instance()->IsNetworkConnect();
		}, TRANSLATE("BUTTON_OK"))
			->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
			->setMessage(TRANSLATE("USERDATA_NETWORK_CONNECT_CHECK"))
			->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->setPopupPosition(visibleSize / 2)
			->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
			->show(gameScene, ZORDER::POPUP);
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

void CUserDataManager::sortUserDataList(std::string key, const LIST_COMPARE& compare)
{
//    if(key == USERDATA_KEY::CHALLENGE_CUR_VALUE_LIST ||
//       key == USERDATA_KEY::CHALLENGE_CUR_LIST) return;
    
    auto list = CUserDataManager::Instance()->getUserData_ListRef(key);
    std::sort(list->begin(), list->end(), compare);
}


// 데이터 한번에 저장 및 로드를 위해 주석처리 - 2016. 9. 3
///* google login을 시도한 후에 호출됨 */
//void CUserDataManager::GoogleLoginResult()
//{
//    CCLOG("GoogleLoginResult : Called");
//    if (CGoogleCloudManager::Instance()->getIsConnected())
//    {
//        CCLOG("GoogleLoginResult : Call google revision function");
//        
//        // 리비전 비교 위해 함수 호출
//        std::string crypto_key = MakeCryptoString(USERDATA_KEY::DATA_REVISION, USERDATA::CRYPTO_KEY);
//        CSDKUtil::Instance()->GoogleCloudLoad(crypto_key.c_str());
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
//    sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByKey(key.c_str());
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
//        dataLoadFromGoogleCloud();
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
//void CUserDataManager::dataLoadFromGoogleCloud()
//{
//    for(auto keyInfo : m_UserDataKeyList)
//    {
//        std::string crypto_key = MakeCryptoString(keyInfo.first.c_str(), USERDATA::CRYPTO_KEY);
//        
//        // google sdk와 통신후 convertJsonToUserData 호출됨
//        CSDKUtil::Instance()->GoogleCloudLoad(crypto_key.c_str());
//    }
//}
//
//void CUserDataManager::convertJsonToUserData(std::string key, std::string valueJson)
//{
//    // 복호화
//    std::string decrypto_key = MakeCryptoString(key, USERDATA::CRYPTO_KEY);
//    std::string decrypto_value = MakeCryptoString(valueJson, USERDATA::CRYPTO_KEY);
//    
//    CCLOG("=======================GoogleCloudLoad========================");
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
//    if (CGoogleCloudManager::Instance()->getIsConnected())
//        CGoogleCloudManager::Instance()->GoogleCloudDataSave(crypto_key.c_str(), crypto_value);
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
//    if (CGoogleCloudManager::Instance()->getIsConnected())
//        CGoogleCloudManager::Instance()->GoogleCloudDataSave(crypto_key.c_str(), crypto_value);
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
//    if (CGoogleCloudManager::Instance()->getIsConnected())
//        CGoogleCloudManager::Instance()->GoogleCloudDataSave(crypto_key.c_str(), crypto_value);
//}
//
//void CUserDataManager::addKey(std::string keyKind, std::string key)
//{
//    m_UserDataKeyList.emplace(std::pair<std::string, std::string>(key, keyKind));
//    CCLOG("Kind : %s Add Key : %s ", keyKind.c_str(), key.c_str());
//}
//
//void CUserDataManager::overwriteXmlByGoogleCloud(std::string key, std::string valueJson)
//{
//    CCLOG("Overwrite Xml by Google cloud data - key : %s value : %s ", key.c_str(), valueJson.c_str());
//    UserDefault::getInstance()->setStringForKey(key.c_str(), valueJson);
//}