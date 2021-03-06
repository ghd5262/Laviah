#include "PlanetDataManager.hpp"
#include "UserDataManager.h"
#include "DataManagerUtils.h"
#include "CharacterDataManager.h"

using namespace cocos2d;

CPlanetDataManager::CPlanetDataManager()
{
    initWithJson(m_PlanetList, "planetList.json");
}

CPlanetDataManager::~CPlanetDataManager()
{
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_PlanetList);
}

CPlanetDataManager* CPlanetDataManager::Instance()
{
    static CPlanetDataManager instance;
    return &instance;
}

void CPlanetDataManager::initWithJson(PLANET_LIST &list, std::string fileName)
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
    CCLOG("PLANET JSON : \n %s\n", clearString.c_str());
    
    const Json::Value array = root["planets"];
    for(auto planet : array)
        this->addPlanetToList(planet);
}

void CPlanetDataManager::addPlanetToList(const Json::Value& json)
{
    auto data      = new PLANET();
    
    data->_index                = json["index"].asInt();
    data->_openLevel            = json["openLevel"].asInt();
    data->_leaderboard          = json["leaderboard"].asString();
    data->_enable               = json["enable"].asBool();
    data->_name                 = StringUtils::format(PLANET_DEFINE::NAME.c_str(),           data->_index);
    data->_planetTexture        = StringUtils::format(PLANET_DEFINE::PLANET_TEXTURE.c_str(), data->_index);
    data->_normalBulletTexture  = StringUtils::format(PLANET_DEFINE::NORMAL_BULLET.c_str(),  data->_index);
    data->_normalMissileTexture = StringUtils::format(PLANET_DEFINE::NORMAL_MISSILE.c_str(), data->_index);
    data->_stickBulletTexture   = StringUtils::format(PLANET_DEFINE::STAND_BULLET.c_str(),   data->_index);
    data->_standBulletTexture   = StringUtils::format(PLANET_DEFINE::STICK_BULLET.c_str(),   data->_index);
    data->_texturePack          = StringUtils::format(PLANET_DEFINE::TEXTURE_PACK.c_str(),   data->_index);
    
    CCharacterDataManager::addTexturePackToCache(data->_texturePack);
    m_PlanetList.emplace(std::pair<int, const PLANET*>(data->_index, data));
}

const PLANET* CPlanetDataManager::getPlanetByIndex(int index) const
{
    auto data = m_PlanetList.find(index);
    if(data == m_PlanetList.end()) {
        CCLOG("Wrong PLANET index : %d", index);
        CCASSERT(false, "Wrong PLANET index");
        return nullptr;
    }
    return data->second;
}

const PLANET* CPlanetDataManager::getCurPlanet() const
{
    auto currentPlanet = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::PLANET);
    return this->getPlanetByIndex(currentPlanet);
}

int CPlanetDataManager::getPlanetSavedData(int param)
{
    auto currentPlanet = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::PLANET);
    return CUserDataManager::Instance()->getUserData_ParamData(USERDATA_KEY::PLANET_LIST, currentPlanet,
                                                               param, 0);
}

void CPlanetDataManager::setPlanetSavedData(int param, int value)
{
    auto currentPlanet = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::PLANET);
    CUserDataManager::Instance()->setUserData_ItemParam(USERDATA_KEY::PLANET_LIST, currentPlanet,
                                                        param, value);
}

void CPlanetDataManager::setPlanetForceOpen(int index)
{
    auto data = this->getPlanetByIndex(index);
    auto casted = const_cast<PLANET*>(data);
    casted->_forceOpen = true;
}

bool CPlanetDataManager::IsPlanetOpened(int index)
{
    if(index > 0){
        auto data = this->getPlanetByIndex(index);
        auto enabled = data->_enable;
        auto userDataManager = CUserDataManager::Instance();
        auto cleared = (100 <= userDataManager->getUserData_ParamData(USERDATA_KEY::PLANET_LIST, index - 1,
                                                                     PARAM_PLANET::STAGE_PERCENT, 0));
        auto open = (100 <= userDataManager->getUserData_ParamData(USERDATA_KEY::PLANET_LIST, index,
                                                                   PARAM_PLANET::STAGE_PERCENT, 0));
        auto forceOpened = data->_forceOpen;
        return (enabled && (cleared || forceOpened || open));
    }
    return true;
}
