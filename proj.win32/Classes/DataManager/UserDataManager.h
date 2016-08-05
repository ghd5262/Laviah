#pragma once
#include "../Common/HSHUtility.h"
#include <vector>
#include <map>


struct sUSER_DATA{
    
    std::map<std::string, unsigned> _userDataUnsignedMap;
    std::map<std::string, std::vector<unsigned>*> _userDataListMap;
    std::map<std::string, std::string> _userDataKeyMap;
    
//	unsigned _saveRevision;
//    unsigned _level;
//    unsigned _coin;
//    unsigned _bestTotalScore;
//    unsigned _bestCombo;
//    unsigned _currentCharacter;
//    unsigned _currentPet;
//    unsigned _currentRocket;
//    unsigned _currentChallenge;
//	unsigned _healthLevel;
//	unsigned _starItemLimitTimeLevel;
//	unsigned _coinItemLimitTimeLevel;
//	unsigned _bonusItemLimitTimeLevel;
//	unsigned _magnetItemLimitTimeLevel;
//	unsigned _magnetItemLimitRadiusLevel;
//	unsigned _giantItemLimitTimeLevel;
//    std::vector<bool> _characterList;
//    std::vector<bool> _petList;
//    std::vector<bool> _rocketList;
//    std::vector<bool> _completeChallengeList;
//    std::vector<bool> _mtrlList;
    
};

class CGoogleCloudManager;
class CUserDataManager
{
    friend CGoogleCloudManager;
    
public:
    static CUserDataManager* Instance();
	void DataLoad();
	
    void addKey(std::string keyKind, std::string key);
    
	bool CoinUpdate(int value);

    unsigned getUserData_Number(std::string key);
    void setUserData_Number(std::string key, unsigned value);
    
    std::vector<unsigned>* getUserData_List(std::string key);
    bool getUserData_IsItemHave(std::string key, unsigned itemIdx);
    void setUserData_ItemGet(std::string key, unsigned itemIdx);
    
    //getter & setter
//	unsigned getSaveRevision();
	void setSaveRevision(unsigned value);
    std::map<std::string, std::string> getKeyList() { return m_UserData->_userDataKeyMap; }
//    unsigned getLevel();
//    void setLevel(unsigned value);
    
//    unsigned getCoin();
//    void setCoin(unsigned value);
    
//    unsigned getHealthLevel();
//    void setHealthLevel(unsigned value);
    
//	unsigned getStarItemLimitTimeLevel();
//	void setStarItemLimitTimeLevel(unsigned value);
    
//	unsigned getCoinItemLimitTimeLevel();
//    void setCoinItemLimitTimeLevel(unsigned value);
    
//	unsigned getBonusItemLimitTimeLevel();
//	void setBonusItemLimitTimeLevel(unsigned value);
    
//	unsigned getGiantItemLimitTimeLevel();
//	void setGiantItemLimitTimeLevel(unsigned value);

//	unsigned getMagnetItemLimitTimeLevel();
//	void setMagnetItemLimitTimeLevel(unsigned value);
    
//	unsigned getMagnetItemLimitRadiusLevel();
//	void setMagnetItemLimitRadiusLevel(unsigned value);

//	unsigned getBestTotalScore();
//	void setBestTotalScore(unsigned value);

//	unsigned getBestCombo();
//	void setBestCombo(unsigned value);

//	unsigned getCurrentCharacter();
//	void setCurrentCharacter(unsigned value);

//	unsigned getCurrentPet();
//	void setCurrentPet(unsigned value);

//	unsigned getCurrentRocket();
//	void setCurrentRocket(unsigned value);

//	unsigned getCurrentChallenge();
//	void setCurrentChallenge(unsigned value);

    
//    std::vector<std::string> getKeyList() { return m_UserData->_userDataKeyList; }
    
//	std::vector<bool> getUserDataHaveCharacterList() { return m_UserData->_characterList; }
    
    
//	bool isCharacterHave(int characterIdx);
//	void haveCharacter(int characterIdx);
//    bool isPetHave(int petIdx);
//    void havePet(int petIdx);
//    bool isRocketHave(int rocketIdx);
//    void haveRocket(int rocketIdx);

	float getStarItemLimitTime();
	float getCoinItemLimitTime();
	float getGiantItemLimitTime();
	float getBonusItemLimitTime();
	float getMagnetItemLimitTime();
	float getMagnetItemLimitRadius();

private:
	void dataLoadFromXML();
	void dataLoadFromGoogleCloud();

    void userDataSave_Number(std::string key);
    void userDataSave_List(std::string key);
	void userDataSave_Revision();
    
    void userDataLoad(std::string key, std::string valueJson);
    void userDataLoad_Number(std::string key, std::string valueJson);
    void userDataLoad_List(std::string key, std::string valueJson);
    
//	void dataSave_UserLevel();
//	void dataSave_UserCoin();
//	void dataSave_UserHealth();
//	void dataSave_UserStarItemLimitTime();
//	void dataSave_UserCoinItemLimitTime();
//	void dataSave_UserGiantItemLimitTime();
//	void dataSave_UserMagnetItemLimitTime();
//	void dataSave_UserMagnetItemLimitRadius();
//	void dataSave_UserBonusItemLimitTime();
//	void dataSave_UserBestTotalScore();
//	void dataSave_UserBestCombo();
//	void dataSave_UserCurrentCharacter();
//	void dataSave_UserCurrentPet();
//	void dataSave_UserCurrentRocket();
//	void dataSave_UserCurrentChallenge();
//	void dataSave_UserCharacterList();
//	void dataSave_UserPetList();
//	void dataSave_UserRocketList();
    

//    void dataLoad_UserDataSaveRevision(std::string value);
//    void dataLoad_UserLevel(std::string value);
//    void dataLoad_UserCoin(std::string value);
//    void dataLoad_UserHealth(std::string value);
//    void dataLoad_UserStarItemLimitTime(std::string value);
//    void dataLoad_UserCoinItemLimitTime(std::string value);
//    void dataLoad_UserGiantItemLimitTime(std::string value);
//    void dataLoad_UserBonusItemLimitTime(std::string value);
//    void dataLoad_UserMagnetItemLimitTime(std::string value);
//    void dataLoad_UserMagnetItemLimitRadius(std::string value);
//    void dataLoad_UserBestTotalScore(std::string value);
//    void dataLoad_UserBestCombo(std::string value);
//    void dataLoad_UserCurrentCharacter(std::string value);
//    void dataLoad_UserCurrentPet(std::string value);
//    void dataLoad_UserCurrentRocket(std::string value);
//    void dataLoad_UserCurrentChallenge(std::string value);
//    void dataLoad_UserCharacterList(std::string value);
//    void dataLoad_UserPetList(std::string value);
//    void dataLoad_UserRocketList(std::string value);


    CUserDataManager();
    virtual ~CUserDataManager();
    
private:
    std::shared_ptr<sUSER_DATA> m_UserData;
};