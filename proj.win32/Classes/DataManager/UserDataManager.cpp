#include "UserDataManager.h"



CUserDataManager::CUserDataManager()
{
    m_UserData = std::shared_ptr<sUSER_DATA>(new sUSER_DATA(), [](sUSER_DATA* userData)
                                             {
                                                 delete userData;
                                             });
    
    m_UserData->_level          = UserDefault::getInstance()->getIntegerForKey(         "USER_LEVEL",                   0);
    m_UserData->_gold           = UserDefault::getInstance()->getIntegerForKey(         "USER_GOLD",                    0);
    m_UserData->_bestTotalScore = UserDefault::getInstance()->getIntegerForKey(         "USER_BEST_TOTAL_SCORE",        0);
    m_UserData->_bestCombo      = UserDefault::getInstance()->getIntegerForKey(         "USER_BEST_COMBO",              0);
    m_UserData->_currentCharacter = UserDefault::getInstance()->getIntegerForKey(       "USER_CUR_CHARACTER",           0);
    m_UserData->_currentPet       = UserDefault::getInstance()->getIntegerForKey(       "USER_CUR_PET",                 0);
    m_UserData->_currentRocket    = UserDefault::getInstance()->getIntegerForKey(       "USER_CUR_ROCKET",              0);
    m_UserData->_currentChallenge = UserDefault::getInstance()->getIntegerForKey(       "USER_CUR_CHALLENGE",           0);
    m_UserData->_myHealth       = UserDefault::getInstance()->getFloatForKey(           "USER_MY_HEALTH",               1);
    m_UserData->_myStarItemTime = UserDefault::getInstance()->getFloatForKey(           "USER_MY_STAR",                 5);
    m_UserData->_myCoinItemTime = UserDefault::getInstance()->getFloatForKey(           "USER_MY_COIN",                 5);
    m_UserData->_myBonusItemTime = UserDefault::getInstance()->getFloatForKey(          "USER_MY_BONUS",                10);
    m_UserData->_myMagnetItemTime = UserDefault::getInstance()->getFloatForKey(         "USER_MY_MAGNET",               5);
    m_UserData->_myMagnetItemLimitRadius = UserDefault::getInstance()->getFloatForKey(  "USER_MY_MAGNET_LIMIT_RADIUS",  80);
    m_UserData->_myGiantItemTime = UserDefault::getInstance()->getFloatForKey(          "USER_MY_GIANT",                5);
}

CUserDataManager::~CUserDataManager()
{
}

CUserDataManager* CUserDataManager::Instance()
{
    static CUserDataManager instance;
    return &instance;
}

unsigned CUserDataManager::getUserDataLevel()
{
    return m_UserData->_level;
}

void CUserDataManager::setUserDataLevel(unsigned value)
{
    m_UserData->_level = value;
    UserDefault::getInstance()->setIntegerForKey("USER_LEVEL", value);
}

unsigned CUserDataManager::getUserDataGold()
{
    return m_UserData->_gold;
}

void CUserDataManager::setUserDataGold(unsigned value)
{
    m_UserData->_gold = value;
    UserDefault::getInstance()->setIntegerForKey("USER_GOLD", value);
}

unsigned CUserDataManager::getUserDataBestTotalScore()
{
    return m_UserData->_bestTotalScore;
}

void CUserDataManager::setUserDataBestTotalScore(unsigned value)
{
    m_UserData->_bestTotalScore = value;
    UserDefault::getInstance()->setIntegerForKey("USER_BEST_TOTAL_SCORE", value);
}

unsigned CUserDataManager::getUserDataBestCombo()
{
    return m_UserData->_bestCombo;
}

void CUserDataManager::setUserDataBestCombo(unsigned value)
{
    m_UserData->_bestCombo = value;
    UserDefault::getInstance()->setIntegerForKey("USER_BEST_COMBO", value);
}

unsigned CUserDataManager::getUserDataCurrentCharacter()
{
    return m_UserData->_currentCharacter;
}

void CUserDataManager::setUserDataCurrentCharacter(unsigned value)
{
    m_UserData->_bestCombo = value;
    UserDefault::getInstance()->setIntegerForKey("USER_CUR_CHARACTER", value);
}

unsigned CUserDataManager::getUserDataCurrentPet()
{
    return m_UserData->_currentPet;
}

void CUserDataManager::setUserDataCurrentPet(unsigned value)
{
    m_UserData->_bestCombo = value;
    UserDefault::getInstance()->setIntegerForKey("USER_CUR_PET", value);
}

unsigned CUserDataManager::getUserDataCurrentRocket()
{
    return m_UserData->_currentRocket;
}

void CUserDataManager::setUserDataCurrentRocket(unsigned value)
{
    m_UserData->_bestCombo = value;
    UserDefault::getInstance()->setIntegerForKey("USER_CUR_ROCKET", value);
}

unsigned CUserDataManager::getUserDataCurrentChallenge()
{
    return m_UserData->_currentChallenge;
}

void CUserDataManager::setUserDataCurrentChallenge(unsigned value)
{
    m_UserData->_bestCombo = value;
    UserDefault::getInstance()->setIntegerForKey("USER_CUR_CHALLENGE", value);
}

float CUserDataManager::getUserDataMyHealth()
{
    return m_UserData->_myHealth;
}

void CUserDataManager::setUserDataMyHealth(float value)
{
    m_UserData->_myHealth = value;
    UserDefault::getInstance()->setFloatForKey("USER_MY_HEALTH", value);
}

float CUserDataManager::getUserDataMyStar()
{
    return m_UserData->_myStarItemTime;
}

void CUserDataManager::setUserDataMyStar(float value)
{
    m_UserData->_myStarItemTime = value;
    UserDefault::getInstance()->setFloatForKey("USER_MY_STAR", value);
}

float CUserDataManager::getUserDataMyCoin()
{
    return m_UserData->_myCoinItemTime;
}

void CUserDataManager::setUserDataMyCoin(float value)
{
    m_UserData->_myCoinItemTime = value;
    UserDefault::getInstance()->setFloatForKey("USER_MY_COIN", value);
}

float CUserDataManager::getUserDataMyBonus()
{
    return m_UserData->_myBonusItemTime;
}

void CUserDataManager::setUserDataMyBonus(float value)
{
    m_UserData->_myBonusItemTime = value;
    UserDefault::getInstance()->setFloatForKey("USER_MY_BONUS", value);
}

float CUserDataManager::getUserDataMyMagnet()
{
    return m_UserData->_myMagnetItemTime;
}

void CUserDataManager::setUserDataMyMagnet(float value)
{
    m_UserData->_myMagnetItemTime = value;
    UserDefault::getInstance()->setFloatForKey("USER_MY_MAGNET", value);
}

float CUserDataManager::getUserDataMyMagnetLimitRadius()
{
    return m_UserData->_myMagnetItemLimitRadius;
}

void CUserDataManager::setUserDataMyMagnetLimitRadius(float value)
{
	m_UserData->_myMagnetItemLimitRadius = value;
    UserDefault::getInstance()->setFloatForKey("USER_MY_MAGNET_LIMIT_RADIUS", value);
}

float CUserDataManager::getUserDataMyGiant()
{
    return m_UserData->_myGiantItemTime;
}

void CUserDataManager::setUserDataMyGiant(float value)
{
    m_UserData->_myGiantItemTime = value;
    UserDefault::getInstance()->setFloatForKey("USER_MY_GIANT", value);
}
