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

static const std::string CRYPTO_KEY = "sktjdgmlq1024!";

CUserDataManager::CUserDataManager() : m_IsFirstRevisionCall(false)
{
    m_UserData = std::shared_ptr<sUSER_DATA>(new sUSER_DATA(), [](sUSER_DATA* userData)
                                             {
                                                 delete userData;
                                             });
    
    Json::Value root;
    Json::Reader reader;
    
    // userDefaultDataList.json 읽음
    std::string strUserDefaultDataList = FileUtils::getInstance()->fullPathForFilename("jsonRes/userDefaultDataList.json");
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
        std::vector<unsigned>* defaultValue = new std::vector<unsigned>();
        addKey("userDefaultDatas_List", dataKey);
        m_UserData->_userDataListMap.emplace(std::pair<std::string, std::vector<unsigned>*>(dataKey, defaultValue));
    }
}

CUserDataManager::~CUserDataManager()
{
    std::for_each(m_UserData->_userDataListMap.begin(), m_UserData->_userDataListMap.end(), [=](std::pair<std::string, std::vector<unsigned>*> list)
                  {
                      if(list.second != nullptr)
                          delete list.second;
                  });
}

CUserDataManager* CUserDataManager::Instance()
{
    static CUserDataManager instance;
    return &instance;
}

