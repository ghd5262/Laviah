#include "UserDataManager.h"
#include "../Scene/GameScene.h"
#include "../MyUI/MyButton.h"
#include "../MyUI/Popup.h"
#include "../MyUI/Popup/EarnCoinPopup.h"
#include "../json/json.h"
#include "../DataManager/WorkshopItemDataManager.h"
#include "../SDKUtil/SDKUtil.h"
#include "../GoogleCloud/GoogleCloudManager.h"
#include "../DataManager/CharacterDataManager.h"
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
unsigned compare (unsigned a, unsigned b)
{
    return ( a < b );
}
//d8d56d34a90de34339f8a174523adaae - texturepacker global key
static const std::string CRYPTO_KEY = "sktjdgmlq1024!";

CUserDataManager::CUserDataManager()
	: m_JsonUserDataFromXML("")
	, m_JsonUserDataFromGoogleCloud("")
// 데이터 한번에 저장 및 로드를 위해 주석처리 - 2016. 9. 3
//: m_IsFirstRevisionCall(false)
//, m_IsDataLoadFinish(false)
{
	m_UserData = std::shared_ptr<sUSER_DATA>(new sUSER_DATA(), [](sUSER_DATA* userData)
	{
		delete userData;
	});

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
    
    
    const Json::Value numberDataArray = root["userDefaultDatas_Number"];
    
    for (unsigned int dataCount = 0; dataCount < numberDataArray.size(); ++dataCount)
    {
        const Json::Value valueItem = numberDataArray[dataCount];
        
        std::string dataKey = valueItem["dataKey"].asString();
        unsigned defaultValue = valueItem["defaultValue"].asUInt();
        addKey("userDefaultDatas_Number", dataKey);
        m_UserData->_userDataUnsignedMap.emplace(std::pair<std::string, unsigned>(dataKey, defaultValue));
    }
    
    const Json::Value listDataArray = root["userDefaultDatas_List"];
    
    for (unsigned int dataCount = 0; dataCount < listDataArray.size(); ++dataCount)
    {
        const Json::Value valueItem = listDataArray[dataCount];
        
        std::string dataKey = valueItem["dataKey"].asString();
		auto emptyList = new std::vector<unsigned>();
        addKey("userDefaultDatas_List", dataKey);

		m_UserData->_userDataListMap.emplace(std::pair<std::string, std::vector<unsigned>*>(dataKey, emptyList));
    }
    
	if (m_UserData->_userDataKeyMap.size() == 0)
    {
        CCLOG("WARNNING : There is no key of game data");
        CCASSERT(false, "No Key");
    }
}

CUserDataManager::~CUserDataManager()
{
	std::for_each(m_UserData->_userDataListMap.begin(), m_UserData->_userDataListMap.end(), [=](std::pair<std::string, std::vector<unsigned>*> list)
	{
		if (list.second != nullptr)
			delete list.second;
	});
}

CUserDataManager* CUserDataManager::Instance()
{
    static CUserDataManager instance;
    return &instance;
}

void CUserDataManager::GoogleLoginResult()
{
    CCLOG("GoogleLoginResult : Called");
    dataLoad();
}

void CUserDataManager::dataLoad()
{
	bool isFirstTimeToLoad = true;

    std::string crypto_key = MakeCryptoString("USER_DATA_FIRSTLOAD", CRYPTO_KEY);
	isFirstTimeToLoad = UserDefault::getInstance()->getBoolForKey(crypto_key.c_str(), true);

	if (isFirstTimeToLoad && CGoogleCloudManager::Instance()->getIsConnected())
	{
		// 첫 로드일 경우 구글 클라우드 로드를 기다린다.
		dataLoadFromGoogleCloud();
		dataLoadFromXML();
		//convertJsonToUserData(m_JsonUserDataFromXML);
	}
	else
	{
		// 첫 로드가 아닐 경우 xml에서 로드
		dataLoadFromXML();
		convertJsonToUserData(m_JsonUserDataFromXML);
	}

}

void CUserDataManager::dataLoadFromXML()
{
	std::string crypto_key = MakeCryptoString("USER_DATA", CRYPTO_KEY);
	std::string crypto_value = UserDefault::getInstance()->getStringForKey(crypto_key.c_str(), "");
	std::string decrypto_value = MakeCryptoString(crypto_value, CRYPTO_KEY);

	m_JsonUserDataFromXML = decrypto_value;
}

