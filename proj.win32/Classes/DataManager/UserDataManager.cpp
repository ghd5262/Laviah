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
    ssize_t bufferSize = 0;
    unsigned char* userDefaultDataListJson = FileUtils::getInstance()->getFileData(strUserDefaultDataList.c_str(), "rb", &bufferSize);
    std::string userDefaultDataListClearData((const char*)userDefaultDataListJson);
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

void CUserDataManager::GoogleLoginResult()
{
    CCLOG("GoogleLoginResult Called");
    if (CGoogleCloudManager::Instance()->getIsConnected())
    {
        CCLOG("GoogleLoginResult Call google revision function");
        
        // 리비전 비교 위해 함수 호출
        std::string crypto_key = MakeCryptoString("USER_DATA_SAVE_REVISION", CRYPTO_KEY);
        CSDKUtil::Instance()->GoogleCloudLoad(crypto_key.c_str());
        
        m_IsFirstRevisionCall = true;
    }
    else{
        CCLOG("GoogleLoginResult Call xml revision function");
        
        // XML로부터 데이터 로드
        dataLoadFromXML();
    }
}

void CUserDataManager::afterCallFirstRevision() {
    
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
         * 때문에 구글 클라우드의 Revision이 높을 경우에는 항상 XML에 모두 저장한다. 
         * 전제조건으로 게임을 새로 설치한 경우에는 로그인하도록 유도한다. */
	}
}

void CUserDataManager::dataLoadFromGoogleCloud()
{
    for(auto keyInfo : m_UserData->_userDataKeyMap)
    {
        std::string crypto_key = MakeCryptoString(keyInfo.first.c_str(), CRYPTO_KEY);
        CSDKUtil::Instance()->GoogleCloudLoad(crypto_key.c_str());
    }
}

void CUserDataManager::dataLoadFromXML()
{
    for(auto keyInfo : m_UserData->_userDataKeyMap){
            std::string crypto_key = MakeCryptoString(keyInfo.first.c_str(), CRYPTO_KEY);
            userDataLoad(crypto_key.c_str(), UserDefault::getInstance()->getStringForKey(crypto_key.c_str(), ""));
    }
}

void CUserDataManager::addKey(std::string keyKind, std::string key)
{
    m_UserData->_userDataKeyMap.emplace(std::pair<std::string, std::string>(key, keyKind));
    CCLOG("Kind : %s Add Key : %s ", keyKind.c_str(), key.c_str());
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

unsigned CUserDataManager::getUserData_Number(std::string key)
{
    if(m_UserData->_userDataUnsignedMap.find(key) != m_UserData->_userDataUnsignedMap.end()){
        auto value = m_UserData->_userDataUnsignedMap.find(key)->second;
        return value;
    }
    CCLOG("There is no user data key : %s", key.c_str());
    CCASSERT(false, "Wrong Key");
}

void CUserDataManager::setUserData_Number(std::string key, unsigned value)
{
    if(m_UserData->_userDataUnsignedMap.find(key) != m_UserData->_userDataUnsignedMap.end())
        m_UserData->_userDataUnsignedMap.find(key)->second = value;
    
    userDataSave_Number(key);
}

void CUserDataManager::setSaveRevision(unsigned value)
{
    if(m_UserData->_userDataUnsignedMap.find("USER_DATA_SAVE_REVISION") != m_UserData->_userDataUnsignedMap.end())
        m_UserData->_userDataUnsignedMap.find("USER_DATA_SAVE_REVISION")->second = value;
    
    userDataSave_Revision();
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


// 오름차순 compare
unsigned compare (unsigned a, unsigned b)
{
    return ( a < b );
}

void CUserDataManager::setUserData_ItemGet(std::string key, unsigned itemIdx)
{
    if(m_UserData->_userDataListMap.find(key) != m_UserData->_userDataListMap.end()){
        auto list = m_UserData->_userDataListMap.find(key)->second;

        list->push_back(itemIdx);
        
        std::sort(list->begin(), list->end(), compare);
    }
    userDataSave_List(key);
}


void CUserDataManager::userDataSave_Number(std::string key)
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
        CSDKUtil::Instance()->GoogleCloudSave(crypto_key.c_str(), crypto_value);
}

void CUserDataManager::userDataSave_List(std::string key)
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
        CSDKUtil::Instance()->GoogleCloudSave(crypto_key.c_str(), crypto_value);
}

