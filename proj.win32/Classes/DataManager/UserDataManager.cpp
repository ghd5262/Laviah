#include "UserDataManager.h"
#include "../Scene/GameScene.h"
#include "../MyUI/MyButton.h"
#include "../MyUI/Popup.h"
#include "../MyUI/Popup/EarnCoinPopup.h"
#include "../json/json.h"
#include "../DataManager/WorkshopItemDataManager.h"
#include "../SDKUtil/SDKUtil.h"
#include "../GoogleCloud/GoogleCloudManager.h"

#define JSONREADER_CREATE \
	Json::Value root; \
	Json::Reader reader; \
	bool parsingSuccessful = reader.parse(value, root); \
	if (!parsingSuccessful) \
	{ \
		CCLOG("Error : Failed to parse configuration\n", reader.getFormatedErrorMessages()); \
		return; \
	}

#define JSONWRITER_CREATE \
	Json::Value root; \
	Json::Value data; \
	Json::StyledWriter writer;

CUserDataManager::CUserDataManager()
{
    m_UserData = std::shared_ptr<sUSER_DATA>(new sUSER_DATA(), [](sUSER_DATA* userData)
                                             {
                                                 delete userData;
                                             });
    
	addKey("USER_DATA_SAVE_REVISION");
	addKey("USER_LEVEL");
	addKey("USER_COIN");
	addKey("USER_HEALTH_IDX");
	addKey("USER_STAR_LIMIT_TIME_IDX");
	addKey("USER_COIN_LIMIT_TIME_IDX");
	addKey("USER_GIANT_LIMIT_TIME_IDX");
	addKey("USER_BONUS_LIMIT_TIME_IDX");
	addKey("USER_MAGNET_LIMIT_TIME_IDX");
	addKey("USER_MAGNET_LIMIT_SIZE_IDX");
	addKey("USER_BEST_TOTAL_SCORE");
	addKey("USER_BEST_COMBO");
	addKey("USER_CUR_CHARACTER");
	addKey("USER_CUR_PET");
	addKey("USER_CUR_ROCKET");
	addKey("USER_CUR_CHALLENGE");
	addKey("USER_CHARACTER_LIST");
	addKey("USER_PET_LIST");
	addKey("USER_ROCKET_LIST");

	DataLoad();
}

CUserDataManager::~CUserDataManager()
{
}

CUserDataManager* CUserDataManager::Instance()
{
    static CUserDataManager instance;
    return &instance;
}

void CUserDataManager::DataLoad()
{
	/* saveRevision이 더 높은 데이터로 로딩한다.
	 * 게임중 인터넷이 끊기면 구글클라우드에는 저장하지 않기 때문에 대부분 xml의 revision이 높을 것이다.
	 * 하지만 게임을 지웠다가 다시 설치할 경우엔 클라우드의 데이터를 가져와야한다. */
	if (0)
	{
		dataLoadFromGoogleCloud();
	}
	else{
		dataLoadFromXML();
	}
}

void CUserDataManager::dataLoadFromGoogleCloud()
{
	CSDKUtil::Instance()->GoogleCloudLoad("USER_DATA_SAVE_REVISION");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_LEVEL");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_COIN");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_HEALTH_IDX");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_STAR_LIMIT_TIME_IDX");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_COIN_LIMIT_TIME_IDX");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_GIANT_LIMIT_TIME_IDX");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_BONUS_LIMIT_TIME_IDX");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_MAGNET_LIMIT_TIME_IDX");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_MAGNET_LIMIT_SIZE_IDX");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_BEST_TOTAL_SCORE");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_BEST_COMBO");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_CUR_CHARACTER");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_CUR_PET");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_CUR_ROCKET");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_CUR_CHALLENGE");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_CHARACTER_LIST");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_PET_LIST");
	CSDKUtil::Instance()->GoogleCloudLoad("USER_ROCKET_LIST");
}

