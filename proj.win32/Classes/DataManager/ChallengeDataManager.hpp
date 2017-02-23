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

class CChallengeClearChecker;
class CChallengeRewarder;
struct sCHALLENGE_PARAM
{
    int _index;
	bool _hiddenType;
	bool _visible;
	bool _isHighLevel;
	CHECKER_TYPE _checkerType;
    std::string _contents;
    std::string _rewardKey;
    int _rewardValue;
	std::vector<std::string> _materialKeyList;
	std::vector<int> _materialValueList;

    sCHALLENGE_PARAM()
    : _index(-1)
	, _hiddenType(false)
	, _visible(false)
	, _isHighLevel(false)
	, _checkerType(CHECKER_TYPE::ETC)
    , _contents("")
    , _rewardKey("")
    , _rewardValue(0){}
    
    sCHALLENGE_PARAM(const sCHALLENGE_PARAM& data)
    : _index(data._index)
	, _hiddenType(data._hiddenType)
	, _visible(data._visible)
	, _isHighLevel(data._isHighLevel)
	, _checkerType(data._checkerType)
    , _contents(data._contents)
    , _rewardKey(data._rewardKey)
    , _rewardValue(data._rewardValue)
    {
        DATA_MANAGER_UTILS::copyList(data._materialKeyList,   _materialKeyList);
        DATA_MANAGER_UTILS::copyList(data._materialValueList, _materialValueList);
	}
    
    sCHALLENGE_PARAM(const sCHALLENGE_PARAM* data)
    : _index(data->_index)
	, _hiddenType(data->_hiddenType)
	, _visible(data->_visible)
	, _isHighLevel(data->_isHighLevel)
	, _checkerType(data->_checkerType)
    , _contents(data->_contents)
    , _rewardKey(data->_rewardKey)
    , _rewardValue(data->_rewardValue)
    {
        DATA_MANAGER_UTILS::copyList(data->_materialKeyList,   _materialKeyList);
        DATA_MANAGER_UTILS::copyList(data->_materialValueList, _materialValueList);
	}
};

namespace CHALLENGE_DEFINE {
	static const int LIMIT_COUNT = 3;
	static const std::string NORMAL_CONTENT = "CHALLENGE_NORMAL_CONTENT_%d";
	static const std::string NORMAL_HIDDEN  = "CHALLENGE_HIDDEN_CONTENT_%d";
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
typedef std::map<int, const sCHALLENGE_PARAM*> CHALLENGE_LIST;
typedef std::function<bool(const sCHALLENGE_PARAM*)> CHALLENGE_PICK;

class CChallengeDataManager
{
public:
    static CChallengeDataManager* Instance();
	bool CheckCompleteAll();
	bool CheckChallengeComplete(int index, bool isHidden);
	const sCHALLENGE_PARAM* CompleteCheckRealTime(bool isHidden);

	sREWARD_DATA Reward(int index);
	sREWARD_DATA RewardByKey(std::string key, int value);
    int NonCompleteChallengeExist();
    void getNewChallenges();
    const sCHALLENGE_PARAM* SkipChallenge(int index);

    //getter & setter
    const sCHALLENGE_PARAM* getNormalChallengeByIndex(int index) const;
    const sCHALLENGE_PARAM* getHiddenChallengeByIndex(int index) const;
    const sCHALLENGE_PARAM* getNewRandomChallenge();
    const sCHALLENGE_PARAM* getNonCompleteChallengeFromCurrentList();
    cocos2d::Sprite* getRewardSprite(std::string rewardKey, int rewardValue);
    CHALLENGE_LIST getHiddenChallengeList() const {
        return m_HiddenChallengeDataList;
    }
    void UpdateCurHiddenChallengeList();
    
private:
    void initWithJson(std::string fileName);
	void initETCChekerList();
	void initRewarderList();
	void initMaterialValueListByUserData(sCHALLENGE_PARAM* data);

	const Json::Value initChallengeWithDefaultValue(bool hidden, std::string key, const Json::Value data);
	void addChallengeToList(CHALLENGE_LIST &list, const Json::Value& data, bool hiddenType);

    const sCHALLENGE_PARAM* getNewRandomChallengeFromList(CHALLENGE_LIST &list);
    CHALLENGE_LIST getNonCompletedChallengeList();
    void removeChallengeFromUserData(int index);
    
    void getCurChallengeListByType(ARRAY_DATA& list, bool isHidden);
    
    CChallengeDataManager();
    virtual ~CChallengeDataManager();
    
private:
    CHALLENGE_LIST m_NormalChallengeDataList;
	CHALLENGE_LIST m_HiddenChallengeDataList;
    CHECKER_LIST m_CheckerList;
    REWARDER_LIST m_RewarderList;
    CChallengeClearChecker* m_Checker;
    CChallengeRewarder* m_Rewarder;
	Json::Value m_NormalChallengeDefaultSet;
	Json::Value m_HiddenChallengeDefaultSet;
};