#pragma once
#include "../Common/HSHUtility.h"
#include "../json/json.h"
#include <vector>
#include <map>

struct RANK_REWARD_DATA{
    int _index;
    int _rank;
    int _friendsCount;
    std::vector<std::string> _keyList;
    std::vector<int> _valueList;
    
    RANK_REWARD_DATA()
    : _index(0)
    , _rank(0)
    , _friendsCount(0){
        _keyList.clear();
        _valueList.clear();
    }
};

typedef std::vector<RANK_REWARD_DATA> RANK_REWARD_LIST;
class CRankRewardDataManager
{
public:
    static CRankRewardDataManager* Instance();
    
    RANK_REWARD_DATA getRewardByData(int rank, int freindsCount) const;
    
private:
    void initWithJson(RANK_REWARD_LIST &list, std::string fileName);
    void addRewardToList(const Json::Value& json);

    CRankRewardDataManager();
    virtual ~CRankRewardDataManager();
    
private:
    RANK_REWARD_LIST m_RewardList;
};