void CUserDataManager::dataLoadFromXML()
{
	DataLoad_UserDataSaveRevision(UserDefault::getInstance()->getStringForKey("USER_DATA_SAVE_REVISION", ""));
	DataLoad_UserLevel(UserDefault::getInstance()->getStringForKey("USER_LEVEL", ""));
	DataLoad_UserCoin(UserDefault::getInstance()->getStringForKey("USER_COIN", ""));
	DataLoad_UserHealth(UserDefault::getInstance()->getStringForKey("USER_HEALTH_IDX", ""));
	DataLoad_UserStarItemLimitTime(UserDefault::getInstance()->getStringForKey("USER_STAR_LIMIT_TIME_IDX", ""));
	DataLoad_UserCoinItemLimitTime(UserDefault::getInstance()->getStringForKey("USER_COIN_LIMIT_TIME_IDX", ""));
	DataLoad_UserGiantItemLimitTime(UserDefault::getInstance()->getStringForKey("USER_GIANT_LIMIT_TIME_IDX", ""));
	DataLoad_UserBonusItemLimitTime(UserDefault::getInstance()->getStringForKey("USER_BONUS_LIMIT_TIME_IDX", ""));
	DataLoad_UserMagnetItemLimitTime(UserDefault::getInstance()->getStringForKey("USER_MAGNET_LIMIT_TIME_IDX", ""));
	DataLoad_UserMagnetItemLimitRadius(UserDefault::getInstance()->getStringForKey("USER_MAGNET_LIMIT_SIZE_IDX", ""));
	DataLoad_UserBestTotalScore(UserDefault::getInstance()->getStringForKey("USER_BEST_TOTAL_SCORE", ""));
	DataLoad_UserBestCombo(UserDefault::getInstance()->getStringForKey("USER_BEST_COMBO", ""));
	DataLoad_UserCurrentCharacter(UserDefault::getInstance()->getStringForKey("USER_CUR_CHARACTER", ""));
	DataLoad_UserCurrentPet(UserDefault::getInstance()->getStringForKey("USER_CUR_PET", ""));
	DataLoad_UserCurrentRocket(UserDefault::getInstance()->getStringForKey("USER_CUR_ROCKET", ""));
	DataLoad_UserCurrentChallenge(UserDefault::getInstance()->getStringForKey("USER_CUR_CHALLENGE", ""));
	DataLoad_UserCharacterList(UserDefault::getInstance()->getStringForKey("USER_CHARACTER_LIST", ""));
	DataLoad_UserPetList(UserDefault::getInstance()->getStringForKey("USER_PET_LIST", ""));
	DataLoad_UserRocketList(UserDefault::getInstance()->getStringForKey("USER_ROCKET_LIST", ""));
}


void CUserDataManager::DataLoad_UserDataSaveRevision(std::string value)
{
	if (value != ""){
		JSONREADER_CREATE
			size_t revision = root["data"].asInt();
		CCLOG("SaveRevision : %d", revision);


		m_UserData->_saveRevision = revision;
	}
	else{
		CCLOG("This is the first time to play.");
		m_UserData->_saveRevision = 0;
	}
}

void CUserDataManager::DataLoad_UserLevel(std::string value)
{
	if (value != ""){
		JSONREADER_CREATE
			size_t userLevel = root["data"].asInt();
		CCLOG("LEVEL : %d", userLevel);

		m_UserData->_level = userLevel;
	}
	else{
		CCLOG("This is the first time to play.");
		m_UserData->_level = 1;
	}
}

void CUserDataManager::DataLoad_UserCoin(std::string value)
{
	if (value != ""){
		JSONREADER_CREATE
			size_t userCoin = root["data"].asInt();
		CCLOG("COIN : %d", userCoin);

		m_UserData->_coin = userCoin;
	}
	else{
		CCLOG("This is the first time to play.");
		m_UserData->_coin = 0;
	}
}