void CUserDataManager::userDataSave_Revision()
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
        CSDKUtil::Instance()->GoogleCloudSave(crypto_key.c_str(), crypto_value);
}


void CUserDataManager::userDataLoad(std::string key, std::string valueJson)
{
    // 복호화
    std::string decrypto_key = MakeCryptoString(key, CRYPTO_KEY);
    std::string decrypto_value = MakeCryptoString(valueJson, CRYPTO_KEY);
    
    CCLOG("===========================GoogleCloudLoad============================");
    CCLOG("Decrypto Key : %s", decrypto_key.c_str());
    CCLOG("Decrypto Value : %s", decrypto_value.c_str());
    CCLOG("======================================================================");
    
    if(m_UserData->_userDataKeyMap.find(decrypto_key) != m_UserData->_userDataKeyMap.end()){
        if(m_UserData->_userDataKeyMap.find(decrypto_key)->second == "userDefaultDatas_Number")
        {
            userDataLoad_Number(decrypto_key, decrypto_value);
        }
        else if(m_UserData->_userDataKeyMap.find(decrypto_key)->second == "userDefaultDatas_List")
        {
            userDataLoad_List(decrypto_key, decrypto_value);
        }
    }
}

void CUserDataManager::userDataLoad_Number(std::string key, std::string valueJson)
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

void CUserDataManager::userDataLoad_List(std::string key, std::string valueJson)
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

float CUserDataManager::getStarItemLimitTime()
{
    sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByKey("USER_STAR_LIMIT_TIME_IDX");
    float limitTime = item._valuePerLevel * getUserData_Number("USER_STAR_LIMIT_TIME_IDX");
    return limitTime;
}

float CUserDataManager::getCoinItemLimitTime()
{
    sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByKey("USER_COIN_LIMIT_TIME_IDX");
    float limitTime = item._valuePerLevel * getUserData_Number("USER_COIN_LIMIT_TIME_IDX");
    return limitTime;
}

float CUserDataManager::getGiantItemLimitTime()
{
    sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByKey("USER_GIANT_LIMIT_TIME_IDX");
    float limitTime = item._valuePerLevel * getUserData_Number("USER_GIANT_LIMIT_TIME_IDX");
    return limitTime;
}

float CUserDataManager::getBonusItemLimitTime()
{
    sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByKey("USER_BONUS_LIMIT_TIME_IDX");
    float limitTime = item._valuePerLevel * getUserData_Number("USER_BONUS_LIMIT_TIME_IDX");
    return limitTime;
}

float CUserDataManager::getMagnetItemLimitTime()
{
    sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByKey("USER_MAGNET_LIMIT_TIME_IDX");
    float limitTime = item._valuePerLevel * getUserData_Number("USER_MAGNET_LIMIT_TIME_IDX");
    return limitTime;
}

float CUserDataManager::getMagnetItemLimitRadius()
{
    sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByKey("USER_MAGNET_LIMIT_SIZE_IDX");
    float limitTime = item._valuePerLevel * getUserData_Number("USER_MAGNET_LIMIT_SIZE_IDX");
    return limitTime;
}


//unsigned CUserDataManager::getLevel()
//{
//    return m_UserData->_level;
//}

//void CUserDataManager::setLevel(unsigned value)
//{
//    m_UserData->_level = value;
//    UserDefault::getInstance()->setIntegerForKey("USER_LEVEL", value);
//}

//unsigned CUserDataManager::getSaveRevision()
//{
//    return m_UserData->_saveRevision;
//}

//unsigned CUserDataManager::getCoin()
//{
//    return m_UserData->_coin;
//}

//void CUserDataManager::setCoin(unsigned value)
//{
//    m_UserData->_coin = value;
//    dataSave_UserCoin();
//}

//unsigned CUserDataManager::getHealthLevel()
//{
//    return m_UserData->_healthLevel;
//}

//void CUserDataManager::setHealthLevel(unsigned value)
//{
//    m_UserData->_healthLevel = value;
//    dataSave_UserHealth();
//}

//unsigned CUserDataManager::getStarItemLimitTimeLevel()
//{
//    return m_UserData->_starItemLimitTimeLevel;
//}

//void CUserDataManager::setStarItemLimitTimeLevel(unsigned value)
//{
//    m_UserData->_starItemLimitTimeLevel = value;
//    dataSave_UserStarItemLimitTime();
//}

