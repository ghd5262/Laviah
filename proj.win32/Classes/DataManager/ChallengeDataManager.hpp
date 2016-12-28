#pragma once
#include "../Common/HSHUtility.h"
#include <map>
#include <vector>
#include <algorithm>

struct sCHALLENGE_PARAM
{
    int _index;
    int _level;
    bool _oneTime;
    std::string _contents;
    std::map<std::string, int> _materialList;
    std::map<std::string, int> _rewardList;
    
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
	}
    
    sCHALLENGE_PARAM(const sCHALLENGE_PARAM* data)
    : _index(data->_index)
    , _level(data->_level)
    , _oneTime(data->_oneTime)
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
    
    //getter & setter
    const sCHALLENGE_PARAM* getChallengeByIndex(int index) const;
    const sCHALLENGE_PARAM* getRandomChallenge(bool oneTime);
    const sCHALLENGE_PARAM* getRandomChallengeByLevel(int level, bool below);
    const sCHALLENGE_PARAM* getRandomChallengeFromList(const CHALLENGE_PICK& callFunc,
                                                       CHALLENGE_LIST &list);
    
private:
    void initWithJson(CHALLENGE_LIST &list, std::string fileName);
    
    CChallengeDataManager();
    virtual ~CChallengeDataManager();
    
private:
    CHALLENGE_LIST m_CallengeList;
};

