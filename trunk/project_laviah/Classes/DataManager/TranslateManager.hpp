#pragma once
#include <iostream>
#include "cocos2d.h"
#include <map>
class CTranslateManager{
	typedef std::vector<std::string> LANGUAGE_KEY_LIST;
    typedef std::map<std::string, std::string>   CONTENT_LIST;
    typedef std::map<std::string, CONTENT_LIST*> LANGUAGE_LIST;
    typedef std::function<void()> TRANSLATE_LISTENER;
    typedef std::vector<TRANSLATE_LISTENER> LISTENER_LIST;
public:
    static CTranslateManager* Instance();
    std::string getContent(std::string key);
    std::string getContentByLanguageKey(std::string key, std::string languageKey);
    LANGUAGE_KEY_LIST getLanguageKeyList() const { return m_LanguageKeyList; };
    void ChangeLanguage(std::string languageKey);
    void addTranslateEventListener(const TRANSLATE_LISTENER& listener);
    std::string getCurrentDate(bool timeEnable = true);
    
    CC_SYNTHESIZE_READONLY(std::string, m_CurrentSelectLanguage, CurrentSelectLanguage);
    
private:
    void initLanguageList(LANGUAGE_LIST &list, std::string fileName);
	void createContentList(std::string languageKey);
	void addContentToList(CONTENT_LIST* list, std::string contentkey, std::string content);
	CONTENT_LIST* getContentList(std::string languageKey);

    CTranslateManager();
    ~CTranslateManager();
    
public:
	LANGUAGE_KEY_LIST  m_LanguageKeyList;
    LANGUAGE_LIST m_LanguageList;
    LISTENER_LIST m_ListenerList;
};

#define TRANSLATE(key) CTranslateManager::Instance()->getContent(key)