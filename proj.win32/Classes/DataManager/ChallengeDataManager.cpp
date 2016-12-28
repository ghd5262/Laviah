#include "ChallengeDataManager.hpp"
#include "../json/json.h"

using namespace cocos2d;

const static std::string FILE_NAME = "challengeList.json";

CChallengeDataManager::CChallengeDataManager()
{
    initWithJson(m_CallengeList, FILE_NAME);
}

CChallengeDataManager::~CChallengeDataManager()
{
    auto cleanList = [=](CHALLENGE_LIST &list){
        for (auto data : list)
        {
            delete data;
            data = nullptr;
        }
        list.clear();
    };
    
    cleanList(m_CallengeList);
}

CChallengeDataManager* CChallengeDataManager::Instance()
{
    static CChallengeDataManager instance;
    return &instance;
}

void CChallengeDataManager::initWithJson(CHALLENGE_LIST &list, std::string fileName)
{
    Json::Value root;
    Json::Reader reader;
    
    std::string file = FileUtils::getInstance()->fullPathForFilename(fileName);
    std::string fileData = FileUtils::getInstance()->getStringFromFile(file);
    size_t pos = fileData.rfind("}");
    fileData = fileData.substr(0, pos + 1);
    
    bool parsingSuccessful = reader.parse(fileData, root);
    if (!parsingSuccessful)
    {
        CCASSERT(false, MakeString("parser failed : \n %s", fileData.c_str()).c_str());
        return;
    }
    CCLOG("Challenge List JSON : \n %s\n", fileData.c_str());
    
    
    const Json::Value challengeArray = root["challenges"];
    
    for (unsigned int count = 0; count < challengeArray.size(); ++count)
    {
        const Json::Value challenge = challengeArray[count];
        
        sCHALLENGE_PARAM challengeInfo;
        
        challengeInfo._index        = list.size();
        challengeInfo._level        = challenge["level"].asInt();
        challengeInfo._oneTime      = challenge["oneTime"].asBool();
        challengeInfo._contents     = challenge["contents"].asString();
        const Json::Value materialList  = challenge["material"];
        const Json::Value rewardList    = challenge["reward"];
        
        for (auto material : materialList)
        {
            auto key = materialList[material];
            auto value = materialList[material].asInt();
            challengeInfo._materialList.emplace(std::pair<std::string, int>("", value));
        }
        
        for (unsigned int reward = 0; reward < rewardList.size(); reward++)
        {
            auto key = materialList[reward];
            auto value = materialList[reward].asInt();
            challengeInfo._rewardList.emplace(std::pair<std::string, int>("", value));
        }
        
        list.emplace_back(new sCHALLENGE_PARAM(challengeInfo));
    }
}

const sCHALLENGE_PARAM* CChallengeDataManager::getChallengeByIndex(int index) const
{
    if (m_CallengeList.size() <= index) {
        CCLOG("Wrong index : %d", index);
        CCASSERT(false, "Wrong index");
        return nullptr;
    }
    return m_CallengeList.at(index);
}

const sCHALLENGE_PARAM* CChallengeDataManager::getRandomChallenge(bool oneTime)
{
    if(oneTime){
        return getRandomChallengeFromList([=](const sCHALLENGE_PARAM* data){
            return !data->_oneTime;
        }, m_CallengeList);
    }
    else
    {
        return getRandomChallengeFromList([=](const sCHALLENGE_PARAM* data){
            return data->_oneTime;
        }, m_CallengeList);
    }
}

const sCHALLENGE_PARAM* CChallengeDataManager::getRandomChallengeByLevel(int level, bool below)
{
    if(below){
        return getRandomChallengeFromList([=](const sCHALLENGE_PARAM* data){
            return data->_level > level;
        }, m_CallengeList);
    }
    else
    {
        return getRandomChallengeFromList([=](const sCHALLENGE_PARAM* data){
            return data->_level != level;
        }, m_CallengeList);
    }
}

const sCHALLENGE_PARAM* CChallengeDataManager::getRandomChallengeFromList(const CHALLENGE_PICK& callFunc,
                                                                          CHALLENGE_LIST &list)
{
    const sCHALLENGE_PARAM* picked;
    do{
        auto size = list.size();
        auto randomIdx = random<int>(0, int(size) - 1);
        picked = list.at(randomIdx);
    } while (callFunc(picked));
    
    CCLOG("Pick a challenge :: \nidx %d \ncontent %s \nlevel %d",
          picked->_index,
          picked->_contents.c_str(),
          picked->_level);
    
    return picked;
}
