#pragma once
#include "../Common/HSHUtility.h"
#include "UserDataDefine.h"
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

class CUserDataManager
{
    typedef std::function<int(int, int)> LIST_COMPARE;
public:
    static CUserDataManager* Instance();
    void Initialize();
    
    //getter & setter
    bool getIsFirstPlay();
    void setIsFirstPlay(bool isFirst);
    std::string getLastResourcesVersion();
    void setLastResourcesVersion(std::string version);
	tm* getLastSavedTime();
    long long getLastTimestamp();
    long long getFreeRewardTimestamp();
    
    float getItemValueByItemIndex(int itemIndex);
    float getItemValueBySkillIndex(int skillIndex);
    
    ARRAY_DATA getUserData_List(std::string key);
    PARAM_DATA_ARRAY getUserData_ParamList(std::string key);
    
    int getUserData_Number(std::string key);
    int getUserData_ParamData(std::string key, int index, int paramIdx, int defaultValue);
    bool getUserData_IsItemExist(std::string key, int itemIdx);
    bool getUserData_IsItemExistWithParam(std::string key, int paramIdx, int value);
    
	void SaveUserData(bool saveToCloud = false, bool autoSave = false);
    
    void LoadUserDataFromXML();
    void LoadUserDataFromCloud();
    
    void setSaveRevision(int value);
    
    void setUserData_Number(std::string key, int value);
    void setUserData_NumberAdd(std::string key, int value);
    void setUserData_ItemExist(std::string key, int itemIdx);
    void setUserData_ItemParam(std::string key, int itemIdx, int paramIdx, int value);
    void setUserData_Reset();
    
    void setLastSavedTime(long long unixTime);
    void setLastTimestamp(long long timestamp);
    void setFreeRewardTimestamp(long long timestamp);
    
    bool CoinUpdate(int value);
    void ExpAdd(int exp);
    
    static int getUserDataSequenceFromList(std::string key, int itemIndex);
    
    CC_SYNTHESIZE(bool, m_DataLoadFinished, DataLoadFinished);
private:
	void initUserDefaultValue(sUSER_DATA &data);

	void convertJsonToUserData(sUSER_DATA &data, std::string valueJson);

	void convertUserDataToJson(sUSER_DATA &data, std::string &valueJson);

	void overwriteXmlByCloud(std::string valueJson);

    ARRAY_DATA* getUserData_ListRef(std::string key);
    
    PARAM_DATA_ARRAY* getUserData_ParamListRef(std::string key);

    void userDataClean(sUSER_DATA &data);
    
    static void sortUserDataList(std::string key, const LIST_COMPARE& compare);
	
    CUserDataManager();
    virtual ~CUserDataManager();
    
private:
    sUSER_DATA m_UserData;

	std::string m_JsonUserDataFromXML;
	std::string m_JsonUserDataFromCloud;
};
