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
    ITEM_PARAM = 5,
	CONTINUE = 6,
};

enum USERDATA_PARAM_ACHIEVEMENT_NORMAL{
    NORMAL_INDEX = 0,
    STATE = 1, ///< 0: non-completed, 1: running, 2: finished, 3: completed
};

enum ACHIEVEMENT_NORMAL_STATE{
    NONE_COMPLETED = 0,
    RUNNING,
    FINISHED,
    COMPLETED,
};

enum USERDATA_PARAM_ACHIEVEMENT_HIDDEN{
    HIDDEN_INDEX = 0,
    LEVEL = 1,
};

class CAchievementClearChecker;
class CAchievementRewarder;

typedef std::vector<int> MATERIAL_VALUES;
struct ACHIEVEMENT_MATERIAL {
    std::string _materialKey;
    MATERIAL_VALUES _materialValues;
};

typedef std::vector<ACHIEVEMENT_MATERIAL> MATERIAL_LIST;
struct ACHIEVEMENT_LEVEL {
    std::string _title;
    std::string _contents;
    std::string _rewardKey;
    int _rewardValue;
    int _contentsValue;
    CHECKER_TYPE _checkerType;
    MATERIAL_LIST _materialList;
};

typedef std::vector<ACHIEVEMENT_LEVEL> ACHIEVEMENT_LEVEL_LIST;
struct ACHIEVEMENT {
    int _index;
    bool _hiddenType;
    bool _visibleType;
    ACHIEVEMENT_LEVEL_LIST _levelList;
};
//
//struct sACHIEVEMENT_PARAM
//{
//    
//    
//    
//    
//    
//    
//    
//    
//    
//    int _index;
//	bool _hiddenType;
//	bool _visible;
//    
//    CHECKER_TYPE _checkerType;
//    std::string _title;
//    std::string _contents;
//    std::string _rewardKey;
//    int _valueIndex;
//	std::vector<std::string> _materialKeyList;
//	std::vector<int> _materialValueList;
//    std::vector<int> _rewardValueList;
//
//    sACHIEVEMENT_PARAM()
//    : _index(-1)
//	, _hiddenType(false)
//	, _visible(false)
//	, _checkerType(CHECKER_TYPE::ETC)
//    , _title("")
//    , _contents("")
//    , _rewardKey("")
//    , _valueIndex(-1){}
//    
//    sACHIEVEMENT_PARAM(const sACHIEVEMENT_PARAM& data)
//    : _index(data._index)
//	, _hiddenType(data._hiddenType)
//	, _visible(data._visible)
//	, _checkerType(data._checkerType)
//    , _title(data._title)
//    , _contents(data._contents)
//    , _rewardKey(data._rewardKey)
//    , _valueIndex(data._valueIndex)
//    {
//        DATA_MANAGER_UTILS::copyList(data._materialKeyList,   _materialKeyList);
//        DATA_MANAGER_UTILS::copyList(data._materialValueList, _materialValueList);
//        DATA_MANAGER_UTILS::copyList(data._rewardValueList,   _rewardValueList);
//
//	}
//    
//    sACHIEVEMENT_PARAM(const sACHIEVEMENT_PARAM* data)
//    : _index(data->_index)
//	, _hiddenType(data->_hiddenType)
//	, _visible(data->_visible)
//	, _checkerType(data->_checkerType)
//    , _title(data->_title)
//    , _contents(data->_contents)
//    , _rewardKey(data->_rewardKey)
//    , _valueIndex(data->_valueIndex)
//    {
//        DATA_MANAGER_UTILS::copyList(data->_materialKeyList,   _materialKeyList);
//        DATA_MANAGER_UTILS::copyList(data->_materialValueList, _materialValueList);
//        DATA_MANAGER_UTILS::copyList(data->_rewardValueList,   _rewardValueList);
//	}
//};
//
namespace ACHIEVEMENT_DEFINE {
	static const int LIMIT_COUNT = 3;
	static const std::string NORMAL_CONTENT = "ACHIEVEMENT_NORMAL_CONTENT_%d";
    static const std::string HIDDEN_CONTENT = "ACHIEVEMENT_HIDDEN_CONTENT_%d";
	static const std::string HIDDEN_TITLE   = "ACHIEVEMENT_HIDDEN_TITLE_%d_%d";
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
typedef std::map<int, const ACHIEVEMENT*> ACHIEVEMENT_LIST;
typedef std::function<bool(const ACHIEVEMENT*)> ACHIEVEMENT_PICK;
//typedef std::vector<int> NORMAL_ACHIEVEMENT_VALUE_LIST;
//typedef std::map<std::string, NORMAL_ACHIEVEMENT_VALUE_LIST> VALUE_MAP;
class CAchievementDataManager
{
public:
    static CAchievementDataManager* Instance();
	bool CheckCompleteAll();
	bool CheckAchievementComplete(int index, bool isHidden);
	const ACHIEVEMENT* CompleteCheckRealTime(bool isHidden);
    void ResetNormalAchievements();

    
	sREWARD_DATA Reward(int index);
	sREWARD_DATA RewardByKey(std::string key, int value);
    int NonCompleteAchievementExist();
    void getNewAchievements();
    const ACHIEVEMENT* SkipAchievement(int index);
    
