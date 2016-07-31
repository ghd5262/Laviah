#pragma once
#include "../Common/HSHUtility.h"
#include <vector>

struct sUSER_DATA{
	unsigned _saveRevision;
    unsigned _level;
    unsigned _coin;
    unsigned _bestTotalScore;
    unsigned _bestCombo;
    unsigned _currentCharacter;
    unsigned _currentPet;
    unsigned _currentRocket;
    unsigned _currentChallenge;
	unsigned _healthLevel;
	unsigned _starItemLimitTimeLevel;
	unsigned _coinItemLimitTimeLevel;
	unsigned _bonusItemLimitTimeLevel;
	unsigned _magnetItemLimitTimeLevel;
	unsigned _magnetItemLimitRadiusLevel;
	unsigned _giantItemLimitTimeLevel;
    std::vector<bool> _characterList;
    std::vector<bool> _petList;
    std::vector<bool> _rocketList;
    std::vector<bool> _completeChallengeList;
    std::vector<bool> _mtrlList;
    std::vector<std::string> _userDataKeyList;
};

class CUserDataManager
{
public:
    static CUserDataManager* Instance();
	void DataLoad();
	
	bool CoinUpdate(int value);

	void DataLoad_UserDataSaveRevision(std::string value);
	void DataLoad_UserLevel(std::string value);
	void DataLoad_UserCoin(std::string value);
	void DataLoad_UserHealth(std::string value);
	void DataLoad_UserStarItemLimitTime(std::string value);
	void DataLoad_UserCoinItemLimitTime(std::string value);
	void DataLoad_UserGiantItemLimitTime(std::string value);
	void DataLoad_UserBonusItemLimitTime(std::string value);
	void DataLoad_UserMagnetItemLimitTime(std::string value);
	void DataLoad_UserMagnetItemLimitRadius(std::string value);
	void DataLoad_UserBestTotalScore(std::string value);
	void DataLoad_UserBestCombo(std::string value);
	void DataLoad_UserCurrentCharacter(std::string value);
	void DataLoad_UserCurrentPet(std::string value);
	void DataLoad_UserCurrentRocket(std::string value);
	void DataLoad_UserCurrentChallenge(std::string value);
	void DataLoad_UserCharacterList(std::string value);
	void DataLoad_UserPetList(std::string value);
	void DataLoad_UserRocketList(std::string value);

    //getter & setter
	unsigned getSaveRevision();
	void setSaveRevision(unsigned value);

    unsigned getLevel();
    void setLevel(unsigned value);
    
    unsigned getCoin();
    void setCoin(unsigned value);
    
    unsigned getHealthLevel();
    void setHealthLevel(unsigned value);
    
	unsigned getStarItemLimitTimeLevel();
	void setStarItemLimitTimeLevel(unsigned value);
    
	unsigned getCoinItemLimitTimeLevel();
    void setCoinItemLimitTimeLevel(unsigned value);
    
	unsigned getBonusItemLimitTimeLevel();
	void setBonusItemLimitTimeLevel(unsigned value);
    
	unsigned getGiantItemLimitTimeLevel();
	void setGiantItemLimitTimeLevel(unsigned value);

	unsigned getMagnetItemLimitTimeLevel();
	void setMagnetItemLimitTimeLevel(unsigned value);
    
	unsigned getMagnetItemLimitRadiusLevel();
	void setMagnetItemLimitRadiusLevel(unsigned value);

	unsigned getBestTotalScore();
	void setBestTotalScore(unsigned value);

	unsigned getBestCombo();
	void setBestCombo(unsigned value);

	unsigned getCurrentCharacter();
	void setCurrentCharacter(unsigned value);

	unsigned getCurrentPet();
	void setCurrentPet(unsigned value);

	unsigned getCurrentRocket();
	void setCurrentRocket(unsigned value);

	unsigned getCurrentChallenge();
	void setCurrentChallenge(unsigned value);

    void addKey(std::string key);
    std::vector<std::string> getKeyList() { return m_UserData->_userDataKeyList; }
    
	std::vector<bool> getUserDataHaveCharacterList() { return m_UserData->_characterList; }
    
    
	bool isCharacterHave(int characterIdx);
	void haveCharacter(int characterIdx);

	float getStarItemLimitTime();
	float getCoinItemLimitTime();
	float getGiantItemLimitTime();
	float getBonusItemLimitTime();
	float getMagnetItemLimitTime();
	float getMagnetItemLimitRadius();

private:
	void dataLoadFromXML();
	void dataLoadFromGoogleCloud();

	void dataSave_UserDataSaveRevision();
	void dataSave_UserLevel();
	void dataSave_UserCoin();
	void dataSave_UserHealth();
	void dataSave_UserStarItemLimitTime();
	void dataSave_UserCoinItemLimitTime();
	void dataSave_UserGiantItemLimitTime();
	void dataSave_UserMagnetItemLimitTime();
	void dataSave_UserMagnetItemLimitRadius();
	void dataSave_UserBonusItemLimitTime();
	void dataSave_UserBestTotalScore();
	void dataSave_UserBestCombo();
	void dataSave_UserCurrentCharacter();
	void dataSave_UserCurrentPet();
	void dataSave_UserCurrentRocket();
	void dataSave_UserCurrentChallenge();
	void dataSave_UserCharacterList();
	void dataSave_UserPetList();
	void dataSave_UserRocketList();

    CUserDataManager();
    virtual ~CUserDataManager();
    
private:
    std::shared_ptr<sUSER_DATA> m_UserData;
};