void CUserDataManager::dataLoadFromGoogleCloud()
{
	std::string crypto_key = MakeCryptoString("USER_DATA", CRYPTO_KEY);
	       
	// google sdk와 통신후 googleCloudDataLoad 호출됨
	// googleCloudDataLoad 호출 후에는 revision 비교 후 더 높은 것을 채택함
	CSDKUtil::Instance()->GoogleCloudLoad(crypto_key.c_str());
}

void CUserDataManager::googleCloudDataLoad(std::string cryptoValue)
{
	std::string decrypto_value = MakeCryptoString(cryptoValue, CRYPTO_KEY);

	m_JsonUserDataFromGoogleCloud = decrypto_value;

	if (isGoogleRevisionHigher())
	{
		convertJsonToUserData(m_JsonUserDataFromGoogleCloud);
		overwriteXmlByGoogleCloud(m_JsonUserDataFromGoogleCloud);
	}
	else
	{
		convertJsonToUserData(m_JsonUserDataFromXML);
	}
}

void CUserDataManager::convertJsonToUserData(std::string valueJson)
{
	if (valueJson == "")
	{
        // 첫 실행시 0번째 아이템은 가지고 있는 상태
        for(auto list : m_UserData->_userDataListMap)
        {
            list.second->emplace_back(0);
        }
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

	for (auto keyInfo : m_UserData->_userDataKeyMap)
	{
		std::string keyKind = keyInfo.second;
		std::string key = keyInfo.first;

		if (keyKind == "userDefaultDatas_Number")
		{
			unsigned value = dataArray[key.c_str()].asUInt();
			if (m_UserData->_userDataUnsignedMap.find(key) != m_UserData->_userDataUnsignedMap.end())
				m_UserData->_userDataUnsignedMap.find(key)->second = value;
		}
		else if (keyKind == "userDefaultDatas_List")
		{
			const Json::Value arrayValue = dataArray[key.c_str()];
			for (auto value : arrayValue)
			{
				unsigned itemIdx = value.asUInt();

				if (getUserData_IsItemHave(key, itemIdx))
				{
					CCLOG("Item get : Already have %s", key.c_str());
					continue;
				}

				if (m_UserData->_userDataListMap.find(key) != m_UserData->_userDataListMap.end()){
					auto list = m_UserData->_userDataListMap.find(key)->second;

					list->push_back(itemIdx);

					std::sort(list->begin(), list->end(), compare);
				}
			}
		}
	}
}

bool CUserDataManager::isGoogleRevisionHigher()
{
	unsigned googleRevision = 0;
	unsigned xmlRevision = 0;
	std::string key = "USER_DATA_SAVE_REVISION";
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
		googleRevision = dataArray[key.c_str()].asUInt();
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
		xmlRevision = dataArray[key.c_str()].asUInt();
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

void CUserDataManager::overwriteXmlByGoogleCloud(std::string valueJson)
{
    CCLOG("Overwrite Xml by Google cloud data - value : %s ", valueJson.c_str());
    UserDefault::getInstance()->setStringForKey("USER_DATA", valueJson);
}

void CUserDataManager::addKey(std::string keyKind, std::string key)
{
    m_UserData->_userDataKeyMap.emplace(std::pair<std::string, std::string>(key, keyKind));
    CCLOG("Kind : %s Add Key : %s ", keyKind.c_str(), key.c_str());
}

#pragma mark -
#pragma mark [ interface function getter ]
unsigned CUserDataManager::getUserData_Number(std::string key)
{
    if(m_UserData->_userDataUnsignedMap.find(key) != m_UserData->_userDataUnsignedMap.end()){
        auto value = m_UserData->_userDataUnsignedMap.find(key)->second;
        return value;
    }
    CCLOG("There is no user data key : %s", key.c_str());
    CCASSERT(false, "Wrong Key");
}

std::vector<unsigned>* CUserDataManager::getUserData_List(std::string key)
{
    if(m_UserData->_userDataListMap.find(key) != m_UserData->_userDataListMap.end()){
        auto list = m_UserData->_userDataListMap.find(key)->second;
        return list;
    }
    CCLOG("There is no user data key : %s", key.c_str());
    CCASSERT(false, "Wrong Key");
}

bool CUserDataManager::getUserData_IsItemHave(std::string key, unsigned itemIdx)
{
    if(m_UserData->_userDataListMap.find(key) == m_UserData->_userDataListMap.end()){
        CCLOG("There is no list with this key %s", key.c_str());
        CCASSERT(false, "Wrong Key");
    }
    
    auto itemList = m_UserData->_userDataListMap.find(key)->second;
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

void CUserDataManager::convertUserDataToJson()
{
	setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);

	JSONWRITER_CREATE
		
	Json::Value dataArray = root["data"];
	
	for (auto keyInfo : m_UserData->_userDataKeyMap)
	{
		std::string keyKind = keyInfo.second;
		std::string key = keyInfo.first;
		Json::Value userData;
		if (keyKind == "userDefaultDatas_Number")
		{
			dataArray[key.c_str()] = getUserData_Number(key);
			//dataArray[key.c_str()] = userData;
		}
		else if (keyKind == "userDefaultDatas_List")
		{
			Json::Value jsonItemList;
			auto list = getUserData_List(key);

			for (auto data : *list)
			{
				jsonItemList.append(static_cast<unsigned>(data));
			}
			dataArray[key.c_str()] = jsonItemList;
			//dataArray[key.c_str()] = userData;
		}
	}
	std::string dataStr = "";
	root["data"] = dataArray;
	dataStr = writer.write(root);
	// 암호화
	std::string crypto_key = MakeCryptoString("USER_DATA", CRYPTO_KEY);
	std::string crypto_value = MakeCryptoString(dataStr, CRYPTO_KEY);

	UserDefault::getInstance()->setStringForKey(crypto_key.c_str(), crypto_value);

	if (CGoogleCloudManager::Instance()->getIsConnected())
		CGoogleCloudManager::Instance()->GoogleCloudDataSave(crypto_key.c_str(), crypto_value);
}

#pragma mark -
#pragma mark [ interface function setter ]
void CUserDataManager::setSaveRevision(unsigned value)
{
    if(m_UserData->_userDataUnsignedMap.find("USER_DATA_SAVE_REVISION") != m_UserData->_userDataUnsignedMap.end())
        m_UserData->_userDataUnsignedMap.find("USER_DATA_SAVE_REVISION")->second = value;
}

void CUserDataManager::setUserData_Number(std::string key, unsigned value)
{
    if(m_UserData->_userDataUnsignedMap.find(key) != m_UserData->_userDataUnsignedMap.end())
        m_UserData->_userDataUnsignedMap.find(key)->second = value;
    
    convertUserDataToJson();
}

void CUserDataManager::setUserData_ItemGet(std::string key, unsigned itemIdx)
{
    if(getUserData_IsItemHave(key, itemIdx))
    {
        CCLOG("Item get : Already have %s", key.c_str());
        return;
    }
    
    if(m_UserData->_userDataListMap.find(key) != m_UserData->_userDataListMap.end()){
        auto list = m_UserData->_userDataListMap.find(key)->second;

        list->push_back(itemIdx);
        
        std::sort(list->begin(), list->end(), compare);
    }
    convertUserDataToJson();
}

bool CUserDataManager::CoinUpdate(int value)
{
    bool result = false;
    if (value > 0)
    {
        result = true;
        setUserData_Number("USER_COIN", getUserData_Number("USER_COIN") + value);
    }
    else
    {
        result = false;
        if ((value * -1) > getUserData_Number("USER_COIN") )
        {
            Size visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();
            
            auto popup = CPopup::createWithSpecificFormat(CEarnCoinPopup::create(), POPUPEFFECT_none);
            popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
                                    origin.x + visibleSize.height * 0.5f));
            popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            Director::getInstance()->getRunningScene()->addChild(popup, 102);
        }
        else
        {
            result = true;
            setUserData_Number("USER_COIN", getUserData_Number("USER_COIN") + value);
        }
    }
    return result;
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
//        std::string crypto_key = MakeCryptoString("USER_DATA_SAVE_REVISION", CRYPTO_KEY);
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
//unsigned CUserDataManager::getUserData_Number(std::string key)
//{
//    if(m_UserData->_userDataUnsignedMap.find(key) != m_UserData->_userDataUnsignedMap.end()){
//        auto value = m_UserData->_userDataUnsignedMap.find(key)->second;
//        return value;
//    }
//    CCLOG("There is no user data key : %s", key.c_str());
//    CCASSERT(false, "Wrong Key");
//}
//
//std::vector<unsigned>* CUserDataManager::getUserData_List(std::string key)
//{
//    if(m_UserData->_userDataListMap.find(key) != m_UserData->_userDataListMap.end()){
//        auto list = m_UserData->_userDataListMap.find(key)->second;
//        return list;
//    }
//    CCLOG("There is no user data key : %s", key.c_str());
//    CCASSERT(false, "Wrong Key");
//}
//
//bool CUserDataManager::getUserData_IsItemHave(std::string key, unsigned itemIdx)
//{
//    if(m_UserData->_userDataListMap.find(key) == m_UserData->_userDataListMap.end()){
//        CCLOG("There is no list with this key %s", key.c_str());
//        CCASSERT(false, "Wrong Key");
//    }
//    
//    auto itemList = m_UserData->_userDataListMap.find(key)->second;
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
//void CUserDataManager::setSaveRevision(unsigned value)
//{
//    if(m_UserData->_userDataUnsignedMap.find("USER_DATA_SAVE_REVISION") != m_UserData->_userDataUnsignedMap.end())
//        m_UserData->_userDataUnsignedMap.find("USER_DATA_SAVE_REVISION")->second = value;
//    
//    convertUserDataToJson_Revision();
//}
//
//void CUserDataManager::setUserData_Number(std::string key, unsigned value)
//{
//    if(m_UserData->_userDataUnsignedMap.find(key) != m_UserData->_userDataUnsignedMap.end())
//        m_UserData->_userDataUnsignedMap.find(key)->second = value;
//    
//    convertUserDataToJson_Number(key);
//}
//
//void CUserDataManager::setUserData_ItemGet(std::string key, unsigned itemIdx)
//{
//    if(m_UserData->_userDataListMap.find(key) != m_UserData->_userDataListMap.end()){
//        auto list = m_UserData->_userDataListMap.find(key)->second;
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
//        setUserData_Number("USER_COIN", getUserData_Number("USER_COIN") + value);
//    }
//    else
//    {
//        result = false;
//        if ((value * -1) > getUserData_Number("USER_COIN") )
//        {
//            Size visibleSize = Director::getInstance()->getVisibleSize();
//            Vec2 origin = Director::getInstance()->getVisibleOrigin();
//            
//            auto popup = CPopup::createWithSpecificFormat(CEarnCoinPopup::create(), POPUPEFFECT_none);
//            popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
//                                    origin.x + visibleSize.height * 0.5f));
//            popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//            Director::getInstance()->getRunningScene()->addChild(popup, 102);
//        }
//        else
//        {
//            result = true;
//            setUserData_Number("USER_COIN", getUserData_Number("USER_COIN") + value);
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
//    unsigned xmlRevision = 0;
//    std::string crypto_key = MakeCryptoString("USER_DATA_SAVE_REVISION", CRYPTO_KEY);
//    auto valueJson = UserDefault::getInstance()->getStringForKey(crypto_key.c_str(), "");
//    if (valueJson != ""){
//        JSONREADER_CREATE
//        xmlRevision = root["data"].asInt();
//    }
//    
//    unsigned googleRevision = getUserData_Number("USER_DATA_SAVE_REVISION");
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
//    for(auto keyInfo : m_UserData->_userDataKeyMap){
//        std::string crypto_key = MakeCryptoString(keyInfo.first.c_str(), CRYPTO_KEY);
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
//    for(auto keyInfo : m_UserData->_userDataKeyMap)
//    {
//        std::string crypto_key = MakeCryptoString(keyInfo.first.c_str(), CRYPTO_KEY);
//        
//        // google sdk와 통신후 convertJsonToUserData 호출됨
//        CSDKUtil::Instance()->GoogleCloudLoad(crypto_key.c_str());
//    }
//}
//
//void CUserDataManager::convertJsonToUserData(std::string key, std::string valueJson)
//{
//    // 복호화
//    std::string decrypto_key = MakeCryptoString(key, CRYPTO_KEY);
//    std::string decrypto_value = MakeCryptoString(valueJson, CRYPTO_KEY);
//    
//    CCLOG("=======================GoogleCloudLoad========================");
//    CCLOG("Decrypto Key : %s", decrypto_key.c_str());
//    CCLOG("Decrypto Value : %s", decrypto_value.c_str());
//    CCLOG("==============================================================");
//    
//    if(m_UserData->_userDataKeyMap.find(decrypto_key) != m_UserData->_userDataKeyMap.end()){
//        if(m_UserData->_userDataKeyMap.find(decrypto_key)->second == "userDefaultDatas_Number")
//        {
//            convertJsonToUserData_Number(decrypto_key, decrypto_value);
//        }
//        else if(m_UserData->_userDataKeyMap.find(decrypto_key)->second == "userDefaultDatas_List")
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
//        unsigned data = root["data"].asInt();
//        CCLOG("%s : %d", key.c_str(), data);
//        if(m_UserData->_userDataUnsignedMap.find(key) != m_UserData->_userDataUnsignedMap.end())
//            m_UserData->_userDataUnsignedMap.find(key)->second = data;
//    }
//    else{
//        CCLOG("This is the first time to play.");
//        CCLOG("%s : %d", key.c_str(), m_UserData->_userDataUnsignedMap.find(key)->second);
//    }
//}
//
//void CUserDataManager::convertJsonToUserData_List(std::string key, std::string valueJson)
//{
//    std::vector<unsigned>* list;
//    
//    if(m_UserData->_userDataListMap.find(key) == m_UserData->_userDataListMap.end()){
//        CCLOG("There is no list with this key %s", key.c_str());
//        CCASSERT(false, "Wrong Key");
//    }
//    
//    list = m_UserData->_userDataListMap.find(key)->second;
//    CCLOG("%s : %s", key.c_str(), valueJson.c_str());
//    
//    if (valueJson != ""){
//        JSONREADER_CREATE
//        const Json::Value dataArray = root["data"];
//        for(auto element : dataArray)
//        {
//            list->push_back(element.asUInt());
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
//    std::string crypto_key = MakeCryptoString(key, CRYPTO_KEY);
//    std::string crypto_value = MakeCryptoString(dataStr, CRYPTO_KEY);
//    
//    UserDefault::getInstance()->setStringForKey(crypto_key.c_str(), crypto_value);
//    setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
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
//        jsonItemList.append(static_cast<unsigned>(data));
//    }
//    root["data"] = jsonItemList;
//    std::string dataStr = writer.write(root);
//    
//    // 암호화
//    std::string crypto_key = MakeCryptoString(key, CRYPTO_KEY);
//    std::string crypto_value = MakeCryptoString(dataStr, CRYPTO_KEY);
//    
//    UserDefault::getInstance()->setStringForKey(crypto_key.c_str(), crypto_value);
//    setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//    
//    if (CGoogleCloudManager::Instance()->getIsConnected())
//        CGoogleCloudManager::Instance()->GoogleCloudDataSave(crypto_key.c_str(), crypto_value);
//}
//
//void CUserDataManager::convertUserDataToJson_Revision()
//{
//    std::string key = "USER_DATA_SAVE_REVISION";
//    
//    JSONWRITER_CREATE
//    root["data"] = getUserData_Number(key);
//    std::string dataStr = writer.write(root);
//    
//    // 암호화
//    std::string crypto_key = MakeCryptoString(key, CRYPTO_KEY);
//    std::string crypto_value = MakeCryptoString(dataStr, CRYPTO_KEY);
//    
//    UserDefault::getInstance()->setStringForKey(crypto_key.c_str(), crypto_value);
//    
//    if (CGoogleCloudManager::Instance()->getIsConnected())
//        CGoogleCloudManager::Instance()->GoogleCloudDataSave(crypto_key.c_str(), crypto_value);
//}
//
//void CUserDataManager::addKey(std::string keyKind, std::string key)
//{
//    m_UserData->_userDataKeyMap.emplace(std::pair<std::string, std::string>(key, keyKind));
//    CCLOG("Kind : %s Add Key : %s ", keyKind.c_str(), key.c_str());
//}
//
//void CUserDataManager::overwriteXmlByGoogleCloud(std::string key, std::string valueJson)
//{
//    CCLOG("Overwrite Xml by Google cloud data - key : %s value : %s ", key.c_str(), valueJson.c_str());
//    UserDefault::getInstance()->setStringForKey(key.c_str(), valueJson);
//}