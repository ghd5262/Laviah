#include "UserLevelDataManager.hpp"
#include "../Common/HSHUtility.h"

using namespace cocos2d;
CUserLevelDataManager::CUserLevelDataManager()
{
    m_LevelList.clear();
    this->initWithJson("userLevelList.json");
}

CUserLevelDataManager* CUserLevelDataManager::Instance()
{
    static CUserLevelDataManager instance;
    return &instance;
}

bool CUserLevelDataManager::IsMaxLevel(int level)
{
    return (m_LevelList.size() <= level);
}

bool CUserLevelDataManager::LevelUpCheck(int oldLevel, int newExp)
{
    if(this->IsMaxLevel(oldLevel)) return false;
    
    auto levelData = this->getLevelDataByIndex(oldLevel);
    return (levelData._levelExp <= newExp);
}

void CUserLevelDataManager::initWithJson(std::string fileName)
{
    Json::Value root;
    Json::Reader reader;
    
    std::string file     = FileUtils::getInstance()->fullPathForFilename(fileName);
    std::string fileData = FileUtils::getInstance()->getStringFromFile(file);
    size_t pos           = fileData.rfind("}");
    fileData             = fileData.substr(0, pos + 1);
    bool parsingSucceed  = reader.parse(fileData, root);
    
    if (!parsingSucceed)
    {
        CCASSERT(false, MakeString("parser failed : \n %s", fileData.c_str()).c_str());
        return;
    }
    CCLOG("userLevelList.json : \n %s\n", fileData.c_str());
    
    const Json::Value levelArray = root["levels"];
    for (auto levelData : levelArray)
        this->addLevelDataToList(m_LevelList, levelData);
}

void CUserLevelDataManager::addLevelDataToList(LEVEL_LIST& list, const Json::Value& json)
{
    USER_LEVEL levelData;
    
    levelData._index    = json["index"].asInt();
    levelData._levelExp = json["levelExp"].asInt();
    levelData._sumExp   = json["sumExp"].asInt();
    
    list.emplace_back(levelData);
}

USER_LEVEL CUserLevelDataManager::getLevelDataByIndex(int index)
{
    if(m_LevelList.size() <= index){
        CCLOG("Wrong level index %d", index);
        CCASSERT(false, "Wrong level index");
    }
    
    return m_LevelList.at(index);
}
