#pragma once
#include <iostream>
#include "cocos2d.h"
#include <map>
class CTranslateManager{
    typedef std::map<std::string, std::string>   CONTENT_LIST;
    typedef std::map<std::string, CONTENT_LIST*> LANGUAGE_LIST;
    
public:
    static CTranslateManager* Instance();
    std::string getContent(std::string key);
    
    CC_SYNTHESIZE(std::string, m_CurrentSelectLanguage, CurrentSelectLanguage);
    
private:
    void initLanguageList(LANGUAGE_LIST &list, std::string fileName);
    void addContentToList(CONTENT_LIST *list, std::string key, std::string content);
    CONTENT_LIST* createContentList(std::string key);

    CTranslateManager();
    ~CTranslateManager(){};
    
public:
    LANGUAGE_LIST m_LanguageList;
};

#define TRANSLATE(key) CTranslateManager::Instance()->getContent(key)