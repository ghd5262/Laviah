#include "RankRewardDataManager.hpp"
#include "UserDataManager.h"
#include "DataManagerUtils.h"

using namespace cocos2d;

CRankRewardDataManager::CRankRewardDataManager()
{
    initWithJson(m_RewardList, "rankRewardList.json");
}

CRankRewardDataManager::~CRankRewardDataManager(){}

CRankRewardDataManager* CRankRewardDataManager::Instance()
{
    static CRankRewardDataManager instance;
    return &instance;
}

void CRankRewardDataManager::initWithJson(RANK_REWARD_LIST &list, std::string fileName)
{
    Json::Value        root;
    Json::Reader       reader;
    auto jsonString  = FileUtils::getInstance()->fullPathForFilename(fileName);
    auto clearString = FileUtils::getInstance()->getStringFromFile(jsonString);
    auto pos         = clearString.rfind("}");
    clearString      = clearString.substr(0, pos + 1);
    bool succeed     = reader.parse(clearString, root);
    if (!succeed)
    {
        CCASSERT(false, StringUtils::format("parser failed : \n %s", clearString.c_str()).c_str());
        return;
    }
    CCLOG("RANK REWARD JSON : \n %s\n", clearString.c_str());
    
    const Json::Value array = root["rankReward"];
    for(auto reward : array)
        this->addRewardToList(reward);
}

void CRankRewardDataManager::addRewardToList(const Json::Value& json)
{
    RANK_REWARD_DATA data;
    
    data._index         = json["index"].asInt();
    data._rank          = json["rank"].asInt();
    data._friendsCount  = json["friendCount"].asInt();
    auto keyList        = json["rewardKey"];
    auto valueList      = json["rewardValue"];
    
    for(auto key : keyList){
        if(key.isNull()) continue;
        data._keyList.emplace_back(key.asString());
    }
    for(auto value : valueList){
        if(value.isNull()) continue;
        data._valueList.emplace_back(value.asInt());
    }
    m_RewardList.emplace_back(data);
}

RANK_REWARD_DATA CRankRewardDataManager::getRewardByData(int rank, int freindsCount) const
{
    auto list = DATA_MANAGER_UTILS::getNonPtrListByFunc([=](RANK_REWARD_DATA data){
        if(data._rank != rank) return false;
        if(data._friendsCount >= freindsCount) return false;
        
        return true;
    }, m_RewardList);
    
    auto size = list.size();
    if(!size) return RANK_REWARD_DATA();
    return list.at(size-1);
}
