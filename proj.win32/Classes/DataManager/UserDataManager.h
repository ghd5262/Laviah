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
    
	// 데이터 한번에 저장 및 로드를 위해 주석처리 - 2016. 9. 3
    //void GoogleLoginResult();

    //getter & setter
    unsigned getUserData_Number(std::string key);
    std::vector<unsigned>* getUserData_List(std::string key);
    bool getUserData_IsItemHave(std::string key, unsigned itemIdx);
    float getItemCurrentValue(std::string key);
    std::map<std::string, std::string> getKeyList() { return m_UserData->_userDataKeyMap; }
    
    void setSaveRevision(unsigned value);
    void setUserData_Number(std::string key, unsigned value);
    void setUserData_ItemGet(std::string key, unsigned itemIdx);
    bool CoinUpdate(int value);
    //
    //CC_SYNTHESIZE(bool, m_IsDataLoadFinish, IsDataLoadFinish);

private:
	void dataLoad();

	void dataLoadFromXML();

	void dataLoadFromGoogleCloud();

	void convertJsonToUserData(std::string valueJson);

	void convertUserDataToJson();

	void googleCloudDataLoad(std::string cryptoValue);

	void overwriteXmlByGoogleCloud(std::string valueJson);

	bool isGoogleRevisionHigher();

	void addKey(std::string keyKind, std::string key);

	// 데이터 한번에 저장 및 로드를 위해 주석처리 - 2016. 9. 3
    /*void callbackFirstRevision();
    
    void dataLoad();
    void dataLoadFromXML();
	void dataLoadFromGoogleCloud();
    
    void convertJsonToUserData(std::string key, std::string valueJson);
    void convertJsonToUserData_Number(std::string key, std::string valueJson);
    void convertJsonToUserData_List(std::string key, std::string valueJson);

    void convertUserDataToJson_Number(std::string key);
    void convertUserDataToJson_List(std::string key);
    void convertUserDataToJson_Revision();
    
    void addKey(std::string keyKind, std::string key);
    
    void overwriteXmlByGoogleCloud(std::string key, std::string valueJson);*/
    
    CUserDataManager();
    virtual ~CUserDataManager();
    
private:
    std::shared_ptr<sUSER_DATA> m_UserData;
	std::string m_JsonUserDataFromXML;
	std::string m_JsonUserDataFromGoogleCloud;
	
	// 데이터 한번에 저장 및 로드를 위해 주석처리 - 2016. 9. 3
    //bool m_IsFirstRevisionCall;
};