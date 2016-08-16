#pragma once
#include "../Common/HSHUtility.h"
#include <vector>
#include <map>


struct sUSER_DATA{
    
    std::map<std::string, unsigned> _userDataUnsignedMap;
    std::map<std::string, std::vector<unsigned>*> _userDataListMap;
    std::map<std::string, std::string> _userDataKeyMap;

};

class CGoogleCloudManager;
class CUserDataManager
{
    friend CGoogleCloudManager;
    
public:
    static CUserDataManager* Instance();
    
    void GoogleLoginResult();
	
    void addKey(std::string keyKind, std::string key);
    
	bool CoinUpdate(int value);

    unsigned getUserData_Number(std::string key);
    void setUserData_Number(std::string key, unsigned value);
    
    std::vector<unsigned>* getUserData_List(std::string key);
    bool getUserData_IsItemHave(std::string key, unsigned itemIdx);
    void setUserData_ItemGet(std::string key, unsigned itemIdx);
    
    //getter & setter
	void setSaveRevision(unsigned value);
    std::map<std::string, std::string> getKeyList() { return m_UserData->_userDataKeyMap; }

	float getStarItemLimitTime();
	float getCoinItemLimitTime();
	float getGiantItemLimitTime();
	float getBonusItemLimitTime();
	float getMagnetItemLimitTime();
	float getMagnetItemLimitRadius();

private:
    void dataLoad();
    
    void afterCallFirstRevision();
    
    void dataLoadFromXML();
	void dataLoadFromGoogleCloud();

    void userDataSave_Number(std::string key);
    void userDataSave_List(std::string key);
	void userDataSave_Revision();
    
    void userDataLoad(std::string key, std::string valueJson);
    void userDataLoad_Number(std::string key, std::string valueJson);
    void userDataLoad_List(std::string key, std::string valueJson);

    CUserDataManager();
    virtual ~CUserDataManager();
    
private:
    std::shared_ptr<sUSER_DATA> m_UserData;
    bool m_IsFirstRevisionCall;
};