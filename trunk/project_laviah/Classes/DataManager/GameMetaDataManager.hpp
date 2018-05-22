#pragma once
#include "../Common/HSHUtility.h"
#include "../json/json.h"

class CGameMetaDataManager
{
public:
    static CGameMetaDataManager* Instance();
    Json::Value getMetaDataByKey(std::string key);
    
private:
    void initWithJson(std::string fileName);
    
    CGameMetaDataManager();
    virtual ~CGameMetaDataManager();
    
private:
    Json::Value m_GameMetaData;
};

#define META_DATA(key) CGameMetaDataManager::Instance()->getMetaDataByKey(key)