//unsigned CUserDataManager::getCoinItemLimitTimeLevel()
//{
//    return m_UserData->_coinItemLimitTimeLevel;
//}

//void CUserDataManager::setCoinItemLimitTimeLevel(unsigned value)
//{
//    m_UserData->_coinItemLimitTimeLevel = value;
//    dataSave_UserCoin();
//}

//unsigned CUserDataManager::getBonusItemLimitTimeLevel()
//{
//    return m_UserData->_bonusItemLimitTimeLevel;
//}

//void CUserDataManager::setBonusItemLimitTimeLevel(unsigned value)
//{
//    m_UserData->_bonusItemLimitTimeLevel = value;
//    dataSave_UserBonusItemLimitTime();
//}

//unsigned CUserDataManager::getGiantItemLimitTimeLevel()
//{
//    return m_UserData->_giantItemLimitTimeLevel;
//}

//void CUserDataManager::setGiantItemLimitTimeLevel(unsigned value)
//{
//    m_UserData->_giantItemLimitTimeLevel = value;
//    dataSave_UserGiantItemLimitTime();
//}

//unsigned CUserDataManager::getMagnetItemLimitTimeLevel()
//{
//    return m_UserData->_magnetItemLimitTimeLevel;
//}

//void CUserDataManager::setMagnetItemLimitTimeLevel(unsigned value)
//{
//    m_UserData->_magnetItemLimitTimeLevel = value;
//    dataSave_UserMagnetItemLimitTime();
//}

//unsigned CUserDataManager::getMagnetItemLimitRadiusLevel()
//{
//    return m_UserData->_magnetItemLimitRadiusLevel;
//}

//void CUserDataManager::setMagnetItemLimitRadiusLevel(unsigned value)
//{
//    m_UserData->_magnetItemLimitRadiusLevel = value;
//    dataSave_UserMagnetItemLimitRadius();
//}


//unsigned CUserDataManager::getBestTotalScore()
//{
//    return m_UserData->_bestTotalScore;
//}

//void CUserDataManager::setBestTotalScore(unsigned value)
//{
//    m_UserData->_bestTotalScore = value;
//    dataSave_UserBestTotalScore();
//}

//unsigned CUserDataManager::getBestCombo()
//{
//    return m_UserData->_bestCombo;
//}

//void CUserDataManager::setBestCombo(unsigned value)
//{
//    m_UserData->_bestCombo = value;
//    dataSave_UserBestCombo();
//}

//unsigned CUserDataManager::getCurrentCharacter()
//{
//    return m_UserData->_currentCharacter;
//}

//void CUserDataManager::setCurrentCharacter(unsigned value)
//{
//    m_UserData->_currentCharacter = value;
//    dataSave_UserCurrentCharacter();
//}

//unsigned CUserDataManager::getCurrentPet()
//{
//    return m_UserData->_currentPet;
//}

//void CUserDataManager::setCurrentPet(unsigned value)
//{
//    m_UserData->_currentPet = value;
//    dataSave_UserCurrentPet();
//}

//unsigned CUserDataManager::getCurrentRocket()
//{
//    return m_UserData->_currentRocket;
//}

//void CUserDataManager::setCurrentRocket(unsigned value)
//{
//    m_UserData->_currentRocket = value;
//    dataSave_UserCurrentRocket();
//}

//unsigned CUserDataManager::getCurrentChallenge()
//{
//    return m_UserData->_currentChallenge;
//}

//void CUserDataManager::setCurrentChallenge(unsigned value)
//{
//    m_UserData->_currentChallenge = value;
//    dataSave_UserCurrentChallenge();
//}

//bool CUserDataManager::isCharacterHave(int characterIdx)
//{
//    if (m_UserData->_characterList.size() > 0)
//        return m_UserData->_characterList.at(characterIdx);
//    return false;
//}

//void CUserDataManager::haveCharacter(int characterIdx)
//{
//    m_UserData->_characterList.at(characterIdx) = true;
//    dataSave_UserCharacterList();
//}

//bool CUserDataManager::isPetHave(int petIdx)
//{
//    if (m_UserData->_petList.size() > 0)
//        return m_UserData->_petList.at(petIdx);
//    return false;
//}

//void CUserDataManager::havePet(int petIdx)
//{
//    m_UserData->_petList.at(petIdx) = true;
//    dataSave_UserPetList();
//}

