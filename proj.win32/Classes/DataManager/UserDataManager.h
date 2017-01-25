#pragma once
#include "../Common/HSHUtility.h"
#include <vector>
#include <map>
typedef std::vector<int> ARRAY_DATA;
typedef std::map<std::string, int> SINGLE_DATA_LIST;
typedef std::map<std::string, ARRAY_DATA*> ARRAY_DATA_LIST;
typedef std::map<std::string, std::string> USERDATA_KEY_LIST;

struct sUSER_DATA{
	SINGLE_DATA_LIST  _userDataIntMap;
	ARRAY_DATA_LIST   _userDataListMap;
};

namespace USERDATA_KEY {
	const std::string LAST_SAVED_TIME		    = "USER_DATA_LAST_SAVED_TIME";
    const std::string FIRST_LOAD			    = "USER_DATA_FIRSTLOAD";
	const std::string DATA_REVISION				= "USER_DATA_SAVE_REVISION";

    const std::string LEVEL                     = "USER_LEVEL";
    const std::string CHARACTER                 = "USER_CUR_CHARACTER";
    const std::string COIN                      = "USER_COIN";
    const std::string PET                       = "USER_CUR_PET";
    const std::string ROCKET                    = "USER_CUR_ROCKET";
    
    const std::string BEST_COMBO                = "USER_BEST_COMBO";
    const std::string BEST_SCORE                = "USER_BEST_TOTAL_SCORE";
    
    const std::string ITEM_HEALTH               = "USER_HEALTH_IDX";
    const std::string ITEM_TIME_BOUNS           = "USER_BONUS_LIMIT_TIME_IDX";
    const std::string ITEM_TIME_COIN            = "USER_COIN_LIMIT_TIME_IDX";
    const std::string ITEM_TIME_STAR            = "USER_STAR_LIMIT_TIME_IDX";
    const std::string ITEM_TIME_GIANT           = "USER_GIANT_LIMIT_TIME_IDX";
    const std::string ITEM_TIME_MAGNET          = "USER_MAGNET_LIMIT_TIME_IDX";
    const std::string ITEM_SIZE_MAGNET          = "USER_MAGNET_LIMIT_SIZE_IDX";
    
    const std::string SELECT_ITEM               = "USER_CUR_SELECT_ITEM";
    const std::string SELECT_KEY                = "USER_CUR_SELECT_KEY";
    
    const std::string CHARACTER_LIST            = "USER_CHARACTER_LIST";
    const std::string ROCKET_LIST               = "USER_ROCKET_LIST";
    const std::string PET_LIST                  = "USER_PET_LIST";
    
    const std::string CHALLENGE_CUR_LIST        = "USER_CUR_CHALLENGE_LIST";
    const std::string CHALLENGE_CUR_VALUE_LIST  = "USER_CUR_CHALLENGE_VALUE_LIST";
    const std::string CHALLENGE_COM_LIST        = "USER_COMPLETE_CHALLENGE_LIST";
};

class CGoogleCloudManager;
class CUserDataManager
{
    friend CGoogleCloudManager;
    typedef std::function<int(int, int)> LIST_COMPARE;
public:
    static CUserDataManager* Instance();
    void UserDataLoad();

    //getter & setter
    bool getIsFirstPlay();
	tm* getLastSavedTime();
    int getUserData_Number(std::string key);
    ARRAY_DATA getUserData_List(std::string key);
    bool getUserData_IsItemHave(std::string key, int itemIdx);
    float getItemCurrentValue(std::string key);
	USERDATA_KEY_LIST getKeyList() { return m_UserDataKeyList; }
    
	void SaveUserData(bool saveToCloud = false, bool forceSave = false);
    void setSaveRevision(int value);
    void setUserData_Number(std::string key, int value);
    void setUserData_ItemGet(std::string key, int itemIdx);
	void setUserData_ItemRemove(std::string key, int itemIdx);
    void setUserData_ItemRemoveAll(std::string key);
    void setUserData_Reset();
    void setLastSavedTime(long long unixTime);
    bool CoinUpdate(int value);
    
    static int getUserDataSequenceFromList(std::string key, int itemIndex);
    
private:
	void initUserDefaultValue(sUSER_DATA &data);

	void initUserDataKey(sUSER_DATA &data);
    
	void initSingleUserDataWithDefaultValue(std::string key);

	void initArrayUserDataWithDefaultValue(std::string key);

	void dataLoadFromXML();

	void dataLoadFromGoogleCloud();

	void convertJsonToUserData(sUSER_DATA &data, std::string valueJson);

	void convertUserDataToJson(std::string &valueJson);

	void googleCloudDataLoad(std::string cryptoValue);

	void overwriteXmlByGoogleCloud(std::string valueJson);

	bool isGoogleRevisionHigher();

	void addKey(std::string keyKind, std::string key);

	void saveUserDataToGoogleCloud(std::string key, std::string data, bool forceSave = false);

    ARRAY_DATA* getUserData_ListRef(std::string key);

    static void sortUserDataList(std::string key, const LIST_COMPARE& compare);
	
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
    sUSER_DATA m_UserData;
	sUSER_DATA m_UserDefaultData;
	USERDATA_KEY_LIST m_UserDataKeyList;

	std::string m_JsonUserDataFromXML;
	std::string m_JsonUserDataFromGoogleCloud;
	
	// 데이터 한번에 저장 및 로드를 위해 주석처리 - 2016. 9. 3
    //bool m_IsFirstRevisionCall;
};