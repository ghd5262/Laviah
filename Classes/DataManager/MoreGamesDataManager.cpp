#include "MoreGamesDataManager.hpp"
#include "UserDataManager.h"
#include "DataManagerUtils.h"
#include "../SDKUtil/SDKUtil.h"

using namespace cocos2d;

CMoreGamesDataManager::CMoreGamesDataManager()
{
    initWithJson(m_MoreGamesList, "moreGamesList.json");
}

CMoreGamesDataManager::~CMoreGamesDataManager()
{
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_MoreGamesList);
}

CMoreGamesDataManager* CMoreGamesDataManager::Instance()
{
    static CMoreGamesDataManager instance;
    return &instance;
}

void CMoreGamesDataManager::initWithJson(MOREGAMES_LIST &list, std::string fileName)
{
    Json::Value root;
    Json::Reader reader;
    
    std::string strMoreGamesList = FileUtils::getInstance()->fullPathForFilename(fileName);
    std::string MoreGamesListClearData = FileUtils::getInstance()->getStringFromFile(strMoreGamesList);
    size_t pos = MoreGamesListClearData.rfind("}");
    MoreGamesListClearData = MoreGamesListClearData.substr(0, pos + 1);
    bool parsingSuccessful = reader.parse(MoreGamesListClearData, root);
    if (!parsingSuccessful)
    {
        CCASSERT(false, MakeString("parser failed : \n %s", MoreGamesListClearData.c_str()).c_str());
        return;
    }
    CCLOG("strMoreGamesList JSON : \n %s\n", MoreGamesListClearData.c_str());
    
    const Json::Value itemArray = root["moreGames"];
    for (unsigned int itemCount = 0; itemCount < itemArray.size(); ++itemCount)
    {
        const Json::Value valueItem = itemArray[itemCount];
        this->addMoreGamesToList(valueItem);
    }
}

void CMoreGamesDataManager::addMoreGamesToList(const Json::Value& data)
{
    sMOREGAMES_PARAM* param = new sMOREGAMES_PARAM();
    
    param->_idx             = data["index"].asInt();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    param->_url             = data["androidUrl"].asString();
#else
    param->_url             = data["iosUrl"].asString();
#endif
    param->_textureName     = StringUtils::format(MOREGAMES_DEFINE::TEXTURE_NAME.c_str(), param->_idx);
    
    m_MoreGamesList.emplace(std::pair<int, const sMOREGAMES_PARAM*>(param->_idx, param));
}

const sMOREGAMES_PARAM* CMoreGamesDataManager::getMoreGamesByIndex(int index) const
{
    auto data = m_MoreGamesList.find(index);
    if(data == m_MoreGamesList.end()) {
        CCLOG("Wrong MoreGames index : %d", index);
        CCASSERT(false, "Wrong MoreGames index");
        return nullptr;
    }
    return data->second;
}