//bool CUserDataManager::isRocketHave(int rocketIdx)
//{
//    if (m_UserData->_rocketList.size() > 0)
//        return m_UserData->_rocketList.at(rocketIdx);
//    return false;
//}

//void CUserDataManager::haveRocket(int rocketIdx)
//{
//    m_UserData->_rocketList.at(rocketIdx) = true;
//    dataSave_UserRocketList();
//}


//void CUserDataManager::dataLoad_UserDataSaveRevision(std::string value)
//{
//    if (value != ""){
//        JSONREADER_CREATE
//        unsigned revision = root["data"].asInt();
//        CCLOG("SaveRevision : %d", revision);
//        
//        
//        m_UserData->_saveRevision = revision;
//    }
//    else{
//        CCLOG("This is the first time to play.");
//		m_UserData->_saveRevision = 0;
//	}
//}
//
//void CUserDataManager::dataLoad_UserLevel(std::string value)
//{
//	if (value != ""){
//		JSONREADER_CREATE
//			unsigned userLevel = root["data"].asInt();
//		CCLOG("LEVEL : %d", userLevel);
//
//		m_UserData->_level = userLevel;
//	}
//	else{
//		CCLOG("This is the first time to play.");
//		m_UserData->_level = 1;
//	}
//}
//
//void CUserDataManager::dataLoad_UserCoin(std::string value)
//{
//	if (value != ""){
//		JSONREADER_CREATE
//			unsigned userCoin = root["data"].asInt();
//		CCLOG("COIN : %d", userCoin);
//
//		m_UserData->_coin = userCoin;
//	}
//	else{
//		CCLOG("This is the first time to play.");
//		m_UserData->_coin = 0;
//	}
//}
//
//void CUserDataManager::dataLoad_UserHealth(std::string value)
//{
//	if (value != ""){
//		JSONREADER_CREATE
//			unsigned itemLevel = root["data"].asInt();
//		sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByKey("Health");
//		float health = item._valuePerLevel * itemLevel;
//		CCLOG("HEALTH_IDX : %d value : %f", itemLevel, health);
//
//		m_UserData->_healthLevel = itemLevel;
//	}
//	else{
//		CCLOG("This is the first time to play.");
//		m_UserData->_healthLevel = 0;
//	}
//}
//
//void CUserDataManager::dataLoad_UserStarItemLimitTime(std::string value)
//{
//	if (value != ""){
//		JSONREADER_CREATE
//			unsigned itemLevel = root["data"].asInt();
//		sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByKey("USER_STAR_LIMIT_TIME_IDX");
//		float limitTime = item._valuePerLevel * itemLevel;
//		CCLOG("STAR_LIMIT_TIME_IDX : %d value : %f", itemLevel, limitTime);
//
//		m_UserData->_starItemLimitTimeLevel = itemLevel;
//	}
//	else{
//		CCLOG("This is the first time to play.");
//		m_UserData->_starItemLimitTimeLevel = 0;
//	}
//}
//
//void CUserDataManager::dataLoad_UserCoinItemLimitTime(std::string value)
//{
//	if (value != ""){
//		JSONREADER_CREATE
//			unsigned itemLevel = root["data"].asInt();
//		sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByKey("USER_COIN_LIMIT_TIME_IDX");
//		float limitTime = item._valuePerLevel * itemLevel;
//		CCLOG("COIN_LIMIT_TIME_IDX : %d value : %f", itemLevel, limitTime);
//
//		m_UserData->_coinItemLimitTimeLevel = itemLevel;
//	}
//	else{
//		CCLOG("This is the first time to play.");
//		m_UserData->_coinItemLimitTimeLevel = 0;
//	}
//}
//
//void CUserDataManager::dataLoad_UserGiantItemLimitTime(std::string value)
//{
//	if (value != ""){
//		JSONREADER_CREATE
//			unsigned itemLevel = root["data"].asInt();
//		sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByKey("USER_GIANT_LIMIT_TIME_IDX");
//		float limitTime = item._valuePerLevel * itemLevel;
//		CCLOG("GIANT_LIMIT_TIME_IDX : %d value : %f", itemLevel, limitTime);
//
//		m_UserData->_giantItemLimitTimeLevel = itemLevel;
//	}
//	else{
//		CCLOG("This is the first time to play.");
//		m_UserData->_giantItemLimitTimeLevel = 0;
//	}
//}
//
//void CUserDataManager::dataLoad_UserBonusItemLimitTime(std::string value)
//{
//	if (value != ""){
//		JSONREADER_CREATE
//			unsigned itemLevel = root["data"].asInt();
//		sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByKey("USER_BONUS_LIMIT_TIME_IDX");
//		float limitTime = item._valuePerLevel * itemLevel;
//		CCLOG("BONUS_LIMIT_TIME_IDX : %d value : %f", itemLevel, limitTime);
//
//		m_UserData->_bonusItemLimitTimeLevel = itemLevel;
//	}
//	else{
//		CCLOG("This is the first time to play.");
//		m_UserData->_bonusItemLimitTimeLevel = 0;
//	}
//}
//
//void CUserDataManager::dataLoad_UserMagnetItemLimitTime(std::string value)
//{
//	if (value != ""){
//		JSONREADER_CREATE
//			unsigned itemLevel = root["data"].asInt();
//		sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByKey("USER_MAGNET_LIMIT_TIME_IDX");
//		float limitTime = item._valuePerLevel * itemLevel;
//		CCLOG("MAGNET_LIMIT_TIME_IDX : %d value : %f", itemLevel, limitTime);
//
//		m_UserData->_magnetItemLimitTimeLevel = itemLevel;
//	}
//	else{
//		CCLOG("This is the first time to play.");
//		m_UserData->_magnetItemLimitTimeLevel = 0;
//	}
//}
//
//void CUserDataManager::dataLoad_UserMagnetItemLimitRadius(std::string value)
//{
//	if (value != ""){
//		JSONREADER_CREATE
//			unsigned itemLevel = root["data"].asInt();
//		sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByKey("USER_MAGNET_LIMIT_SIZE_IDX");
//		float radius = item._valuePerLevel * itemLevel;
//		CCLOG("MAGNET_LIMIT_SIZE_IDX : %d value : %f", itemLevel, radius);
//
//		m_UserData->_magnetItemLimitRadiusLevel = itemLevel;
//	}
//	else{
//		CCLOG("This is the first time to play.");
//		m_UserData->_magnetItemLimitRadiusLevel = 0;
//	}
//}
//
//void CUserDataManager::dataLoad_UserBestTotalScore(std::string value)
//{
//	if (value != ""){
//		JSONREADER_CREATE
//			unsigned bestTotalScore = root["data"].asInt();
//		CCLOG("BEST_TOTAL_SCORE : %d", bestTotalScore);
//
//		m_UserData->_bestTotalScore = bestTotalScore;
//	}
//	else{
//		CCLOG("This is the first time to play.");
//		m_UserData->_bestTotalScore = 0;
//	}
//}
//
//void CUserDataManager::dataLoad_UserBestCombo(std::string value)
//{
//	if (value != ""){
//		JSONREADER_CREATE
//			unsigned bestCombo = root["data"].asInt();
//		CCLOG("BEST_COMBO : %d", bestCombo);
//
//		m_UserData->_bestCombo = bestCombo;
//	}
//	else{
//		CCLOG("This is the first time to play.");
//		m_UserData->_bestCombo = 0;
//	}
//}
//
//void CUserDataManager::dataLoad_UserCurrentCharacter(std::string value)
//{
//	if (value != ""){
//		JSONREADER_CREATE
//			unsigned currentCharacter = root["data"].asInt();
//		CCLOG("CUR_CHARACTER_IDX : %d", currentCharacter);
//
//		m_UserData->_currentCharacter = currentCharacter;
//	}
//	else{
//		CCLOG("This is the first time to play.");
//		m_UserData->_currentCharacter = 0;
//	}
//}
//
//void CUserDataManager::dataLoad_UserCurrentPet(std::string value)
//{
//	if (value != ""){
//		JSONREADER_CREATE
//			unsigned currentPet = root["data"].asInt();
//		CCLOG("CUR_PET_IDX : %d", currentPet);
//
//		m_UserData->_currentPet = currentPet;
//	}
//	else{
//		CCLOG("This is the first time to play.");
//		m_UserData->_currentPet = 0;
//	}
//}
//
//void CUserDataManager::dataLoad_UserCurrentRocket(std::string value)
//{
//	if (value != ""){
//		JSONREADER_CREATE
//			unsigned currentRocket = root["data"].asInt();
//		CCLOG("CUR_ROCKET_IDX : %d", currentRocket);
//
//		m_UserData->_currentRocket = currentRocket;
//	}
//	else{
//		CCLOG("This is the first time to play.");
//		m_UserData->_currentRocket = 0;
//	}
//}
//
//void CUserDataManager::dataLoad_UserCurrentChallenge(std::string value)
//{
//	if (value != ""){
//		JSONREADER_CREATE
//			unsigned currentChallenge = root["data"].asInt();
//		CCLOG("CUR_CHALLENGE_IDX : %d", currentChallenge);
//
//		m_UserData->_currentChallenge = currentChallenge;
//	}
//	else{
//		CCLOG("This is the first time to play.");
//		m_UserData->_currentChallenge = 0;
//	}
//}

