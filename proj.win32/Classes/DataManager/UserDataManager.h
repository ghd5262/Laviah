#pragma once
#include "../Common/HSHUtility.h"
#include <vector>
#include <map>

struct sUSER_DATA{
    std::map<std::string, unsigned> _userDataUnsignedMap;
    std::map<std::string, std::vector<unsigned>*> _userDataListMap;
	std::map<std::string, std::string> _userDataKeyMap;
};

namespace USERDATA_KEY {
    const std::string DATA_REVISION      = "USER_DATA_SAVE_REVISION";
    									 
    const std::string LEVEL              = "USER_LEVEL";
    const std::string CHARACTER          = "USER_CUR_CHARACTER";
    const std::string COIN               = "USER_COIN";
    const std::string PET                = "USER_CUR_PET";
    const std::string ROCKET             = "USER_CUR_ROCKET";
    									 
    const std::string BEST_COMBO         = "USER_BEST_COMBO";
    const std::string BEST_SCORE         = "USER_BEST_TOTAL_SCORE";
    									 
    const std::string ITEM_HEALTH        = "USER_HEALTH_IDX";
    const std::string ITEM_TIME_BOUNS    = "USER_BONUS_LIMIT_TIME_IDX";
    const std::string ITEM_TIME_COIN     = "USER_COIN_LIMIT_TIME_IDX";
    const std::string ITEM_TIME_STAR     = "USER_STAR_LIMIT_TIME_IDX";
    const std::string ITEM_TIME_GIANT    = "USER_GIANT_LIMIT_TIME_IDX";
    const std::string ITEM_TIME_MAGNET   = "USER_MAGNET_LIMIT_TIME_IDX";
    const std::string ITEM_SIZE_MAGNET   = "USER_MAGNET_LIMIT_SIZE_IDX";
    									 
    const std::string SELECT_ITEM        = "USER_CUR_SELECT_ITEM";
    const std::string SELECT_KEY         = "USER_CUR_SELECT_KEY";
    									 
    const std::string CHARACTER_LIST     = "USER_CHARACTER_LIST";
    const std::string ROCKET_LIST        = "USER_ROCKET_LIST";
    const std::string PET_LIST           = "USER_PET_LIST";

	const std::string CHALLENGE_CUR_LIST = "USER_CUR_CHALLENGE_LIST";
	const std::string CHALLENGE_COM_LIST = "USER_COMPLETE_CHALLENGE_LIST";
};

class CGoogleCloudManager;
class CUserDataManager
{
    friend CGoogleCloudManager;
    
public:
    static CUserDataManager* Instance();
    void UserDataLoad();

    //getter & setter
    void initUserDefaultValue();
    bool getIsFirstPlay();
    unsigned getUserData_Number(std::string key);
    std::vector<unsigned>* getUserData_List(std::string key);
    bool getUserData_IsItemHave(std::string key, unsigned itemIdx);
    float getItemCurrentValue(std::string key);
    std::map<std::string, std::string> getKeyList() { return m_UserData->_userDataKeyMap; }
    
    void setSaveRevision(unsigned value);
    void setUserData_Number(std::string key, unsigned value);
    void setUserData_ItemGet(std::string key, unsigned itemIdx);
	void setUserData_ItemRemove(std::string key, unsigned itemIdx);
    bool CoinUpdate(int value);
    
private:


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