void CUserDataManager::DataLoad_UserHealth(std::string value)
{
	if (value != ""){
		JSONREADER_CREATE
			size_t itemLevel = root["data"].asInt();
		sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByName("Health");
		float health = item._valuePerLevel * itemLevel;
		CCLOG("HEALTH_IDX : %d value : %f", itemLevel, health);

		m_UserData->_healthLevel = itemLevel;
	}
	else{
		CCLOG("This is the first time to play.");
		m_UserData->_healthLevel = 0;
	}
}

void CUserDataManager::DataLoad_UserStarItemLimitTime(std::string value)
{
	if (value != ""){
		JSONREADER_CREATE
			size_t itemLevel = root["data"].asInt();
		sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByName("StarTime");
		float limitTime = item._valuePerLevel * itemLevel;
		CCLOG("STAR_LIMIT_TIME_IDX : %d value : %f", itemLevel, limitTime);

		m_UserData->_starItemLimitTimeLevel = itemLevel;
	}
	else{
		CCLOG("This is the first time to play.");
		m_UserData->_starItemLimitTimeLevel = 0;
	}
}

void CUserDataManager::DataLoad_UserCoinItemLimitTime(std::string value)
{
	if (value != ""){
		JSONREADER_CREATE
			size_t itemLevel = root["data"].asInt();
		sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByName("CoinTime");
		float limitTime = item._valuePerLevel * itemLevel;
		CCLOG("COIN_LIMIT_TIME_IDX : %d value : %f", itemLevel, limitTime);

		m_UserData->_coinItemLimitTimeLevel = itemLevel;
	}
	else{
		CCLOG("This is the first time to play.");
		m_UserData->_coinItemLimitTimeLevel = 0;
	}
}

void CUserDataManager::DataLoad_UserGiantItemLimitTime(std::string value)
{
	if (value != ""){
		JSONREADER_CREATE
			size_t itemLevel = root["data"].asInt();
		sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByName("GiantTime");
		float limitTime = item._valuePerLevel * itemLevel;
		CCLOG("GIANT_LIMIT_TIME_IDX : %d value : %f", itemLevel, limitTime);

		m_UserData->_giantItemLimitTimeLevel = itemLevel;
	}
	else{
		CCLOG("This is the first time to play.");
		m_UserData->_giantItemLimitTimeLevel = 0;
	}
}

void CUserDataManager::DataLoad_UserBonusItemLimitTime(std::string value)
{
	if (value != ""){
		JSONREADER_CREATE
			size_t itemLevel = root["data"].asInt();
		sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByName("BonusTime");
		float limitTime = item._valuePerLevel * itemLevel;
		CCLOG("BONUS_LIMIT_TIME_IDX : %d value : %f", itemLevel, limitTime);

		m_UserData->_bonusItemLimitTimeLevel = itemLevel;
	}
	else{
		CCLOG("This is the first time to play.");
		m_UserData->_bonusItemLimitTimeLevel = 0;
	}
}

void CUserDataManager::DataLoad_UserMagnetItemLimitTime(std::string value)
{
	if (value != ""){
		JSONREADER_CREATE
			size_t itemLevel = root["data"].asInt();
		sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByName("MagnetTime");
		float limitTime = item._valuePerLevel * itemLevel;
		CCLOG("MAGNET_LIMIT_TIME_IDX : %d value : %f", itemLevel, limitTime);

		m_UserData->_magnetItemLimitTimeLevel = itemLevel;
	}
	else{
		CCLOG("This is the first time to play.");
		m_UserData->_magnetItemLimitTimeLevel = 0;
	}
}

void CUserDataManager::DataLoad_UserMagnetItemLimitRadius(std::string value)
{
	if (value != ""){
		JSONREADER_CREATE
			size_t itemLevel = root["data"].asInt();
		sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByName("MagnetSize");
		float radius = item._valuePerLevel * itemLevel;
		CCLOG("MAGNET_LIMIT_SIZE_IDX : %d value : %f", itemLevel, radius);

		m_UserData->_magnetItemLimitRadiusLevel = itemLevel;
	}
	else{
		CCLOG("This is the first time to play.");
		m_UserData->_magnetItemLimitRadiusLevel = 0;
	}
}