//void CUserDataManager::dataLoad_UserCharacterList(std::string value)
//{
//    if (value != ""){
//        JSONREADER_CREATE
//        const Json::Value dataArray = root["data"];
//        for(auto element : dataArray)
//        {
//            m_UserData->_characterList.push_back(element.asBool());
//        }
//        CCLOG("CHARACTER_LIST : %s", value.c_str());
//    }
//    else{
//        CCLOG("This is the first time to play.");
//        size_t listSize = CCharacterDataManager::Instance()->getCharacterList().size();
//        
//        for(int idx = 0; idx < listSize; idx++)
//        {
//            m_UserData->_characterList.push_back(false);
//        }
//        m_UserData->_characterList.at(0) = true;
//    }
//}
//
//void CUserDataManager::dataLoad_UserPetList(std::string value)
//{
//    if (value != ""){
//        JSONREADER_CREATE
//        const Json::Value dataArray = root["data"];
//        for(auto element : dataArray)
//        {
//            m_UserData->_petList.push_back(element.asBool());
//        }
//        CCLOG("PET_LIST : %s", value.c_str());
//    }
//    else{
//        CCLOG("This is the first time to play.");
//        size_t listSize = CCharacterDataManager::Instance()->getCharacterList().size();
//        
//        for(int idx = 0; idx < listSize; idx++)
//        {
//            m_UserData->_petList.push_back(false);
//        }
//        m_UserData->_petList.at(0) = true; 
//    }
//}
//
//void CUserDataManager::dataLoad_UserRocketList(std::string value)
//{
//    if (value != ""){
//        JSONREADER_CREATE
//        const Json::Value dataArray = root["data"];
//        for(auto element : dataArray)
//        {
//            m_UserData->_rocketList.push_back(element.asBool());
//        }
//        CCLOG("ROCKET_LIST : %s", value.c_str());
//    }
//    else{
//        CCLOG("This is the first time to play.");
//        size_t listSize = CCharacterDataManager::Instance()->getCharacterList().size();
//        
//        for(int idx = 0; idx < listSize; idx++)
//        {
//            m_UserData->_rocketList.push_back(false);
//        }
//        m_UserData->_rocketList.at(0) = true;
//    }
//}


