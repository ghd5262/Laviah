#pragma once
#include "DataManagerUtils.h"
#include "UserDataManager.h"
#include "../Common/HSHUtility.h"
#include "../json/json.h"
#include <map>
#include <vector>
#include <algorithm>


enum CHECKER_TYPE{
	ETC = 0,
	GLOBAL_DATA = 1,
	SINGLE_DATA = 2,
	ITEM_EXIST = 3,
	ITEM_COUNT = 4,
	CONTINUE = 5
};

class CAchievementClearChecker;
class CAchievementRewarder;
struct sACHIEVEMENT_PARAM
{
    int _index;
	bool _hiddenType;
	bool _visible;
	bool _isHighLevel;
	CHECKER_TYPE _checkerType;
    std::string _title;
    std::string _contents;
    std::string _rewardKey;
    int _rewardValue;
	std::vector<std::string> _materialKeyList;
	std::vector<int> _materialValueList;

    sACHIEVEMENT_PARAM()
    : _index(-1)
	, _hiddenType(false)
	, _visible(false)
	, _isHighLevel(false)
	, _checkerType(CHECKER_TYPE::ETC)
    , _title("")
    , _contents("")
    , _rewardKey("")
    , _rewardValue(0){}
    
    sACHIEVEMENT_PARAM(const sACHIEVEMENT_PARAM& data)
    : _index(data._index)
	, _hiddenType(data._hiddenType)
	, _visible(data._visible)
	, _isHighLevel(data._isHighLevel)
	, _checkerType(data._checkerType)
    , _title(data._title)
    , _contents(data._contents)
    , _rewardKey(data._rewardKey)
    , _rewardValue(data._rewardValue)
    {
        DATA_MANAGER_UTILS::copyList(data._materialKeyList,   _materialKeyList);
        DATA_MANAGER_UTILS::copyList(data._materialValueList, _materialValueList);
	}
    
    sACHIEVEMENT_PARAM(const sACHIEVEMENT_PARAM* data)
    : _index(data->_index)
	, _hiddenType(data->_hiddenType)
	, _visible(data->_visible)
	, _isHighLevel(data->_isHighLevel)
	, _checkerType(data->_checkerType)
    , _title(data->_title)
    , _contents(data->_contents)
    , _rewardKey(data->_rewardKey)
    , _rewardValue(data->_rewardValue)
    {
        DATA_MANAGER_UTILS::copyList(data->_materialKeyList,   _materialKeyList);
        DATA_MANAGER_UTILS::copyList(data->_materialValueList, _materialValueList);
	}
};

namespace ACHIEVEMENT_DEFINE {
	static const int LIMIT_COUNT = 3;
	static const std::string NORMAL_CONTENT = "ACHIEVEMENT_NORMAL_CONTENT_%d";
    static const std::string HIDDEN_CONTENT = "ACHIEVEMENT_HIDDEN_CONTENT_%d";
	static const std::string HIDDEN_TITLE   = "ACHIEVEMENT_HIDDEN_TITLE_%d";
}

struct sREWARD_DATA{
	std::string _key;
	int _value;

	sREWARD_DATA()
		: _key("")
		, _value(0){};


	sREWARD_DATA(std::string key, int value)
		: _key(key)
		, _value(value){};
};

typedef std::function<bool(int)> CHECKER;
typedef std::map<std::string, CHECKER> CHECKER_LIST;
typedef std::function<sREWARD_DATA(sREWARD_DATA)> REWARDER;
typedef std::map<std::string, REWARDER> REWARDER_LIST;
typedef std::map<int, const sACHIEVEMENT_PARAM*> ACHIEVEMENT_LIST;
typedef std::function<bool(const sACHIEVEMENT_PARAM*)> ACHIEVEMENT_PICK;
typedef std::vector<int> NORMAL_ACHIEVEMENT_VALUE_LIST;
typedef std::map<std::string, NORMAL_ACHIEVEMENT_VALUE_LIST> VALUE_MAP;
class CAchievementDataManager
{
public:
    static CAchievementDataManager* Instance();
	bool CheckCompleteAll();
	bool CheckAchievementComplete(int index, bool isHidden);
	const sACHIEVEMENT_PARAM* CompleteCheckRealTime(bool isHidden);
    void ResetNormalAchievements();
    
	sREWARD_DATA Reward(int index);
	sREWARD_DATA RewardByKey(std::string key, int value);
    int NonCompleteAchievementExist();
    void getNewAchievements();
    const sACHIEVEMENT_PARAM* SkipAchievement(int index);
    
    //getter & setter
    const sACHIEVEMENT_PARAM* getNormalAchievementByIndex(int index) const;
    const sACHIEVEMENT_PARAM* getHiddenAchievementByIndex(int index) const;
    const sACHIEVEMENT_PARAM* getNewRandomAchievement();
    const sACHIEVEMENT_PARAM* getNonCompleteAchievementFromCurrentList();
    cocos2d::Sprite* getRewardSprite(std::string rewardKey, int rewardValue);
    ACHIEVEMENT_LIST getHiddenAchievementList() const {
        return m_HiddenAchievementDataList;
    }
    void UpdateCurHiddenAchievementList();

private:
    void initWithJson(std::string fileName);
	void initETCChekerList();
	void initRewarderList();
    
	const Json::Value initAchievementWithDefaultValue(bool hidden, std::string key, const Json::Value data);
	void addAchievementToList(ACHIEVEMENT_LIST &list, const Json::Value& data, bool hiddenType);
    void addValueListToMap(const Json::Value data);

    const sACHIEVEMENT_PARAM* getNewRandomAchievementFromList(ACHIEVEMENT_LIST &list);
    ACHIEVEMENT_LIST getNonCompletedAchievementList();
    void removeAchievementFromUserData(int index);
    
    void getCurAchievementListByType(ARRAY_DATA& list, bool isHidden);
    int getMaterialValueByLevel(std::string key, int level);

    CAchievementDataManager();
    virtual ~CAchievementDataManager();
    
private:
    ACHIEVEMENT_LIST m_NormalAchievementDataList;
	ACHIEVEMENT_LIST m_HiddenAchievementDataList;
    CHECKER_LIST m_CheckerList;
    REWARDER_LIST m_RewarderList;
    CAchievementClearChecker* m_Checker;
    CAchievementRewarder* m_Rewarder;
    VALUE_MAP m_ValueMap;
	Json::Value m_NormalAchievementDefaultSet;
	Json::Value m_HiddenAchievementDefaultSet;
};