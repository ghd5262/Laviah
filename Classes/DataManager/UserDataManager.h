#pragma once
#include "../Common/HSHUtility.h"
#include <vector>
#include <map>

typedef std::map<std::string, int>               SINGLE_DATA_LIST;

typedef std::vector<int>                         ARRAY_DATA;
typedef std::map<std::string, ARRAY_DATA*>       ARRAY_DATA_LIST;

typedef std::vector<int>                         PARAM_DATA;
typedef std::map<int, PARAM_DATA*>               PARAM_DATA_ARRAY;
typedef std::map<std::string, PARAM_DATA_ARRAY*> PARAM_DATA_ARRAY_LIST;

typedef std::map<std::string, std::string>       USERDATA_KEY_LIST;

struct sUSER_DATA{
	SINGLE_DATA_LIST      _userDataIntMap;      ///< { key, 0 }
	ARRAY_DATA_LIST       _userDataListMap;     ///< { key, {0, 1, 2, 3} }
    PARAM_DATA_ARRAY_LIST _userDataParamListMap; ///< { key, {0, 0}, {1, 1}, {2, 2} }
};

namespace USERDATA_KEY {
    const std::string FREE_REWARD_LEVEL         = "USER_DATA_FREE_REWARD_LEVEL";
    const std::string FREE_REWARD_TIMESTAMP     = "USER_DATA_FREE_REWARD_TIMESTAMP";
    const std::string LAST_TIMESTAMP            = "USER_DATA_LAST_TIMESTAMP";
	const std::string LAST_SAVED_TIME		    = "USER_DATA_LAST_SAVED_TIME";
    const std::string FIRST_LOAD			    = "USER_DATA_FIRSTLOAD";
	const std::string DATA_REVISION				= "USER_DATA_SAVE_REVISION";
    const std::string DATA_SAVE_AUTO            = "USER_DATA_SAVE_AUTO";

    const std::string LEVEL                     = "USER_LEVEL";
    const std::string EXP                       = "USER_EXP";
    const std::string RANK                      = "USER_RANK";
    const std::string CHARACTER                 = "USER_CUR_CHARACTER";
    const std::string COIN                      = "USER_COIN";

	const std::string SAVED_RUN					= "USER_SAVED_RUN";
	const std::string SAVED_SCORE				= "USER_SAVED_SCORE";
	const std::string SAVED_REVIVE				= "USER_SAVED_REVIVE";

    const std::string PET                       = "USER_CUR_PET";
    const std::string ROCKET                    = "USER_CUR_ROCKET";
    
    const std::string BEST_COMBO                = "USER_BEST_COMBO";
    const std::string BEST_SCORE                = "USER_BEST_TOTAL_SCORE";

    const std::string SELECT_ITEM               = "USER_CUR_SELECT_ITEM";
    const std::string LAST_COM_ACHIEVEMENT      = "USER_LAST_COM_ACHIEVEMENT";
    
    const std::string BGM_VOLUME                = "USER_BGM_VOLUME";
    const std::string EFFECT_VOLUME             = "USER_EFFECT_VOLUME";
    
    const std::string CURRENT_LANGUAGE          = "USER_CURRENT_LANGUAGE";
    
    const std::string CHARACTER_LIST            = "USER_CHARACTER_LIST";
    const std::string ROCKET_LIST               = "USER_ROCKET_LIST";
    const std::string PET_LIST                  = "USER_PET_LIST";

    const std::string NORMAL_CLEAR_COUNT        = "USER_NORMAL_ACHIEVEMENT_COUNT";
    const std::string HIDDEN_CLEAR_COUNT        = "USER_HIDDEN_ACHIEVEMENT_COUNT";
    
    const std::string ITEM_LEVEL                = "USER_ITEM_LEVEL";
    const std::string NORMAL_ACHIEVEMENT_LIST   = "USER_NORMAL_ACHIEVEMENT_LIST";
    const std::string HIDDEN_ACHIEVEMENT_LIST   = "USER_HIDDEN_ACHIEVEMENT_LIST";
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
    long long getLastTimestamp();
    long long getFreeRewardTimestamp();
    int getUserData_Number(std::string key);
    ARRAY_DATA getUserData_List(std::string key);
    PARAM_DATA_ARRAY getUserData_ParamList(std::string key);
    int getUserData_ParamData(std::string key, int index, int paramIdx, int defaultValue);
    bool getUserData_IsItemHave(std::string key, int itemIdx);
    bool getUserData_IsItemExistWithParam(std::string key, int paramIdx, int value);
    
	void SaveUserData(bool saveToCloud = false, bool forceSave = false);
    void SaveUserDataAutomatically();
    void setSaveRevision(int value);
    void setUserData_CloudSaved(std::string cryptoValue);
    void setUserData_Number(std::string key, int value);
    void setUserData_NumberAdd(std::string key, int value);
    void setUserData_ItemGet(std::string key, int itemIdx);
    void setUserData_ItemParam(std::string key, int itemIdx, int paramIdx, int value);
	void setUserData_ItemRemove(std::string key, int itemIdx);
    void setUserData_ItemRemoveAll(std::string key);
    void setUserData_Reset();
    void setLastSavedTime(long long unixTime);
    void setLastTimestamp(long long timestamp);
    void setFreeRewardTimestamp(long long timestamp);
    bool CoinUpdate(int value);
    void ExpAdd(int exp);
    
    static int getUserDataSequenceFromList(std::string key, int itemIndex);
    
private:
	void initUserDefaultValue(sUSER_DATA &data);

	void dataLoadFromXML();

	void dataLoadFromCloud();

	void convertJsonToUserData(sUSER_DATA &data, std::string valueJson);

	void convertUserDataToJson(sUSER_DATA &data, std::string &valueJson);

	void overwriteXmlByCloud(std::string valueJson);

	bool isCloudRevisionHigher();

	void saveUserDataToCloud(std::string key, std::string data, bool forceSave = false);

    ARRAY_DATA* getUserData_ListRef(std::string key);
    
    PARAM_DATA_ARRAY* getUserData_ParamListRef(std::string key);

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

	std::string m_JsonUserDataFromXML;
	std::string m_JsonUserDataFromCloud;
	
	// 데이터 한번에 저장 및 로드를 위해 주석처리 - 2016. 9. 3
    //bool m_IsFirstRevisionCall;
};