void CUserDataManager::DataLoad_UserBestTotalScore(std::string value)
{
	if (value != ""){
		JSONREADER_CREATE
			size_t bestTotalScore = root["data"].asInt();
		CCLOG("BEST_TOTAL_SCORE : %d", bestTotalScore);

		m_UserData->_bestTotalScore = bestTotalScore;
	}
	else{
		CCLOG("This is the first time to play.");
		m_UserData->_bestTotalScore = 0;
	}
}

void CUserDataManager::DataLoad_UserBestCombo(std::string value)
{
	if (value != ""){
		JSONREADER_CREATE
			size_t bestCombo = root["data"].asInt();
		CCLOG("BEST_COMBO : %d", bestCombo);

		m_UserData->_bestCombo = bestCombo;
	}
	else{
		CCLOG("This is the first time to play.");
		m_UserData->_bestCombo = 0;
	}
}

void CUserDataManager::DataLoad_UserCurrentCharacter(std::string value)
{
	if (value != ""){
		JSONREADER_CREATE
			size_t currentCharacter = root["data"].asInt();
		CCLOG("CUR_CHARACTER_IDX : %d", currentCharacter);

		m_UserData->_currentCharacter = currentCharacter;
	}
	else{
		CCLOG("This is the first time to play.");
		m_UserData->_currentCharacter = 0;
	}
}

void CUserDataManager::DataLoad_UserCurrentPet(std::string value)
{
	if (value != ""){
		JSONREADER_CREATE
			size_t currentPet = root["data"].asInt();
		CCLOG("CUR_PET_IDX : %d", currentPet);

		m_UserData->_currentPet = currentPet;
	}
	else{
		CCLOG("This is the first time to play.");
		m_UserData->_currentPet = 0;
	}
}

void CUserDataManager::DataLoad_UserCurrentRocket(std::string value)
{
	if (value != ""){
		JSONREADER_CREATE
			size_t currentRocket = root["data"].asInt();
		CCLOG("CUR_ROCKET_IDX : %d", currentRocket);

		m_UserData->_currentRocket = currentRocket;
	}
	else{
		CCLOG("This is the first time to play.");
		m_UserData->_currentRocket = 0;
	}
}

void CUserDataManager::DataLoad_UserCurrentChallenge(std::string value)
{
	if (value != ""){
		JSONREADER_CREATE
			size_t currentChallenge = root["data"].asInt();
		CCLOG("CUR_CHALLENGE_IDX : %d", currentChallenge);

		m_UserData->_currentChallenge = currentChallenge;
	}
	else{
		CCLOG("This is the first time to play.");
		m_UserData->_currentChallenge = 0;
	}
}

void CUserDataManager::DataLoad_UserCharacterList(std::string value)
{
}

void CUserDataManager::DataLoad_UserPetList(std::string value)
{
}

void CUserDataManager::DataLoad_UserRocketList(std::string value)
{
}



void CUserDataManager::dataSave_UserDataSaveRevision()
{
	JSONWRITER_CREATE
		root["data"] = m_UserData->_saveRevision;
	std::string dataStr = writer.write(root);

	CCUserDefault::getInstance()->setStringForKey("USER_DATA_SAVE_REVISION", dataStr);

	if (CGoogleCloudManager::Instance()->getIsConnected())
		CSDKUtil::Instance()->GoogleCloudSave("USER_DATA_SAVE_REVISION", dataStr);
}

void CUserDataManager::dataSave_UserLevel()
{
	JSONWRITER_CREATE
		root["data"] = m_UserData->_level;
	std::string dataStr = writer.write(root);

	CCUserDefault::getInstance()->setStringForKey("USER_LEVEL", dataStr);
	setSaveRevision(getSaveRevision() + 1);

	if (CGoogleCloudManager::Instance()->getIsConnected())
		CSDKUtil::Instance()->GoogleCloudSave("USER_LEVEL", dataStr);
}