//void CUserDataManager::dataSave_UserLevel()
//{
//	JSONWRITER_CREATE
//		root["data"] = m_UserData->_level;
//	std::string dataStr = writer.write(root);
//
//	UserDefault::getInstance()->setStringForKey("USER_LEVEL", dataStr);
//	setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//
//	if (CGoogleCloudManager::Instance()->getIsConnected())
//		CSDKUtil::Instance()->GoogleCloudSave("USER_LEVEL", dataStr);
//}
//
//void CUserDataManager::dataSave_UserCoin()
//{
//	JSONWRITER_CREATE
//		root["data"] = m_UserData->_coin;
//	std::string dataStr = writer.write(root);
//
//	UserDefault::getInstance()->setStringForKey("USER_COIN", dataStr);
//	setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//
//	if (CGoogleCloudManager::Instance()->getIsConnected())
//		CSDKUtil::Instance()->GoogleCloudSave("USER_COIN", dataStr);
//
//}
//
//void CUserDataManager::dataSave_UserHealth()
//{
//	JSONWRITER_CREATE
//		root["data"] = m_UserData->_healthLevel;
//	std::string dataStr = writer.write(root);
//
//	UserDefault::getInstance()->setStringForKey("USER_HEALTH_IDX", dataStr);
//	setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//
//	if (CGoogleCloudManager::Instance()->getIsConnected())
//		CSDKUtil::Instance()->GoogleCloudSave("USER_HEALTH_IDX", dataStr);
//}
//
//void CUserDataManager::dataSave_UserStarItemLimitTime()
//{
//	JSONWRITER_CREATE
//		root["data"] = m_UserData->_starItemLimitTimeLevel;
//	std::string dataStr = writer.write(root);
//
//	UserDefault::getInstance()->setStringForKey("USER_STAR_LIMIT_TIME_IDX", dataStr);
//	setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//
//	if (CGoogleCloudManager::Instance()->getIsConnected())
//		CSDKUtil::Instance()->GoogleCloudSave("USER_STAR_LIMIT_TIME_IDX", dataStr);
//}
//
//void CUserDataManager::dataSave_UserCoinItemLimitTime()
//{
//	JSONWRITER_CREATE
//		root["data"] = m_UserData->_coinItemLimitTimeLevel;
//	std::string dataStr = writer.write(root);
//
//	UserDefault::getInstance()->setStringForKey("USER_COIN_LIMIT_TIME_IDX", dataStr);
//	setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//
//	if (CGoogleCloudManager::Instance()->getIsConnected())
//		CSDKUtil::Instance()->GoogleCloudSave("USER_COIN_LIMIT_TIME_IDX", dataStr);
//}
//
//void CUserDataManager::dataSave_UserGiantItemLimitTime()
//{
//	JSONWRITER_CREATE
//		root["data"] = m_UserData->_giantItemLimitTimeLevel;
//	std::string dataStr = writer.write(root);
//
//	UserDefault::getInstance()->setStringForKey("USER_GIANT_LIMIT_TIME_IDX", dataStr);
//	setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//
//	if (CGoogleCloudManager::Instance()->getIsConnected())
//		CSDKUtil::Instance()->GoogleCloudSave("USER_GIANT_LIMIT_TIME_IDX", dataStr);
//}
//
//void CUserDataManager::dataSave_UserBonusItemLimitTime()
//{
//	JSONWRITER_CREATE
//		root["data"] = m_UserData->_bonusItemLimitTimeLevel;
//	std::string dataStr = writer.write(root);
//
//	UserDefault::getInstance()->setStringForKey("USER_BONUS_LIMIT_TIME_IDX", dataStr);
//	setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//
//	if (CGoogleCloudManager::Instance()->getIsConnected())
//		CSDKUtil::Instance()->GoogleCloudSave("USER_BONUS_LIMIT_TIME_IDX", dataStr);
//}
//
//void CUserDataManager::dataSave_UserMagnetItemLimitTime()
//{
//	JSONWRITER_CREATE
//		root["data"] = m_UserData->_magnetItemLimitTimeLevel;
//	std::string dataStr = writer.write(root);
//
//	UserDefault::getInstance()->setStringForKey("USER_MAGNET_LIMIT_TIME_IDX", dataStr);
//	setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//
//	if (CGoogleCloudManager::Instance()->getIsConnected())
//		CSDKUtil::Instance()->GoogleCloudSave("USER_MAGNET_LIMIT_TIME_IDX", dataStr);
//}
//
//void CUserDataManager::dataSave_UserMagnetItemLimitRadius()
//{
//	JSONWRITER_CREATE
//		root["data"] = m_UserData->_magnetItemLimitRadiusLevel;
//	std::string dataStr = writer.write(root);
//
//	UserDefault::getInstance()->setStringForKey("USER_MAGNET_LIMIT_SIZE_IDX", dataStr);
//	setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//
//	if (CGoogleCloudManager::Instance()->getIsConnected())
//		CSDKUtil::Instance()->GoogleCloudSave("USER_MAGNET_LIMIT_SIZE_IDX", dataStr);
//}
//
//void CUserDataManager::dataSave_UserBestTotalScore()
//{
//	JSONWRITER_CREATE
//		root["data"] = m_UserData->_bestTotalScore;
//	std::string dataStr = writer.write(root);
//
//	UserDefault::getInstance()->setStringForKey("USER_BEST_TOTAL_SCORE", dataStr);
//	setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//
//	if (CGoogleCloudManager::Instance()->getIsConnected())
//		CSDKUtil::Instance()->GoogleCloudSave("USER_BEST_TOTAL_SCORE", dataStr);
//}
//
//void CUserDataManager::dataSave_UserBestCombo()
//{
//	JSONWRITER_CREATE
//		root["data"] = m_UserData->_bestCombo;
//	std::string dataStr = writer.write(root);
//
//	UserDefault::getInstance()->setStringForKey("USER_BEST_COMBO", dataStr);
//	setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//
//	if (CGoogleCloudManager::Instance()->getIsConnected())
//		CSDKUtil::Instance()->GoogleCloudSave("USER_BEST_COMBO", dataStr);
//}
//
//void CUserDataManager::dataSave_UserCurrentCharacter()
//{
//	JSONWRITER_CREATE
//		root["data"] = m_UserData->_currentCharacter;
//	std::string dataStr = writer.write(root);
//
//	UserDefault::getInstance()->setStringForKey("USER_CUR_CHARACTER", dataStr);
//	setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//
//	if (CGoogleCloudManager::Instance()->getIsConnected())
//		CSDKUtil::Instance()->GoogleCloudSave("USER_CUR_CHARACTER", dataStr);
//}
//
//void CUserDataManager::dataSave_UserCurrentPet()
//{
//	JSONWRITER_CREATE
//		root["data"] = m_UserData->_currentPet;
//	std::string dataStr = writer.write(root);
//
//	UserDefault::getInstance()->setStringForKey("USER_CUR_PET", dataStr);
//	setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//
//	if (CGoogleCloudManager::Instance()->getIsConnected())
//		CSDKUtil::Instance()->GoogleCloudSave("USER_CUR_PET", dataStr);
//}
//
//void CUserDataManager::dataSave_UserCurrentRocket()
//{
//	JSONWRITER_CREATE
//		root["data"] = m_UserData->_currentRocket;
//	std::string dataStr = writer.write(root);
//
//	UserDefault::getInstance()->setStringForKey("USER_CUR_ROCKET", dataStr);
//	setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//
//	if (CGoogleCloudManager::Instance()->getIsConnected())
//		CSDKUtil::Instance()->GoogleCloudSave("USER_CUR_ROCKET", dataStr);
//}
//
//void CUserDataManager::dataSave_UserCurrentChallenge()
//{
//	JSONWRITER_CREATE
//		root["data"] = m_UserData->_currentChallenge;
//	std::string dataStr = writer.write(root);
//
//	UserDefault::getInstance()->setStringForKey("USER_CUR_CHALLENGE", dataStr);
//	setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//
//	if (CGoogleCloudManager::Instance()->getIsConnected())
//		CSDKUtil::Instance()->GoogleCloudSave("USER_CUR_CHALLENGE", dataStr);
//}

