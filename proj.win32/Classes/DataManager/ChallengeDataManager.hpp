#pragma once
#include "../Common/HSHUtility.h"
#include <map>
#include <vector>
#include <algorithm>

typedef std::vector<std::string> KEY_LIST;
typedef std::map<std::string, int> MATERIAL_LIST;
typedef std::map<std::string, int> REWARD_LIST;
typedef std::function<bool(int)> CHECKER;
typedef std::map<std::string, CHECKER> CHECKER_LIST;
typedef std::function<void(int)> REWARDER;
typedef std::map<std::string, REWARDER> REWARDER_LIST;

class CChallengeClearChecker;
class CChallengeRewarder;
struct sCHALLENGE_PARAM
{
    int _index;
    int _level;
    bool _continuingType;
    std::string _contents;
	MATERIAL_LIST _materialList;
	REWARD_LIST _rewardList;

    sCHALLENGE_PARAM()
    : _index(-1)
    , _level(-1)
    , _continuingType(false)
    , _contents(""){}
    
    sCHALLENGE_PARAM(const sCHALLENGE_PARAM& data)
    : _index(data._index)
    , _level(data._level)
    , _continuingType(data._continuingType)
    , _contents(data._contents)
    {
		_materialList.insert(std::begin(data._materialList), std::end(data._materialList));
		_rewardList.insert(std::begin(data._rewardList), std::end(data._rewardList));
	}
    
    sCHALLENGE_PARAM(const sCHALLENGE_PARAM* data)
    : _index(data->_index)
    , _level(data->_level)
    , _continuingType(data->_continuingType)
    , _contents(data->_contents)
    {
		_materialList.insert(std::begin(data->_materialList), std::end(data->_materialList));
		_rewardList.insert(std::begin(data->_rewardList), std::end(data->_rewardList));
	}
};

class CChallengeDataManager
{
    typedef std::vector<const sCHALLENGE_PARAM*> CHALLENGE_LIST;
    typedef std::function<bool(const sCHALLENGE_PARAM*)> CHALLENGE_PICK;
public:
    static CChallengeDataManager* Instance();
    
    void Reward(int index);
    const sCHALLENGE_PARAM* SkipChallenge(int index);

    //getter & setter
    const sCHALLENGE_PARAM* getChallengeByIndex(int index) const;
    const sCHALLENGE_PARAM* getNewRandomChallenge(bool oneTime);
    const sCHALLENGE_PARAM* getMewRandomChallengeByLevel(int level, bool below);
    const sCHALLENGE_PARAM* getNewRandomChallengeFromList(const CHALLENGE_PICK& callFunc,
                                                       CHALLENGE_LIST &list);
    
private:
    void initWithJson(CHALLENGE_LIST &list, std::string fileName);
    void initMaterialKeyList();
    void initRewardKeyList();
	bool checkChallengeComplete(int index);
	void completeAllCurrentChallenges();

    CChallengeDataManager();
    virtual ~CChallengeDataManager();
    
private:
    CHALLENGE_LIST m_CallengeDataList;
    CHECKER_LIST m_CheckerList;
    REWARDER_LIST m_RewarderList;
    KEY_LIST m_MaterialKeyList;
    KEY_LIST m_RewardKeyList;
    CChallengeClearChecker* m_Checker;
    CChallengeRewarder* m_Rewarder;
};