void CUserDataManager::dataSave_UserCoin()
{
	JSONWRITER_CREATE
		root["data"] = m_UserData->_coin;
	std::string dataStr = writer.write(root);

	CCUserDefault::getInstance()->setStringForKey("USER_COIN", dataStr);
	setSaveRevision(getSaveRevision() + 1);

	if (CGoogleCloudManager::Instance()->getIsConnected())
		CSDKUtil::Instance()->GoogleCloudSave("USER_COIN", dataStr);

}

void CUserDataManager::dataSave_UserHealth()
{
	JSONWRITER_CREATE
		root["data"] = m_UserData->_healthLevel;
	std::string dataStr = writer.write(root);

	CCUserDefault::getInstance()->setStringForKey("USER_HEALTH_IDX", dataStr);
	setSaveRevision(getSaveRevision() + 1);

	if (CGoogleCloudManager::Instance()->getIsConnected())
		CSDKUtil::Instance()->GoogleCloudSave("USER_HEALTH_IDX", dataStr);
}

void CUserDataManager::dataSave_UserStarItemLimitTime()
{
	JSONWRITER_CREATE
		root["data"] = m_UserData->_starItemLimitTimeLevel;
	std::string dataStr = writer.write(root);

	CCUserDefault::getInstance()->setStringForKey("USER_STAR_LIMIT_TIME_IDX", dataStr);
	setSaveRevision(getSaveRevision() + 1);

	if (CGoogleCloudManager::Instance()->getIsConnected())
		CSDKUtil::Instance()->GoogleCloudSave("USER_STAR_LIMIT_TIME_IDX", dataStr);
}

void CUserDataManager::dataSave_UserCoinItemLimitTime()
{
	JSONWRITER_CREATE
		root["data"] = m_UserData->_coinItemLimitTimeLevel;
	std::string dataStr = writer.write(root);

	CCUserDefault::getInstance()->setStringForKey("USER_COIN_LIMIT_TIME_IDX", dataStr);
	setSaveRevision(getSaveRevision() + 1);

	if (CGoogleCloudManager::Instance()->getIsConnected())
		CSDKUtil::Instance()->GoogleCloudSave("USER_COIN_LIMIT_TIME_IDX", dataStr);
}

void CUserDataManager::dataSave_UserGiantItemLimitTime()
{
	JSONWRITER_CREATE
		root["data"] = m_UserData->_giantItemLimitTimeLevel;
	std::string dataStr = writer.write(root);

	CCUserDefault::getInstance()->setStringForKey("USER_GIANT_LIMIT_TIME_IDX", dataStr);
	setSaveRevision(getSaveRevision() + 1);

	if (CGoogleCloudManager::Instance()->getIsConnected())
		CSDKUtil::Instance()->GoogleCloudSave("USER_GIANT_LIMIT_TIME_IDX", dataStr);
}

void CUserDataManager::dataSave_UserBonusItemLimitTime()
{
	JSONWRITER_CREATE
		root["data"] = m_UserData->_bonusItemLimitTimeLevel;
	std::string dataStr = writer.write(root);

	CCUserDefault::getInstance()->setStringForKey("USER_BONUS_LIMIT_TIME_IDX", dataStr);
	setSaveRevision(getSaveRevision() + 1);

	if (CGoogleCloudManager::Instance()->getIsConnected())
		CSDKUtil::Instance()->GoogleCloudSave("USER_BONUS_LIMIT_TIME_IDX", dataStr);
}