//void CUserDataManager::dataSave_UserCharacterList()
//{
//    JSONWRITER_CREATE
//    Json::Value list;
//    for(auto data : m_UserData->_characterList)
//    {
//        list.append(static_cast<int>(data));
//    }
//    root["data"] = list;
//    std::string dataStr = writer.write(root);
//    
//    UserDefault::getInstance()->setStringForKey("USER_CHARACTER_LIST", dataStr);
//    setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//    
//    if (CGoogleCloudManager::Instance()->getIsConnected())
//        CSDKUtil::Instance()->GoogleCloudSave("USER_CHARACTER_LIST", dataStr);
//}
//
//void CUserDataManager::dataSave_UserPetList()
//{
//    JSONWRITER_CREATE
//    Json::Value list;
//    for(auto data : m_UserData->_petList)
//    {
//        list.append(static_cast<int>(data));
//    }
//    root["data"] = list;
//    std::string dataStr = writer.write(root);
//    
//    UserDefault::getInstance()->setStringForKey("USER_PET_LIST", dataStr);
//    setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//    
//    if (CGoogleCloudManager::Instance()->getIsConnected())
//        CSDKUtil::Instance()->GoogleCloudSave("USER_PET_LIST", dataStr);
//}
//
//void CUserDataManager::dataSave_UserRocketList()
//{
//    JSONWRITER_CREATE
//    Json::Value list;
//    for(auto data : m_UserData->_rocketList)
//    {
//        list.append(static_cast<int>(data));
//    }
//    root["data"] = list;
//    std::string dataStr = writer.write(root);
//    
//    UserDefault::getInstance()->setStringForKey("USER_ROCKET_LIST", dataStr);
//    setSaveRevision(getUserData_Number("USER_DATA_SAVE_REVISION") + 1);
//    
//    if (CGoogleCloudManager::Instance()->getIsConnected())
//        CSDKUtil::Instance()->GoogleCloudSave("USER_ROCKET_LIST", dataStr);
//}