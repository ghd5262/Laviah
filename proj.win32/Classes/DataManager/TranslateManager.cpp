#include "TranslateManager.hpp"
#include "../Common/HSHUtility.h"
#include "../json/json.h"

using namespace cocos2d;
namespace TRANSLATE_DEFINE {
    static const std::string FILENAME_LANGUAGELIST = "languageList.json";
}

CTranslateManager::CTranslateManager()
: m_CurrentSelectLanguage(Application::getInstance()->getCurrentLanguageCode())
{
    this->initLanguageList(m_LanguageList, TRANSLATE_DEFINE::FILENAME_LANGUAGELIST);
}

CTranslateManager* CTranslateManager::Instance()
{
    static CTranslateManager instance;
    return &instance;
}

void CTranslateManager::initLanguageList(LANGUAGE_LIST &list, std::string fileName)
{
    Json::Value root;
    Json::Reader reader;
    
    std::string file = FileUtils::getInstance()->fullPathForFilename(fileName);
    std::string fileData = FileUtils::getInstance()->getStringFromFile(file);
    size_t pos = fileData.rfind("}");
    fileData = fileData.substr(0, pos + 1);
    
    bool parsingSuccessful = reader.parse(fileData, root);
    if (!parsingSuccessful)
    {
        CCASSERT(false, MakeString("parser failed : \n %s", fileData.c_str()).c_str());
        return;
    }
    CCLOG("Language List JSON : \n %s\n", fileData.c_str());
    
    const Json::Value languageArray = root["languageList"];
    
    for (unsigned int count = 0; count < languageArray.size(); ++count)
    {
        const Json::Value language = languageArray[count];
        auto key                = language["key"].asString();
        auto contentFileName    = language["file"].asString();
        
        Json::Value  contentRoot;
        Json::Reader contentReader;
        std::string contentFile        = FileUtils::getInstance()->fullPathForFilename(contentFileName);
        std::string contentFileData    = FileUtils::getInstance()->getStringFromFile(contentFile);
        size_t contentPos = contentFileData.rfind("}");
        contentFileData = contentFileData.substr(0, contentPos + 1);
        
        bool lParsingSuccessful = contentReader.parse(contentFileData, contentRoot);
        if (!lParsingSuccessful)
        {
            CCASSERT(false, MakeString("parser failed : \n %s", contentFileData.c_str()).c_str());
            return;
        }
        CCLOG("Content List JSON : \n %s\n", contentFileData.c_str());
        
        const Json::Value languageKey  = contentRoot["language"];
        const Json::Value contentArray = contentRoot["contents"];
        
        auto contentList = this->createContentList(languageKey.asString());
        
        for(int index = 0; index < contentArray.size(); index++)
        {
            const Json::Value content = contentArray[index];
            auto contentKey      = content["key"].asString();
            auto contentValue    = content["content"].asString();
            this->addContentToList(contentList, contentKey, contentValue);
        }
    }
}

void CTranslateManager::addContentToList(CONTENT_LIST *list, std::string key, std::string content)
{
    auto iter = list->find(key.c_str());
    if(iter == list->end())
    {
        list->emplace(std::pair<std::string, std::string>(key, content));
    }
    else{
        CCLOG("Content key was duplicated : %s", key.c_str());
        CCASSERT(false, StringUtils::format("Content key was duplicated : %s", key.c_str()).c_str());
    }
}

CTranslateManager::CONTENT_LIST* CTranslateManager::createContentList(std::string key)
{
    auto iter = m_LanguageList.find(key.c_str());
    if(iter == m_LanguageList.end())
    {
        auto list = new CONTENT_LIST();
        m_LanguageList.emplace(std::pair<std::string, CONTENT_LIST*>(key, list));
        return list;
    }
    else{
        CCLOG("Language key was duplicated : %s", key.c_str());
        CCASSERT(false, StringUtils::format("Language key was duplicated : %s", key.c_str()).c_str());
    }
}

/*
 "zh" LanguageType::CHINESE;
 "en" LanguageType::ENGLISH;
 "fr" LanguageType::FRENCH;
 "it" LanguageType::ITALIAN;
 "de" LanguageType::GERMAN;
 "es" LanguageType::SPANISH;
 "nl" LanguageType::DUTCH;
 "ru" LanguageType::RUSSIAN;
 "ko" LanguageType::KOREAN;
 "ja" LanguageType::JAPANESE;
 "hu" LanguageType::HUNGARIAN;
 "pt" LanguageType::PORTUGUESE;
 "ar" LanguageType::ARABIC;
 "nb" LanguageType::NORWEGIAN;
 "pl" LanguageType::POLISH;
 "tr" LanguageType::TURKISH;
 "uk" LanguageType::UKRAINIAN;
 "ro" LanguageType::ROMANIAN;
 "bg" LanguageType::BULGARIAN;
 */
std::string CTranslateManager::getContent(std::string key)
{
    auto LanguageList = m_LanguageList.find(m_CurrentSelectLanguage.c_str());
    if(LanguageList == m_LanguageList.end())
        LanguageList = m_LanguageList.find("en");
    
    auto list = LanguageList->second;
    auto content = list->find(key.c_str());
    
    std::string contentStr = "";
    if(content != list->end())
        contentStr = content->second;
    
    return contentStr;
}
