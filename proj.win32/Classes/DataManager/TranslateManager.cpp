#include "TranslateManager.hpp"
#include "DataManagerUtils.h"
#include "UserDataManager.h"
#include "../Common/HSHUtility.h"
#include "../json/json.h"

using namespace cocos2d;
namespace TRANSLATE_DEFINE {
    static const std::string FILENAME_LANGUAGE = "language.json";
}

CTranslateManager::CTranslateManager()
: m_CurrentSelectLanguage(Application::getInstance()->getCurrentLanguageCode())
{
	this->initLanguageList(m_LanguageList, TRANSLATE_DEFINE::FILENAME_LANGUAGE);
}

CTranslateManager::~CTranslateManager()
{
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_LanguageList);
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
	CCLOG("Language JSON : \n %s\n", fileData.c_str());

	const Json::Value languageKeyList = root["supports"];
	for (auto key : languageKeyList){
		auto languageKey = key.asString();
		m_LanguageKeyList.emplace_back(languageKey);
		this->createContentList(languageKey);
	}

	const Json::Value contentList = root["contents"];
	for (auto content : contentList)
	{
		auto contentKey = content["key"].asString();
		for (auto languageKey : m_LanguageKeyList)
		{
			this->addContentToList(getContentList(languageKey), contentKey, content[languageKey.c_str()].asString());
		}
	}
    
    auto savedIndex = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::CURRENT_LANGUAGE);
    if(savedIndex != -1 && savedIndex < m_LanguageKeyList.size())
        m_CurrentSelectLanguage = m_LanguageKeyList.at(savedIndex);
}

void CTranslateManager::createContentList(std::string languageKey)
{
	auto iter = m_LanguageList.find(languageKey.c_str());
    if(iter == m_LanguageList.end())
    {
        auto list = new CONTENT_LIST();
		m_LanguageList.emplace(std::pair<std::string, CONTENT_LIST*>(languageKey, list));
    }
    else{
		CCLOG("Language key was duplicated : %s", languageKey.c_str());
		CCASSERT(false, StringUtils::format("Language key was duplicated : %s", languageKey.c_str()).c_str());
    }
}

void CTranslateManager::addContentToList(CONTENT_LIST* list, std::string contentkey, std::string content)
{
	auto iter = list->find(contentkey.c_str());
	if (iter == list->end())
	{
		list->emplace(std::pair<std::string, std::string>(contentkey, content));
	}
	else{
		CCLOG("Content key was duplicated : %s", contentkey.c_str());
		CCASSERT(false, StringUtils::format("Content key was duplicated : %s", contentkey.c_str()).c_str());
	}
}

CTranslateManager::CONTENT_LIST* CTranslateManager::getContentList(std::string languageKey)
{
	auto list = m_LanguageList.find(languageKey);
	if (list == m_LanguageList.end())
	{
		CCLOG("There is no [ %s ] key in the list.", languageKey.c_str());
		CCASSERT(false, StringUtils::format("There is no [ %s ] key in the list.", languageKey.c_str()).c_str());
	}
	return list->second;
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
    return getContentByLanguageKey(key, m_CurrentSelectLanguage);
}

std::string CTranslateManager::getContentByLanguageKey(std::string key,
                                                       std::string languageKey)
{
    auto LanguageList = m_LanguageList.find(languageKey.c_str());
    if(LanguageList == m_LanguageList.end())
        LanguageList = m_LanguageList.find("en");
    
    auto list = LanguageList->second;
    auto content = list->find(key.c_str());
    
    std::string contentStr = "";
    if(content != list->end())
        contentStr = content->second;
    
    return contentStr;
}
