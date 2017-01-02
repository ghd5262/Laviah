#pragma once
#include "ChallengeChecker/ChallengeChecker.h"
#include "../Common/HSHUtility.h"
#include <map>
#include <vector>
#include <algorithm>

class CChallengeClearChecker;

typedef std::vector<std::string> KEY_LIST;
typedef std::map<std::string, int> MATERIAL_LIST;
typedef std::map<std::string, int> REWARD_LIST;
typedef std::shared_ptr<CChallengeClearChecker> CHECKER;
typedef std::vector<CHECKER> CHECKER_LIST;

namespace CHALLENGE_DATA_KEY {
#define COIN_SCORE           "COIN_SCORE"
#define STAR_SCORE           "STAR_SCORE"
#define RUN_SCORE            "RUN_SCORE"
    
#define BEST_SCORE           "BEST_SCORE"
#define BEST_COMBO           "BEST_COMBO"
    
#define CHARACTER_COLLECT    "CHARACTER_COLLECT"
#define ROCKET_COLLECT       "ROCKET_COLLECT"
    
#define CHARACTER_COUNT      "CHARACTER_COUNT"
#define ROCKET_COUNT         "ROCKET_COUNT"
    
#define USER_LEVEL           "USER_LEVEL"
#define WORKSHOP_LEVEL       "WORKSHOP_LEVEL"
    
#define COMBO                "COMBO"
#define COIN                 "COIN"
#define ITEM_USE             "ITEM_USE"
};

struct sCHALLENGE_PARAM
{
    int _index;
    int _level;
    bool _oneTime;
    std::string _contents;
	MATERIAL_LIST _materialList;
	REWARD_LIST _rewardList;
	CHECKER_LIST _challengeCheckerList;

    sCHALLENGE_PARAM()
    : _index(-1)
    , _level(-1)
    , _oneTime(false)
    , _contents(""){}
    
    sCHALLENGE_PARAM(const sCHALLENGE_PARAM& data)
    : _index(data._index)
    , _level(data._level)
    , _oneTime(data._oneTime)
    , _contents(data._contents)
    {
		_materialList.insert(std::begin(data._materialList), std::end(data._materialList));
		_rewardList.insert(std::begin(data._rewardList), std::end(data._rewardList));
		std::copy(std::begin(data._challengeCheckerList), std::end(data._challengeCheckerList), std::begin(_challengeCheckerList));
	}
    
    sCHALLENGE_PARAM(const sCHALLENGE_PARAM* data)
    : _index(data->_index)
    , _level(data->_level)
    , _oneTime(data->_oneTime)
    , _contents(data->_contents)
    {
		_materialList.insert(std::begin(data->_materialList), std::end(data->_materialList));
		_rewardList.insert(std::begin(data->_rewardList), std::end(data->_rewardList));
		std::copy(std::begin(data->_challengeCheckerList), std::end(data->_challengeCheckerList), std::begin(_challengeCheckerList));
	}
};

class CChallengeDataManager
{
    typedef std::vector<const sCHALLENGE_PARAM*> CHALLENGE_LIST;
    typedef std::function<bool(const sCHALLENGE_PARAM*)> CHALLENGE_PICK;
public:
    static CChallengeDataManager* Instance();
    
	void UpdateCurrentState(std::string key, int value);
	const sCHALLENGE_PARAM* SkipChallenge(int index);

    //getter & setter
    const sCHALLENGE_PARAM* getChallengeByIndex(int index) const;
    const sCHALLENGE_PARAM* getNewRandomChallenge(bool oneTime);
    const sCHALLENGE_PARAM* getMewRandomChallengeByLevel(int level, bool below);
    const sCHALLENGE_PARAM* getNewRandomChallengeFromList(const CHALLENGE_PICK& callFunc,
                                                       CHALLENGE_LIST &list);
    
private:
    void initWithJson(CHALLENGE_LIST &list, std::string fileName);
    void initKeyListWithJson(std::string fileName);
	void initChallengeClearChecker(sCHALLENGE_PARAM* data);
	void addMaterialToCurrentState(std::string key, int value);
	bool checkCurrentChallengeComplete(int index);
	void completeAllCurrentChallenges();

    CChallengeDataManager();
    virtual ~CChallengeDataManager();
    
private:
    CHALLENGE_LIST m_CallengeDataList;
	MATERIAL_LIST m_CurrentState;
    KEY_LIST m_MaterialKeyList;
    KEY_LIST m_RewardKeyList;
};