/* google login을 시도한 후에 호출됨 */
void CUserDataManager::GoogleLoginResult()
{
    CCLOG("GoogleLoginResult : Called");
    if (CGoogleCloudManager::Instance()->getIsConnected())
    {
        CCLOG("GoogleLoginResult : Call google revision function");
        
        // 리비전 비교 위해 함수 호출
        std::string crypto_key = MakeCryptoString("USER_DATA_SAVE_REVISION", CRYPTO_KEY);
        CSDKUtil::Instance()->GoogleCloudLoad(crypto_key.c_str());
        
        m_IsFirstRevisionCall = true;
    }
    else{
        CCLOG("GoogleLoginResult : Call xml revision function");
        
        // XML로부터 데이터 로드
        dataLoadFromXML();
    }
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

float CUserDataManager::getItemLimitTime(std::string key)
{
    sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByKey(key.c_str());
    float limitTime = item._valuePerLevel * getUserData_Number(key.c_str());
    return limitTime;
}

#pragma mark -
#pragma mark [ interface function setter ]
void CUserDataManager::setSaveRevision(unsigned value)
{
    if(m_UserData->_userDataUnsignedMap.find("USER_DATA_SAVE_REVISION") != m_UserData->_userDataUnsignedMap.end())
    m_UserData->_userDataUnsignedMap.find("USER_DATA_SAVE_REVISION")->second = value;
    
    convertUserDataToJson_Revision();
}

void CUserDataManager::setUserData_Number(std::string key, unsigned value)
{
    if(m_UserData->_userDataUnsignedMap.find(key) != m_UserData->_userDataUnsignedMap.end())
        m_UserData->_userDataUnsignedMap.find(key)->second = value;
    
    convertUserDataToJson_Number(key);
}

void CUserDataManager::setUserData_ItemGet(std::string key, unsigned itemIdx)
{
    if(m_UserData->_userDataListMap.find(key) != m_UserData->_userDataListMap.end()){
        auto list = m_UserData->_userDataListMap.find(key)->second;

        list->push_back(itemIdx);
        
        std::sort(list->begin(), list->end(), compare);
    }
    convertUserDataToJson_List(key);
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

#pragma mark -
#pragma mark [ private function ]
void CUserDataManager::callbackFirstRevision() {
    
    if(m_IsFirstRevisionCall){
        dataLoad();
        m_IsFirstRevisionCall = false;
    }
    
}

void CUserDataManager::dataLoad()
{
    /* saveRevision이 더 높은 데이터로 로딩한다.
     * 게임중 인터넷이 끊기면 구글클라우드에는 저장하지 않기 때문에 대부분 xml의 revision이 높을 것이다.
     * 하지만 게임을 지웠다가 다시 설치할 경우엔 클라우드의 데이터를 가져와야한다. */
    
    unsigned xmlRevision = 0;
    std::string crypto_key = MakeCryptoString("USER_DATA_SAVE_REVISION", CRYPTO_KEY);
    auto valueJson = UserDefault::getInstance()->getStringForKey(crypto_key.c_str(), "");
    if (valueJson != ""){
        JSONREADER_CREATE
        xmlRevision = root["data"].asInt();
    }
    
    unsigned googleRevision = getUserData_Number("USER_DATA_SAVE_REVISION");
    
    CCLOG("xmlRevision : %u googleRevision : %u", xmlRevision, googleRevision);
    
    
    if (xmlRevision >= googleRevision)
    {
        dataLoadFromXML();
    }
    else{
        dataLoadFromGoogleCloud();
        /* 구글 클라우드의 Revision이 높을때(새로 다운 받았을 경우)
         * 마침 인터넷이 연결되어 있지 않다면 데이터가 날아갈 수 있다.(연결되어 있지 않다면 xml에서 로드 하기때문)
         * 때문에 게임을 새로 설치한 경우에는 항상 로그인을 하도록 하고 XML에 모두 저장한다. */
    }
}

void CUserDataManager::dataLoadFromXML()
{
    for(auto keyInfo : m_UserData->_userDataKeyMap){
        std::string crypto_key = MakeCryptoString(keyInfo.first.c_str(), CRYPTO_KEY);
        convertJsonToUserData(crypto_key.c_str(), UserDefault::getInstance()->getStringForKey(crypto_key.c_str(), ""));
    }
}

void CUserDataManager::dataLoadFromGoogleCloud()
{
    for(auto keyInfo : m_UserData->_userDataKeyMap)
    {
        std::string crypto_key = MakeCryptoString(keyInfo.first.c_str(), CRYPTO_KEY);
        
        // google sdk와 통신후 convertJsonToUserData 호출됨
        CSDKUtil::Instance()->GoogleCloudLoad(crypto_key.c_str());
    }
}

void CUserDataManager::convertJsonToUserData(std::string key, std::string valueJson)
{
    // 복호화
    std::string decrypto_key = MakeCryptoString(key, CRYPTO_KEY);
    std::string decrypto_value = MakeCryptoString(valueJson, CRYPTO_KEY);
    
    CCLOG("=======================GoogleCloudLoad========================");
    CCLOG("Decrypto Key : %s", decrypto_key.c_str());
    CCLOG("Decrypto Value : %s", decrypto_value.c_str());
    CCLOG("==============================================================");
    
    if(m_UserData->_userDataKeyMap.find(decrypto_key) != m_UserData->_userDataKeyMap.end()){
        if(m_UserData->_userDataKeyMap.find(decrypto_key)->second == "userDefaultDatas_Number")
        {
            convertJsonToUserData_Number(decrypto_key, decrypto_value);
        }
        else if(m_UserData->_userDataKeyMap.find(decrypto_key)->second == "userDefaultDatas_List")
        {
            convertJsonToUserData_List(decrypto_key, decrypto_value);
        }
    }
}

void CUserDataManager::convertJsonToUserData_Number(std::string key, std::string valueJson)
{
    if (valueJson != ""){
        JSONREADER_CREATE
        unsigned data = root["data"].asInt();
        CCLOG("%s : %d", key.c_str(), data);
        if(m_UserData->_userDataUnsignedMap.find(key) != m_UserData->_userDataUnsignedMap.end())
            m_UserData->_userDataUnsignedMap.find(key)->second = data;
    }
    else{
        CCLOG("This is the first time to play.");
        CCLOG("%s : %d", key.c_str(), m_UserData->_userDataUnsignedMap.find(key)->second);
    }
}

void CUserDataManager::convertJsonToUserData_List(std::string key, std::string valueJson)
{
    std::vector<unsigned>* list;
    
    if(m_UserData->_userDataListMap.find(key) == m_UserData->_userDataListMap.end()){
        CCLOG("There is no list with this key %s", key.c_str());
        CCASSERT(false, "Wrong Key");
    }
    
    list = m_UserData->_userDataListMap.find(key)->second;
    CCLOG("%s : %s", key.c_str(), valueJson.c_str());
    
    if (valueJson != ""){
        JSONREADER_CREATE
        const Json::Value dataArray = root["data"];
        for(auto element : dataArray)
        {
            list->push_back(element.asUInt());
        }
    }
    else{
        CCLOG("This is the first time to play.");
        list->push_back(0);
    }
}

void CUserDataManager::convertUserDataToJson_Number(std::string key)
{
    JSONWRITER_CREATE
    root["data"] = getUserData_Number(key);
    std::string dataStr = writer.write(root);
    
    // 암호화
    std::string crypto_key = MakeCryptoString(key, CRYPTO_KEY);
    std::string crypto_value = MakeCryptoString(dataStr, CRYPTO_KEY);
    
    UserDefault::getInstance()->setStringForKey(crypto_key.c_str(), crypto_value);
    setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
    
    if (CGoogleCloudManager::Instance()->getIsConnected())
        CGoogleCloudManager::Instance()->GoogleCloudDataSave(crypto_key.c_str(), crypto_value);
}

void CUserDataManager::convertUserDataToJson_List(std::string key)
{
    JSONWRITER_CREATE
    Json::Value jsonItemList;
    auto list = getUserData_List(key);
    
    for(auto data : *list)
    {
        jsonItemList.append(static_cast<unsigned>(data));
    }
    root["data"] = jsonItemList;
    std::string dataStr = writer.write(root);
    
    // 암호화
    std::string crypto_key = MakeCryptoString(key, CRYPTO_KEY);
    std::string crypto_value = MakeCryptoString(dataStr, CRYPTO_KEY);
    
    UserDefault::getInstance()->setStringForKey(crypto_key.c_str(), crypto_value);
    setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
    
    if (CGoogleCloudManager::Instance()->getIsConnected())
        CGoogleCloudManager::Instance()->GoogleCloudDataSave(crypto_key.c_str(), crypto_value);
}

void CUserDataManager::convertUserDataToJson_Revision()
{
    std::string key = "USER_DATA_SAVE_REVISION";
    
    JSONWRITER_CREATE
    root["data"] = getUserData_Number(key);
    std::string dataStr = writer.write(root);
    
    // 암호화
    std::string crypto_key = MakeCryptoString(key, CRYPTO_KEY);
    std::string crypto_value = MakeCryptoString(dataStr, CRYPTO_KEY);
    
    UserDefault::getInstance()->setStringForKey(crypto_key.c_str(), crypto_value);
    
    if (CGoogleCloudManager::Instance()->getIsConnected())
        CGoogleCloudManager::Instance()->GoogleCloudDataSave(crypto_key.c_str(), crypto_value);
}

void CUserDataManager::addKey(std::string keyKind, std::string key)
{
    m_UserData->_userDataKeyMap.emplace(std::pair<std::string, std::string>(key, keyKind));
    CCLOG("Kind : %s Add Key : %s ", keyKind.c_str(), key.c_str());
}