void CUserDataManager::dataSave_UserMagnetItemLimitTime()
{
	JSONWRITER_CREATE
		root["data"] = m_UserData->_magnetItemLimitTimeLevel;
	std::string dataStr = writer.write(root);

	CCUserDefault::getInstance()->setStringForKey("USER_MAGNET_LIMIT_TIME_IDX", dataStr);
	setSaveRevision(getSaveRevision() + 1);

	if (CGoogleCloudManager::Instance()->getIsConnected())
		CSDKUtil::Instance()->GoogleCloudSave("USER_MAGNET_LIMIT_TIME_IDX", dataStr);
}

void CUserDataManager::dataSave_UserMagnetItemLimitRadius()
{
	JSONWRITER_CREATE
		root["data"] = m_UserData->_magnetItemLimitRadiusLevel;
	std::string dataStr = writer.write(root);

	CCUserDefault::getInstance()->setStringForKey("USER_MAGNET_LIMIT_SIZE_IDX", dataStr);
	setSaveRevision(getSaveRevision() + 1);

	if (CGoogleCloudManager::Instance()->getIsConnected())
		CSDKUtil::Instance()->GoogleCloudSave("USER_MAGNET_LIMIT_SIZE_IDX", dataStr);
}

void CUserDataManager::dataSave_UserBestTotalScore()
{
	JSONWRITER_CREATE
		root["data"] = m_UserData->_bestTotalScore;
	std::string dataStr = writer.write(root);

	CCUserDefault::getInstance()->setStringForKey("USER_BEST_TOTAL_SCORE", dataStr);
	setSaveRevision(getSaveRevision() + 1);

	if (CGoogleCloudManager::Instance()->getIsConnected())
		CSDKUtil::Instance()->GoogleCloudSave("USER_BEST_TOTAL_SCORE", dataStr);
}

void CUserDataManager::dataSave_UserBestCombo()
{
	JSONWRITER_CREATE
		root["data"] = m_UserData->_bestCombo;
	std::string dataStr = writer.write(root);

	CCUserDefault::getInstance()->setStringForKey("USER_BEST_COMBO", dataStr);
	setSaveRevision(getSaveRevision() + 1);

	if (CGoogleCloudManager::Instance()->getIsConnected())
		CSDKUtil::Instance()->GoogleCloudSave("USER_BEST_COMBO", dataStr);
}

void CUserDataManager::dataSave_UserCurrentCharacter()
{
	JSONWRITER_CREATE
		root["data"] = m_UserData->_currentCharacter;
	std::string dataStr = writer.write(root);

	CCUserDefault::getInstance()->setStringForKey("USER_CUR_CHARACTER", dataStr);
	setSaveRevision(getSaveRevision() + 1);

	if (CGoogleCloudManager::Instance()->getIsConnected())
		CSDKUtil::Instance()->GoogleCloudSave("USER_CUR_CHARACTER", dataStr);
}

void CUserDataManager::dataSave_UserCurrentPet()
{
	JSONWRITER_CREATE
		root["data"] = m_UserData->_currentPet;
	std::string dataStr = writer.write(root);

	CCUserDefault::getInstance()->setStringForKey("USER_CUR_PET", dataStr);
	setSaveRevision(getSaveRevision() + 1);

	if (CGoogleCloudManager::Instance()->getIsConnected())
		CSDKUtil::Instance()->GoogleCloudSave("USER_CUR_PET", dataStr);
}

void CUserDataManager::dataSave_UserCurrentRocket()
{
	JSONWRITER_CREATE
		root["data"] = m_UserData->_currentRocket;
	std::string dataStr = writer.write(root);

	CCUserDefault::getInstance()->setStringForKey("USER_CUR_ROCKET", dataStr);
	setSaveRevision(getSaveRevision() + 1);

	if (CGoogleCloudManager::Instance()->getIsConnected())
		CSDKUtil::Instance()->GoogleCloudSave("USER_CUR_ROCKET", dataStr);
}

