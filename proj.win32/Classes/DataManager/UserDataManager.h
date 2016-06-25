#pragma once
#include "../Common/HSHUtility.h"
#include <vector>

struct sUSER_DATA{
    unsigned _level;
    unsigned _coin;
    unsigned _bestTotalScore;
    unsigned _bestCombo;
    unsigned _currentCharacter;
    unsigned _currentPet;
    unsigned _currentRocket;
    unsigned _currentChallenge;
    float _myHealth;
    float _myStarItemTime;
    float _myCoinItemTime;
    float _myBonusItemTime;
    float _myMagnetItemTime;
    float _myMagnetItemLimitRadius;
    float _myGiantItemTime;
    std::vector<int> _characterList;
    std::vector<int> _petList;
    std::vector<int> _rocketList;
    std::vector<int> _completeChallengeList;
    std::vector<int> _mtrlList;
};

class CUserDataManager
{
public:
    static CUserDataManager* Instance();
	bool CoinUpdate(int value);


    //getter & setter
    unsigned getUserDataLevel();
    void setUserDataLevel(unsigned value);
    
    unsigned getUserDataCoin();
    void setUserDataCoin(unsigned value);
    
    unsigned getUserDataBestTotalScore();
    void setUserDataBestTotalScore(unsigned value);
    
    unsigned getUserDataBestCombo();
    void setUserDataBestCombo(unsigned value);
    
    unsigned getUserDataCurrentCharacter();
    void setUserDataCurrentCharacter(unsigned value);
    
    unsigned getUserDataCurrentPet();
    void setUserDataCurrentPet(unsigned value);
    
    unsigned getUserDataCurrentRocket();
    void setUserDataCurrentRocket(unsigned value);
    
    unsigned getUserDataCurrentChallenge();
    void setUserDataCurrentChallenge(unsigned value);
    
    float getUserDataMyHealth();
    void setUserDataMyHealth(float value);
    
    float getUserDataMyStar();
    void setUserDataMyStar(float value);
    
    float getUserDataMyCoin();
    void setUserDataMyCoin(float value);
    
    float getUserDataMyBonus();
    void setUserDataMyBonus(float value);
    
    float getUserDataMyMagnet();
    void setUserDataMyMagnet(float value);
    
    float getUserDataMyMagnetLimitRadius();
    void setUserDataMyMagnetLimitRadius(float value);
    
    float getUserDataMyGiant();
    void setUserDataMyGiant(float value);
    
private:
    CUserDataManager();
    virtual ~CUserDataManager();
    
private:
    std::shared_ptr<sUSER_DATA> m_UserData;
};