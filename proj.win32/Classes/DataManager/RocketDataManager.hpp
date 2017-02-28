#pragma once
#include "../Common/HSHUtility.h"
#include "../json/json.h"
#include <vector>
#include <map>

namespace ROCKET_DEFINE {
    static const std::string NAME         = "ROCKET_NAME_%d";
    static const std::string TEXTURE_NAME = "rocket_%d.png";
}

struct sROCKET_PARAM{
    int _idx;
    std::string _textureName;
    std::string _name;
    
    sROCKET_PARAM()
    : _idx(0)
    , _textureName("")
    , _name(""){}
};

typedef std::map<int, const sROCKET_PARAM*> ROCKET_LIST;
typedef std::function<bool(const sROCKET_PARAM*)> ROCKET_PICK;
class CRocketDataManager
{
public:
    static CRocketDataManager* Instance();
    
    void PrintRocketInfo(int index);
    bool NonCollectedRocketExist();
    
    //getter & setter
    const sROCKET_PARAM* getRocketByIndex(int index) const;
    const sROCKET_PARAM* getNewRandomRocket();
    ROCKET_LIST getRocketList(){ return m_RocketList; };

private:
    void initWithJson(ROCKET_LIST &list, std::string fileName);
    void addRocketToList(const Json::Value& data);
    void addRocketTexturePack(const Json::Value& array);
    void addTexturePackToCache(std::string fileName);
    const Json::Value initRocketWithDefaultValue(std::string key, const Json::Value data);
    const sROCKET_PARAM* getNewRandomRocketFromList(ROCKET_LIST &list);
    ROCKET_LIST getNonCollectedRocketList();
    
    CRocketDataManager();
    virtual ~CRocketDataManager();
    
private:
    ROCKET_LIST m_RocketList;
    Json::Value m_RocketDefaultSet;
};