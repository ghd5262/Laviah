#include "StageDataManager.hpp"
#include "UserDataManager.h"
#include "DataManagerUtils.h"
#include "CharacterDataManager.h"

using namespace cocos2d;

CStageDataManager::CStageDataManager()
{
    initWithJson(m_StageList, "stageList.json");
}

CStageDataManager::~CStageDataManager()
{
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_StageList);
}

CStageDataManager* CStageDataManager::Instance()
{
    static CStageDataManager instance;
    return &instance;
}

void CStageDataManager::initWithJson(STAGE_LIST &list, std::string fileName)
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
    CCLOG("STAGE JSON : \n %s\n", clearString.c_str());
    
    const Json::Value array = root["stageList"];
    for(auto stage : array)
        this->addStageToList(stage);
}

void CStageDataManager::addStageToList(const Json::Value& json)
{
    auto index       = json["index"].asInt();
    auto openLevel   = json["openLevel"].asInt();
    auto stageData   = json["stageData"];

    auto iter        = m_StageList.find(index);
    STAGE* data      = nullptr;
    
    if(iter == m_StageList.end()) data = new STAGE();
    else                          data = iter->second;
    
    data->_index     = index;
    data->_openLevel = openLevel;
    
    this->addStageDataToStage(data->_stageDataLiat, stageData);
    
    m_StageList.emplace(std::pair<int, STAGE*>(data->_index, data));
}

void CStageDataManager::addStageDataToStage(STAGE_DATA_LIST &list, const Json::Value &json)
{
    STAGE_DATA data;
    
    data._noticeLevel       = json["noticeLevel"].asInt();
    data._patternLevel      = json["patternLevel"].asInt();
    data._type              = json["type"].asInt();
    data._patternIndex      = json["patternIndex"].asInt();
    data._changeDuration    = json["changeDuration"].asDouble();
    data._duration          = json["duration"].asDouble();
    data._changeTime        = json["changeTime"].asDouble();
    data._zoomAngle         = json["zoomAngle"].asDouble();
    data._zoomSize          = json["zoomSize"].asDouble();
    data._speed             = json["speed"].asDouble();
    auto posX               = json["x"].asDouble();
    auto posY               = json["y"].asDouble();
    data._pos               = cocos2d::Vec2(posX, posY);
    
    
    list.emplace_back(data);
}

const STAGE* CStageDataManager::getStageByIndex(int index) const
{
    auto data = m_StageList.find(index);
    if(data == m_StageList.end()) {
        CCLOG("Wrong STAGE index : %d", index);
        CCASSERT(false, "Wrong STAGE index");
        return nullptr;
    }
    return data->second;
}

const STAGE* CStageDataManager::getStageByUserLevel()
{
    auto level      = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL);
    auto pickedList = DATA_MANAGER_UTILS::getMapByFunc([=](STAGE* data){
        return data->_openLevel <= level;
    }, m_StageList);
    
    auto size       = pickedList.size();
    if(size <= 0) {
        CCLOG("There is no stage.");
        return nullptr;
    }
    auto picked     = pickedList.begin();
    std::advance(picked, random<int>(0, int(pickedList.size()-1)));
    
    CCLOG("Pick a stage :: idx %d", (picked->second)->_index);
    
    return (picked->second);

}
