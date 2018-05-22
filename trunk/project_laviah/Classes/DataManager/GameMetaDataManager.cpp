#include "GameMetaDataManager.hpp"
#include "DataManagerUtils.h"

using namespace cocos2d;

CGameMetaDataManager::CGameMetaDataManager()
{
    initWithJson("gameMetaData.json");
}

CGameMetaDataManager::~CGameMetaDataManager(){}

CGameMetaDataManager* CGameMetaDataManager::Instance()
{
    static CGameMetaDataManager instance;
    return &instance;
}

void CGameMetaDataManager::initWithJson(std::string fileName)
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
    CCLOG("META DATA JSON : \n %s\n", clearString.c_str());
    
    auto array = root["gameMetaData"];
    for(auto data : array)
        m_GameMetaData = data;
}

Json::Value CGameMetaDataManager::getMetaDataByKey(std::string key)
{
    auto data = m_GameMetaData[key];
    if(data.isNull()){
        CCLOG("Meta data is null %s", key.c_str());
        CCASSERT(false, "Meta data is null");
    }
    
    return data;
}
