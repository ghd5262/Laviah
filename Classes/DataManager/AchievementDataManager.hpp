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
    NORMAL_STATE = 0, ///< 0: non-completed, 1: running, 2: finished, 3: completed
};

enum ACHIEVEMENT_STATE{
    NON_COMPLETED = 0,
    RUNNING,
    FINISHED,
    COMPLETED,
};

enum USERDATA_PARAM_ACHIEVEMENT_HIDDEN{
    HIDDEN_LEVEL = 0,
     HIDDEN_STATE= 1,
};

class CAchievementClearChecker;
class CAchievementRewarder;

typedef std::vector<int> MATERIAL_VALUES;
struct ACHIEVEMENT_MATERIAL {
    std::string _materialKey;
    MATERIAL_VALUES _materialValues;
    
    ACHIEVEMENT_MATERIAL()
    : _materialKey(""){
        _materialValues.clear();
    };
};

typedef std::vector<ACHIEVEMENT_MATERIAL> MATERIAL_LIST;
struct ACHIEVEMENT_LEVEL {
    std::string _rewardKey;
    int _rewardValue;
    int _contentsValue;
    CHECKER_TYPE _checkerType;
    MATERIAL_LIST _materialList;
    
    ACHIEVEMENT_LEVEL()
    : _rewardKey("")
    , _rewardValue(0)
    , _contentsValue(0)
    , _checkerType(CHECKER_TYPE::ETC){
        _materialList.clear();
    };
};

typedef std::vector<ACHIEVEMENT_LEVEL> ACHIEVEMENT_LEVEL_LIST;
struct ACHIEVEMENT {
    int _index;
    bool _hiddenType;
    bool _visibleType;
    int _currentValue;
    ACHIEVEMENT_LEVEL_LIST _levelList;
    
    ACHIEVEMENT()
    : _index(-1)
    , _hiddenType(false)
    , _visibleType(false)
    , _currentValue(0){
        _levelList.clear();
    }
};

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

typedef std::function<bool(int, int&)> CHECKER;
typedef std::map<std::string, CHECKER> CHECKER_LIST;
typedef std::function<sREWARD_DATA(sREWARD_DATA)> REWARDER;
typedef std::map<std::string, REWARDER> REWARDER_LIST;
typedef std::map<int, const ACHIEVEMENT*> ACHIEVEMENT_LIST;
typedef std::function<bool(const ACHIEVEMENT*)> ACHIEVEMENT_PICK;
class CAchievementDataManager
{
public:
    static CAchievementDataManager* Instance();
	bool CheckCompleteAll();
	bool CheckAchievementComplete(int index, bool isHidden);
	const ACHIEVEMENT* CompleteCheckRealTime(bool isHidden);
    void ResetNormalAchievements();
    void HiddenAchievementLevelUP(int index);
    bool CompletedAllOfLevels(int index);
    bool ExistCompletedHiddenAchievement();
    
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
    std::string getAchievementTitle(int index, int level);
    std::string getAchievementContentsByIndex(int index, bool isHidden);
    ACHIEVEMENT_LIST getHiddenAchievementList() const;
    ACHIEVEMENT_LIST getNonCompletedAchievementList() const;
    ACHIEVEMENT_LIST getPickedAchievementList() const;
    ACHIEVEMENT_LIST getRunnnigAchievementList() const;
    int getAchievementMaxLevelByIndex(int index, bool isHidden);
    int getHiddenAchievementCurrentValue(int index);
    
    static int getAchievementLevelByIndex(int index, bool isHidden);
    static int getAchievementStateByIndex(int index, bool isHidden);
    static void setAchievementLevelByIndex(int index, int level);
    static void setAchievementStateByIndex(int index, int state, bool isHidden);
private:
    void initWithJson(ACHIEVEMENT_LIST &list, std::string fileName);
	void initETCChekerList();
	void initRewarderList();
    
	void addAchievementToList(ACHIEVEMENT_LIST &list, const Json::Value &data);
    void addLevelToAchievement(ACHIEVEMENT_LEVEL_LIST &list, const Json::Value &data);
    void addMaterialToLevel(MATERIAL_LIST &list, const Json::Value &data);

    const ACHIEVEMENT* getNewRandomAchievementFromList(ACHIEVEMENT_LIST &list);

    ACHIEVEMENT_LEVEL getLevelDataFromAchievement(int index, int level, bool isHidden);
        
    void getAchievementParamListByType(PARAM_DATA_ARRAY& list,
                                       bool isHidden);

    CAchievementDataManager();
    virtual ~CAchievementDataManager();
    
private:
    ACHIEVEMENT_LIST m_NormalAchievementDataList;
	ACHIEVEMENT_LIST m_HiddenAchievementDataList;
    CHECKER_LIST m_CheckerList;
    REWARDER_LIST m_RewarderList;
    CAchievementClearChecker* m_Checker;
    CAchievementRewarder* m_Rewarder;
};