    //getter & setter
    const ACHIEVEMENT* getNormalAchievementByIndex(int index) const;
    const ACHIEVEMENT* getHiddenAchievementByIndex(int index) const;
    const ACHIEVEMENT* getNewRandomAchievement();
    const ACHIEVEMENT* getFirstFromNonCompleted();
    
    ACHIEVEMENT_LEVEL  getCurLevelDataByIndex(int index, bool isHidden);
    std::string getAchievementTitleByIndex(int index);
    std::string getAchievementContentsByIndex(int index, bool isHidden);
//    cocos2d::Sprite* getRewardSprite(std::string rewardKey, int rewardValue);
    ACHIEVEMENT_LIST getHiddenAchievementList() const;
    ACHIEVEMENT_LIST getNonCompletedAchievementList() const;
    ACHIEVEMENT_LIST getPickedAchievementList() const;
    ACHIEVEMENT_LIST getRunnnigAchievementList() const;
//    void UpdateCurHiddenAchievementList();
    
    static int getAchievementLevelByIndex(int index);
    static int getAchievementStateByIndex(int index);

private:
    void initWithJson(ACHIEVEMENT_LIST &list, std::string fileName);
	void initETCChekerList();
	void initRewarderList();
    
//	const Json::Value initAchievementWithDefaultValue(bool hidden, std::string key, const Json::Value data);
	void addAchievementToList(ACHIEVEMENT_LIST &list, const Json::Value &data);
    void addLevelToAchievement(ACHIEVEMENT_LEVEL_LIST &list, const Json::Value &data);
    void addMaterialToLevel(MATERIAL_LIST &list, const Json::Value &data);
    
    
//    void addValueListToMap(const Json::Value data);

    const ACHIEVEMENT* getNewRandomAchievementFromList(ACHIEVEMENT_LIST &list);

    ACHIEVEMENT_LEVEL getLevelDataFromAchievement(int index, int level, bool isHidden);
    
    void removeAchievementFromUserData(int index);
    
    void getAchievementParamListByType(PARAM_DATA_ARRAY& list,
                                       bool isHidden);
//    int getMaterialValueByLevel(std::string key, int level);

    CAchievementDataManager();
    virtual ~CAchievementDataManager();
    
private:
    ACHIEVEMENT_LIST m_NormalAchievementDataList;
	ACHIEVEMENT_LIST m_HiddenAchievementDataList;
    CHECKER_LIST m_CheckerList;
    REWARDER_LIST m_RewarderList;
    CAchievementClearChecker* m_Checker;
    CAchievementRewarder* m_Rewarder;
//    VALUE_MAP m_ValueMap;
//	Json::Value m_NormalAchievementDefaultSet;
//	Json::Value m_HiddenAchievementDefaultSet;
};