void CUserDataManager::dataSave_UserCurrentChallenge()
{
	JSONWRITER_CREATE
		root["data"] = m_UserData->_currentChallenge;
	std::string dataStr = writer.write(root);

	CCUserDefault::getInstance()->setStringForKey("USER_CUR_CHALLENGE", dataStr);
	setSaveRevision(getSaveRevision() + 1);

	if (CGoogleCloudManager::Instance()->getIsConnected())
		CSDKUtil::Instance()->GoogleCloudSave("USER_CUR_CHALLENGE", dataStr);
}

void CUserDataManager::dataSave_UserCharacterList()
{
}

void CUserDataManager::dataSave_UserPetList()
{
}

void CUserDataManager::dataSave_UserRocketList()
{
}



unsigned CUserDataManager::getLevel()
{
    return m_UserData->_level;
}

void CUserDataManager::setLevel(unsigned value)
{
    m_UserData->_level = value;
    UserDefault::getInstance()->setIntegerForKey("USER_LEVEL", value);
}

bool CUserDataManager::CoinUpdate(int value)
{
	bool result = false;
	if (value > 0)
	{
		result = true;
		setCoin(getCoin() + value);
	}
	else
	{
		result = false;
		if ((value * -1) > getCoin())
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
			setCoin(getCoin() + value);
		}
	}
	return result;
}


unsigned CUserDataManager::getSaveRevision()
{
	return m_UserData->_saveRevision;
}

void CUserDataManager::setSaveRevision(unsigned value)
{
	m_UserData->_saveRevision = value;
	dataSave_UserDataSaveRevision();
}

unsigned CUserDataManager::getCoin()
{
    return m_UserData->_coin;
}

void CUserDataManager::setCoin(unsigned value)
{
    m_UserData->_coin = value;
	dataSave_UserCoin();
}

unsigned CUserDataManager::getHealthLevel()
{
    return m_UserData->_healthLevel;
}

void CUserDataManager::setHealthLevel(unsigned value)
{
    m_UserData->_healthLevel = value;
	dataSave_UserHealth();
}

unsigned CUserDataManager::getStarItemLimitTimeLevel()
{
    return m_UserData->_starItemLimitTimeLevel;
}

void CUserDataManager::setStarItemLimitTimeLevel(unsigned value)
{
    m_UserData->_starItemLimitTimeLevel = value;
	dataSave_UserStarItemLimitTime();
}

unsigned CUserDataManager::getCoinItemLimitTimeLevel()
{
    return m_UserData->_coinItemLimitTimeLevel;
}

void CUserDataManager::setCoinItemLimitTimeLevel(unsigned value)
{
    m_UserData->_coinItemLimitTimeLevel = value;
	dataSave_UserCoin();
}

unsigned CUserDataManager::getBonusItemLimitTimeLevel()
{
    return m_UserData->_bonusItemLimitTimeLevel;
}

void CUserDataManager::setBonusItemLimitTimeLevel(unsigned value)
{
    m_UserData->_bonusItemLimitTimeLevel = value;
	dataSave_UserBonusItemLimitTime();
}

unsigned CUserDataManager::getGiantItemLimitTimeLevel()
{
	return m_UserData->_giantItemLimitTimeLevel;
}

void CUserDataManager::setGiantItemLimitTimeLevel(unsigned value)
{
	m_UserData->_giantItemLimitTimeLevel = value;
	dataSave_UserGiantItemLimitTime();
}

unsigned CUserDataManager::getMagnetItemLimitTimeLevel()
{
    return m_UserData->_magnetItemLimitTimeLevel;
}

void CUserDataManager::setMagnetItemLimitTimeLevel(unsigned value)
{
    m_UserData->_magnetItemLimitTimeLevel = value;
	dataSave_UserMagnetItemLimitTime();
}

unsigned CUserDataManager::getMagnetItemLimitRadiusLevel()
{
    return m_UserData->_magnetItemLimitRadiusLevel;
}

void CUserDataManager::setMagnetItemLimitRadiusLevel(unsigned value)
{
	m_UserData->_magnetItemLimitRadiusLevel = value;
	dataSave_UserMagnetItemLimitRadius();
}


