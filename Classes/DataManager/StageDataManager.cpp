#include "StageDataManager.hpp"
#include "UserDataManager.h"
#include "DataManagerUtils.h"
#include "CharacterDataManager.h"
#include "PlanetDataManager.hpp"
#include "../GameObject/ObjectManager.h"

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
    
    CCLOG("sdf");
}

void CStageDataManager::addStageToList(const Json::Value& json)
{
    auto index       = json["stageIndex"].asInt();
    auto openLevel   = json["openLevel"].asInt();
    auto stageData   = json["stageData"];

    auto iter        = m_StageList.find(index);
    STAGE* data      = nullptr;
    
    if(iter == m_StageList.end()) data = new STAGE();
    else                          data = iter->second;
    
    data->_stageIndex = index;
    data->_openLevel  = openLevel;
    
    this->addStageDataToStage(data, stageData);
    
    m_StageList.emplace(std::pair<int, STAGE*>(data->_stageIndex, data));
}

void CStageDataManager::addStageDataToStage(STAGE* stageData, const Json::Value &json)
{
    STAGE_DATA data;
    
    data._index             = json["index"].asInt();
    data._noticeLevel       = json["noticeLevel"].asInt();
    data._patternLevel      = json["patternLevel"].asInt();
    data._type              = json["type"].asInt();
    data._patternIndex      = json["patternIndex"].asInt();
    data._targetStar        = json["targetStar"].asInt();
    data._changeDuration    = json["changeDuration"].asDouble();
    data._duration          = json["duration"].asDouble();
    data._changeTime        = json["changeTime"].asDouble();
    data._zoomAngle         = json["zoomAngle"].asDouble();
    data._zoomSize          = json["zoomSize"].asDouble();
    data._speed             = json["speed"].asDouble();
    data._isSavePoint       = json["savePoint"].asBool();
    data._isLevelBelow      = json["isLevelBelow"].asBool();

    auto posX               = json["x"].asDouble();
    auto posY               = json["y"].asDouble();
    data._pos               = cocos2d::Vec2(posX, posY);
    
    auto bgTopR             = json["backgroundTop"]["r"].asInt();
    auto bgTopG             = json["backgroundTop"]["g"].asInt();
    auto bgTopB             = json["backgroundTop"]["b"].asInt();
    data._bgColorTop        = cocos2d::Color3B(bgTopR, bgTopG, bgTopB);
    
    auto bgBottomR          = json["backgroundBottom"]["r"].asInt();
    auto bgBottomG          = json["backgroundBottom"]["g"].asInt();
    auto bgBottomB          = json["backgroundBottom"]["b"].asInt();
    data._bgColorBottom     = cocos2d::Color3B(bgBottomR, bgBottomG, bgBottomB);
    
    auto bulletR            = json["bulletColor"]["r"].asInt();
    auto bulletG            = json["bulletColor"]["g"].asInt();
    auto bulletB            = json["bulletColor"]["b"].asInt();
    data._bulletColor       = cocos2d::Color3B(bulletR, bulletG, bulletB);
    
    if(data._isSavePoint)
        stageData->_savePointList.emplace_back(data._index);
    stageData->_stageDataList.emplace(std::pair<int, STAGE_DATA>(data._index, data));
}

int CStageDataManager::getStageMaxLevel(int index)
{
    auto data = m_StageList.find(index);
    if(data == m_StageList.end()) return 0;
    
    auto stageData = data->second->_stageDataList;
    auto size      = stageData.size();
    if(size <= 0) return 0;
    
    auto lastData  = stageData.at(size -1);
    return lastData._noticeLevel;
}

float CStageDataManager::getStageFinishTime(int index)
{
    auto data = m_StageList.find(index);
    if(data == m_StageList.end()) return 0.f;
    
    auto stageData = data->second->_stageDataList;
    auto size      = stageData.size();
    if(size <= 0) return 0.f;
    
    auto lastData  = stageData.at(size -1);
    return lastData._changeTime;
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
    
    CCLOG("Pick a stage :: stage idx %d", (picked->second)->_stageIndex);
    
    return (picked->second);

}

cocos2d::Color3B CStageDataManager::getCurrentBulletColor()
{
    auto stage = CStageDataManager::Instance()->getStageByIndex(GVALUE->CURRENT_PLANET);
    auto list  = stage->_stageDataList;
    auto level = GVALUE->STAGE_LEVEL;
    if(list.size() <= level)
        return Color3B::WHITE;
    
    return list.at(level)._bulletColor;
}

cocos2d::Color3B CStageDataManager::getCurrentBGTopColor()
{
    auto stage = CStageDataManager::Instance()->getStageByIndex(GVALUE->CURRENT_PLANET);
    auto list  = stage->_stageDataList;
    auto level = GVALUE->STAGE_LEVEL;
    if(list.size() <= level)
        return Color3B(0, 4, 40);
    
    return list.at(level)._bgColorTop;
}

cocos2d::Color3B CStageDataManager::getCurrentBGBottomColor()
{
    auto stage = CStageDataManager::Instance()->getStageByIndex(GVALUE->CURRENT_PLANET);
    auto list  = stage->_stageDataList;
    auto level = GVALUE->STAGE_LEVEL;
    if(list.size() <= level)
        return Color3B(0, 63, 110);
    
    return list.at(level)._bgColorBottom;
}

STAGE_DATA CStageDataManager::getStageDataByIndex(int stageIndex, int index)
{
    auto stageData = this->getStageByIndex(stageIndex);
    
    auto data = stageData->_stageDataList.find(index);
    if(data == stageData->_stageDataList.end()) {
        CCLOG("Wrong stage idx : %d index : %d", stageIndex, index);
        CCASSERT(false, "Wrong stage idx, index");
        return STAGE_DATA();
    }
    return data->second;
}

//STAGE_DATA CStageDataManager::getLastSavedPoint()
//{
//    auto currentStageData = this->getStageByIndex(GVALUE->CURRENT_PLANET);
//    auto savePointList = currentStageData->_savePointList;
//    if(currentStageData->_stageDataList.size() <= 0){
//        CCLOG("Error - The size of _stageDataList is 0 plant : %d", GVALUE->CURRENT_PLANET);
//        CCASSERT(false, "The size of _stageDataList is 0");
//        return STAGE_DATA();
//    }
//    
//    auto passedSavePoints = DATA_MANAGER_UTILS::getNonPtrListByFunc([=](int index){
//        return index <= GVALUE->STAGE_LEVEL;
//    }, savePointList);
//    
//    if(passedSavePoints.size() <= 0)
//        return currentStageData->_stageDataList.at(0);
//    
//    auto lastSavedPointIndex = passedSavePoints.at(passedSavePoints.size() -1);
//    return this->getStageDataByIndex(GVALUE->CURRENT_PLANET, lastSavedPointIndex);
//}

STAGE_DATA CStageDataManager::getSavedPoint()
{
    return this->getStageDataByIndex(GVALUE->CURRENT_PLANET, GVALUE->LAST_SAVED_POINT);
}

void CStageDataManager::setSavePoint()
{
    auto stageData = this->getStageDataByIndex(GVALUE->CURRENT_PLANET, GVALUE->STAGE_LEVEL);
    if(stageData._isSavePoint && GVALUE->LAST_SAVED_POINT != GVALUE->STAGE_LEVEL){
        GVALUE->LAST_SAVED_POINT = GVALUE->STAGE_LEVEL;
//        GVALUE->REVIVE_COUNT = 0;
        GSAVED->CopyData(GVALUE);
    }
}
