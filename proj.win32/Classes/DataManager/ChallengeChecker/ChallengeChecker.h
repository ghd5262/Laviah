#pragma once
#include "../../Common/HSHUtility.h"
#include "../UserDataManager.h"

class CChallengeClearChecker{
public:
    virtual bool Check();
    
    CChallengeClearChecker(){};
    virtual ~CChallengeClearChecker(){};
    
    CC_SYNTHESIZE(int, m_MtrlValue, MtrlValue);
};

class coinScoreCheck : public CChallengeClearChecker
{
    virtual bool Check() override {
        if(m_MtrlValue >= GLOBAL->COINSCORE) return true;
        return false;
    };
};

class starScoreCheck : public CChallengeClearChecker
{
    virtual bool Check() override {
        if(m_MtrlValue >= GLOBAL->STARSCORE) return true;
        return false;
    };
};

class runScoreCheck : public CChallengeClearChecker
{
    virtual bool Check() override {
        if(m_MtrlValue >= GLOBAL->RUNSCORE) return true;
        return false;
    };
};

class bestScoreCheck : public CChallengeClearChecker
{
    virtual bool Check() override {
        auto bestScore = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BEST_SCORE);
        
        if(m_MtrlValue >= bestScore) return true;
        return false;
    };
};

class bestComboCheck : public CChallengeClearChecker
{
    virtual bool Check() override {
        auto bestCombo = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BEST_COMBO);

        if(m_MtrlValue >= bestCombo) return true;
        return false;
    };
};

class characterCollectCheck : public CChallengeClearChecker
{
    virtual bool Check() override {
        auto result = CUserDataManager::Instance()->getUserData_IsItemHave(USERDATA_KEY::CHARACTER_LIST, m_MtrlValue);
        
        return result;
    };
};

class rocketCollectCheck : public CChallengeClearChecker
{
    virtual bool Check() override {
        auto result = CUserDataManager::Instance()->getUserData_IsItemHave(USERDATA_KEY::ROCKET_LIST, m_MtrlValue);
        
        return result;
    };
};

class characterCountCheck : public CChallengeClearChecker
{
    virtual bool Check() override {
        auto count = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHARACTER_LIST)->size();
        
        if(m_MtrlValue >= count) return true;
        return false;
    };
};

class rocketCountCheck : public CChallengeClearChecker
{
    virtual bool Check() override {
        auto count = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::ROCKET_LIST)->size();
        
        if(m_MtrlValue >= count) return true;
        return false;
    };
};

class userLevelCheck : public CChallengeClearChecker
{
    virtual bool Check() override {
        auto level = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL);
        
        if(m_MtrlValue >= level) return true;
        return false;
    };
};

class workshopLevelCheck : public CChallengeClearChecker // TODO: 수정필요
{
    virtual bool Check() override {
        if(m_MtrlValue >= GLOBAL->STARSCORE) return true;
        return false;
    };
};

class comboCheck : public CChallengeClearChecker
{
    virtual bool Check() override {
        if(m_MtrlValue >= GLOBAL->COMBO) return true;
        return false;
    };
};

class coinCheck : public CChallengeClearChecker
{
    virtual bool Check() override {
        auto coin = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::COIN);
        
        if(m_MtrlValue >= coin) return true;
        return false;
    };
};

class itemUseCheck : public CChallengeClearChecker // TODO: 수정필요
{
    virtual bool Check() override {
        auto coin = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::COIN);
        
        if(m_MtrlValue >= coin) return true;
        return false;
    };
};