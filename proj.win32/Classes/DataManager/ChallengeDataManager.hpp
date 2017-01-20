#pragma once
#include "../Common/HSHUtility.h"
#include <map>
#include <vector>
#include <algorithm>

class CChallengeClearChecker;
class CChallengeRewarder;
struct sCHALLENGE_PARAM
{
    int _index;
    int _level;
    bool _continuingType;
    std::string _contents;
    std::string _materialKey;
    std::string _rewardKey;
    int _materialValue;
    int _rewardValue;

    sCHALLENGE_PARAM()
    : _index(-1)
    , _level(-1)
    , _continuingType(false)
    , _contents("")
    , _materialKey("")
    , _rewardKey("")
    , _materialValue(0)
    , _rewardValue(0){}
    
    sCHALLENGE_PARAM(const sCHALLENGE_PARAM& data)
    : _index(data._index)
    , _level(data._level)
    , _continuingType(data._continuingType)
    , _contents(data._contents)
    , _materialKey(data._materialKey)
    , _rewardKey(data._rewardKey)
    , _materialValue(data._materialValue)
    , _rewardValue(data._rewardValue)
    {}
    
    sCHALLENGE_PARAM(const sCHALLENGE_PARAM* data)
    : _index(data->_index)
    , _level(data->_level)
    , _continuingType(data->_continuingType)
    , _contents(data->_contents)
    , _materialKey(data->_materialKey)
    , _rewardKey(data->_rewardKey)
    , _materialValue(data->_materialValue)
    , _rewardValue(data->_rewardValue)
    {}
};

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

namespace CHALLENGE {
    static const int LIMIT_COUNT = 3;
}

typedef std::function<bool(int)> CHECKER;
typedef std::map<std::string, CHECKER> CHECKER_LIST;
typedef std::function<sREWARD_DATA(sREWARD_DATA)> REWARDER;
typedef std::map<std::string, REWARDER> REWARDER_LIST;
typedef std::vector<const sCHALLENGE_PARAM*> CHALLENGE_LIST;
typedef std::function<bool(const sCHALLENGE_PARAM*)> CHALLENGE_PICK;

class CChallengeDataManager
{
public:
    static CChallengeDataManager* Instance();
	bool CheckCompleteAll();
	bool CheckChallengeComplete(int index);
	const sCHALLENGE_PARAM* CompleteCheckRealTime();

	sREWARD_DATA Reward(int index);
	sREWARD_DATA RewardByKey(std::string key, int value);
    int NonCompleteChallengeExist(int level,
                                  bool below,
                                  bool continuingType = false);
    void getNewChallenges();
    const sCHALLENGE_PARAM* SkipChallenge(int index);

    //getter & setter
    const sCHALLENGE_PARAM* getChallengeByIndex(int index) const;
    const sCHALLENGE_PARAM* getNewRandomChallenge(int level,
                                                  bool below,
                                                  bool continuingType = false);
    cocos2d::Sprite* getRewardSprite(std::string rewardKey, int rewardValue);
    static CHALLENGE_LIST getListByFunc(const CHALLENGE_PICK &func, CHALLENGE_LIST list);

private:
    void initWithJson(CHALLENGE_LIST &list, std::string fileName);
    void initMaterialKeyList();
    void initRewardKeyList();
    const sCHALLENGE_PARAM* getNewRandomChallengeFromList(CHALLENGE_LIST &list);
    CHALLENGE_LIST getNonCompletedChallengeList(int level,
                                                bool below,
                                                bool continuingType = false);
    void removeChallengeFromUserData(int index);
    
    CChallengeDataManager();
    virtual ~CChallengeDataManager();
    
private:
    CHALLENGE_LIST m_ChallengeDataList;
    CHECKER_LIST m_CheckerList;
    REWARDER_LIST m_RewarderList;
    CChallengeClearChecker* m_Checker;
    CChallengeRewarder* m_Rewarder;
};