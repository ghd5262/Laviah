#include "RocketDataManager.hpp"
#include "UserDataManager.h"
#include "DataManagerUtils.h"
#include "CharacterDataManager.h"

using namespace cocos2d;

CRocketDataManager::CRocketDataManager()
{
    initWithJson(m_RocketList, "rocketList.json");
}

CRocketDataManager::~CRocketDataManager()
{
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_RocketList);
}

CRocketDataManager* CRocketDataManager::Instance()
{
    static CRocketDataManager instance;
    return &instance;
}

void CRocketDataManager::initWithJson(ROCKET_LIST &list, std::string fileName)
{
    Json::Value root;
    Json::Reader reader;
    
    std::string strRocketList = FileUtils::getInstance()->fullPathForFilename(fileName);
    std::string RocketListClearData = FileUtils::getInstance()->getStringFromFile(strRocketList);
    size_t pos = RocketListClearData.rfind("}");
    RocketListClearData = RocketListClearData.substr(0, pos + 1);
    bool parsingSuccessful = reader.parse(RocketListClearData, root);
    if (!parsingSuccessful)
    {
        CCASSERT(false, MakeString("parser failed : \n %s", RocketListClearData.c_str()).c_str());
        return;
    }
    CCLOG("strRocketList JSON : \n %s\n", RocketListClearData.c_str());
    
    const Json::Value rocketPackArray = root["rocketPackList"];
    this->addRocketTexturePack(rocketPackArray);
    
    m_RocketDefaultSet = root["rocketDefaultSet"];
    
    const Json::Value itemArray = root["rockets"];
    for (unsigned int itemCount = 0; itemCount < itemArray.size(); ++itemCount)
    {
        const Json::Value valueItem = itemArray[itemCount];
        this->addRocketToList(valueItem);
    }
}

void CRocketDataManager::addRocketTexturePack(const Json::Value& array)
{
    for (unsigned int itemCount = 0; itemCount < array.size(); ++itemCount)
    {
        auto normalTexturePackName = array[itemCount].asString();
        CCharacterDataManager::addTexturePackToCache(normalTexturePackName);
    }
}

void CRocketDataManager::addRocketToList(const Json::Value& data)
{
    sROCKET_PARAM* param = new sROCKET_PARAM();
    
    param->_idx             = data["index"].asInt();
    
//    auto initData = [=](std::string key){
//        return this->initRocketWithDefaultValue(key, data[key.c_str()]);
//    };
//    param->_textureName     = initData("textureName").asString();
    
    param->_textureName     = StringUtils::format(ROCKET_DEFINE::TEXTURE_NAME.c_str(), param->_idx);
    param->_name			= StringUtils::format(ROCKET_DEFINE::NAME.c_str(), param->_idx);
    
    m_RocketList.emplace(std::pair<int, const sROCKET_PARAM*>(param->_idx, param));
}

const Json::Value CRocketDataManager::initRocketWithDefaultValue(std::string key,
                                                                 const Json::Value data)
{
    if(data.isNull())
    {
        return m_RocketDefaultSet[key.c_str()];
    }
    return data;
}

const sROCKET_PARAM* CRocketDataManager::getRocketByIndex(int index) const
{
    auto data = m_RocketList.find(index);
    if(data == m_RocketList.end()) {
        CCLOG("Wrong Rocket index : %d", index);
        CCASSERT(false, "Wrong Rocket index");
        return nullptr;
    }
    return data->second;
}

const sROCKET_PARAM* CRocketDataManager::getNewRandomRocket()
{
    auto newList = getNonCollectedRocketList();
    return getNewRandomRocketFromList(newList);
}

const sROCKET_PARAM* CRocketDataManager::getCurRocket()
{
    auto currentRocket = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::ROCKET);
    return this->getRocketByIndex(currentRocket);
}

const sROCKET_PARAM* CRocketDataManager::getNewRandomRocketFromList(ROCKET_LIST &list)
{
    auto size = list.size();
    if (size <= 0) {
        CCLOG("There is no Rocket anymore.");
        return nullptr;
    }
    auto picked = list.begin();
    std::advance(picked, random<int>(0, int(list.size()-1)));
    
    CCLOG("Pick a Rocket :: idx %d name %s",
          (picked->second)->_idx,
          (picked->second)->_name.c_str());
    
    return (picked->second);
}

ROCKET_LIST CRocketDataManager::getNonCollectedRocketList()
{
    auto userDataMng = CUserDataManager::Instance();
    
    return DATA_MANAGER_UTILS::getMapByFunc([=](const sROCKET_PARAM* data){
        
        if (userDataMng->getUserData_IsItemExist(USERDATA_KEY::ROCKET_LIST, data->_idx)) return false;
        
        return true;
    }, m_RocketList);
}


void CRocketDataManager::PrintRocketInfo(int index)
{
    auto Rocket = m_RocketList.at(index);
    CCLOG("=============Rocket Info==============");
    CCLOG("Index          : %d", Rocket->_idx);
    CCLOG("Texture Name   : %s", Rocket->_textureName.c_str());
    CCLOG("Name           : %s", TRANSLATE(Rocket->_name).c_str());
    CCLOG("======================================");
}