unsigned CUserDataManager::getBestTotalScore()
{
	return m_UserData->_bestTotalScore;
}

void CUserDataManager::setBestTotalScore(unsigned value)
{
	m_UserData->_bestTotalScore = value;
	dataSave_UserBestTotalScore();
}

unsigned CUserDataManager::getBestCombo()
{
	return m_UserData->_bestCombo;
}

void CUserDataManager::setBestCombo(unsigned value)
{
	m_UserData->_bestCombo = value;
	dataSave_UserBestCombo();
}

unsigned CUserDataManager::getCurrentCharacter()
{
	return m_UserData->_currentCharacter;
}

void CUserDataManager::setCurrentCharacter(unsigned value)
{
	m_UserData->_currentCharacter = value;
	dataSave_UserCurrentCharacter();
}

unsigned CUserDataManager::getCurrentPet()
{
	return m_UserData->_currentPet;
}

void CUserDataManager::setCurrentPet(unsigned value)
{
	m_UserData->_currentPet = value;
	dataSave_UserCurrentPet();
}

unsigned CUserDataManager::getCurrentRocket()
{
	return m_UserData->_currentRocket;
}

void CUserDataManager::setCurrentRocket(unsigned value)
{
	m_UserData->_currentRocket = value;
	dataSave_UserCurrentRocket();
}

unsigned CUserDataManager::getCurrentChallenge()
{
	return m_UserData->_currentChallenge;
}

void CUserDataManager::setCurrentChallenge(unsigned value)
{
	m_UserData->_currentChallenge = value;
	dataSave_UserCurrentChallenge();
}

bool CUserDataManager::isCharacterHave(int characterIdx)
{
	if (m_UserData->_characterList.size() > 0)	
		return m_UserData->_characterList.at(characterIdx);
	return false;
}

void CUserDataManager::haveCharacter(int characterIdx)
{
	m_UserData->_characterList.at(characterIdx) = true;
}

float CUserDataManager::getStarItemLimitTime()
{
	sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByName("StarTime");
	float limitTime = item._valuePerLevel * m_UserData->_starItemLimitTimeLevel;
	return limitTime;
}

float CUserDataManager::getCoinItemLimitTime()
{
	sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByName("CoinTime");
	float limitTime = item._valuePerLevel * m_UserData->_coinItemLimitTimeLevel;
	return limitTime;
}

float CUserDataManager::getGiantItemLimitTime()
{
	sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByName("GiantTime");
	float limitTime = item._valuePerLevel * m_UserData->_giantItemLimitTimeLevel;
	return limitTime;
}

float CUserDataManager::getBonusItemLimitTime()
{
	sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByName("BonusTime");
	float limitTime = item._valuePerLevel * m_UserData->_bonusItemLimitTimeLevel;
	return limitTime;
}

float CUserDataManager::getMagnetItemLimitTime()
{
	sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByName("MagnetTime");
	float limitTime = item._valuePerLevel * m_UserData->_magnetItemLimitTimeLevel;
	return limitTime;
}

float CUserDataManager::getMagnetItemLimitRadius()
{
	sWORKSHOPITEM_PARAM item = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByName("MagnetSize");
	float limitTime = item._valuePerLevel * m_UserData->_magnetItemLimitRadiusLevel;
	return limitTime;
}

void CUserDataManager::addKey(std::string key)
{
	m_UserData->_userDataKeyList.emplace_back(key);
    
    Json::Value root;
    Json::Value keyString;
    
    for(auto data : m_UserData->_userDataKeyList)
    {
        keyString.append(data);
    }
    root["key"] = keyString;
    Json::StyledWriter writer;
    std::string outputConfig = writer.write( root );
    
    CCLOG("Save Cloud Key : %s Value : %s", "USER_DATA_KEY", outputConfig.c_str());
//    bool result = WriteToFile("test.json", outputConfig.c_str(), outputConfig.length());
}