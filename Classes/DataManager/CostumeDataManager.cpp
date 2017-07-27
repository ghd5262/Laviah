#include "CostumeDataManager.hpp"
#include "UserDataManager.h"
#include "DataManagerUtils.h"
#include "CharacterDataManager.h"

using namespace cocos2d;

CCostumeDataManager::CCostumeDataManager()
{
    initWithJson(m_CostumeList, "costumeList.json");
}

CCostumeDataManager::~CCostumeDataManager()
{
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_CostumeList);
}

CCostumeDataManager* CCostumeDataManager::Instance()
{
    static CCostumeDataManager instance;
    return &instance;
}

void CCostumeDataManager::initWithJson(COSTUME_LIST &list, std::string fileName)
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
    CCLOG("costume JSON : \n %s\n", clearString.c_str());
    
    const Json::Value array = root["costumes"];
    for(auto costume : array)
        this->addCostumeToList(costume);
}

void CCostumeDataManager::addCostumeToList(const Json::Value& json)
{
    auto data      = new COSTUME();
    
    data->_index          = json["index"].asInt();
    data->_characterIndex = json["characterIndex"].asInt();
    data->_enable         = json["enable"].asBool();
    data->_name           = StringUtils::format(COSTUME_DEFINE::NAME.c_str(),         data->_index);
    data->_texture        = StringUtils::format(COSTUME_DEFINE::TEXTURE.c_str(),      data->_index);
    data->_texture_600    = StringUtils::format(COSTUME_DEFINE::TEXTURE_600.c_str(),  data->_index);
    data->_texturePack    = StringUtils::format(COSTUME_DEFINE::TEXTURE_PACK.c_str(), data->_index);
    
    CCharacterDataManager::addTexturePackToCache(data->_texturePack);
    
    m_CostumeList.emplace(std::pair<int, const COSTUME*>(data->_index, data));
}

const COSTUME* CCostumeDataManager::getCostumeByIndex(int index) const
{
    auto data = m_CostumeList.find(index);
    if(data == m_CostumeList.end()) {
        CCLOG("Wrong costume index : %d", index);
        CCASSERT(false, "Wrong costume index");
        return nullptr;
    }
    return data->second;
}

COSTUME_LIST CCostumeDataManager::getCostumeListByCharacter(int index)
{
    return DATA_MANAGER_UTILS::getMapByFunc([=](const COSTUME* data){
        if(data->_index == 0) return true;
        return data->_characterIndex == index;
    }, m_CostumeList);
}

const COSTUME* CCostumeDataManager::getNewRandomCostume()
{
    auto newList = getNonCollectedCostumeList();
    return getNewRandomCostumeFromList(newList);
}

const COSTUME* CCostumeDataManager::getCurCostumeByCharacter(int index)
{
    auto currentCostume = CUserDataManager::Instance()->getUserData_ParamData(USERDATA_KEY::CHARACTER_COSTUME, index, USERDATA_PARAM_COSTUME::COSTUME_INDEX, 0);
    return this->getCostumeByIndex(currentCostume);
}

const COSTUME* CCostumeDataManager::getNewRandomCostumeFromList(COSTUME_LIST &list)
{
    auto size = list.size();
    if (size <= 0) {
        CCLOG("There is no costume anymore.");
        return nullptr;
    }
    auto picked = list.begin();
    std::advance(picked, random<int>(0, int(list.size()-1)));
    
    CCLOG("Pick a costume :: idx %d name %s",
          (picked->second)->_index,
          (picked->second)->_name.c_str());
    
    return (picked->second);
}

COSTUME_LIST CCostumeDataManager::getNonCollectedCostumeList()
{
    auto userDataMng = CUserDataManager::Instance();
    
    return DATA_MANAGER_UTILS::getMapByFunc([=](const COSTUME* data){
        
        if (userDataMng->getUserData_IsItemHave(USERDATA_KEY::COSTUME_LIST, data->_index)) return false;
        
        return true;
    }, m_CostumeList);
}
