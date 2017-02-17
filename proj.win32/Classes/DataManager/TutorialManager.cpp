#include "TutorialManager.hpp"
#include "DataManagerUtils.h"
#include "../Common/HSHUtility.h"
#include "../json/json.h"

using namespace cocos2d;
CTutorialManager::CTutorialManager()
: m_CurrentIndex(0)
, m_CurrentKey(""){}

CTutorialManager::~CTutorialManager(){
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_TutorialList);
}

CTutorialManager* CTutorialManager::Instance()
{
    static CTutorialManager instance;
    return &instance;
}

void CTutorialManager::addStepToByTutorialKey(std::string key, STEP step)
{
    auto iter = m_TutorialList.find(key.c_str());
    TUTORIAL* tutorial = nullptr;
    
    // if there is no tutorial for the key.
    if (iter == m_TutorialList.end())   tutorial = this->addNewTutorial(key);
    else                                tutorial = iter->second;
    
    this->addStepToTutorial(tutorial, step);
}

void CTutorialManager::RunAtFirst(std::string key)
{
    this->RunByIndex(key, 0);
}

void CTutorialManager::RunByIndex(std::string key, int index)
{
    if(key == "") return;
    
    auto assertion = [=](std::string content){
        content += (std::string(" : ") + key);
        CCLOG("%s", content.c_str());
        CCASSERT(false, content.c_str());
    };
    
    auto iter = m_TutorialList.find(key.c_str());
    if(iter == m_TutorialList.end()) assertion("There is no key for data");
    
    auto tutorial = iter->second;
    if(tutorial->size() <= 0)        assertion("There is no tutorial step in");
    if(tutorial->size() <= index){
        this->Clear();
        return;
    }
    
    auto listener = (tutorial->at(index)).getStartListener();
    if(listener) listener();
    
    m_CurrentIndex++;
}

void CTutorialManager::End(std::string key, int index)
{
    if(key   != "") m_CurrentKey   = key;
    if(index != 0 ) m_CurrentIndex = index;
    
    // run next step
    this->RunByIndex(m_CurrentKey, m_CurrentIndex);
}

void CTutorialManager::Clear()
{
    m_CurrentIndex = 0;
    m_CurrentKey = "";
}

CTutorialManager::TUTORIAL* CTutorialManager::addNewTutorial(std::string key)
{
    auto tutorial = new TUTORIAL();
    m_TutorialList.emplace(std::pair<std::string, TUTORIAL*>(key, tutorial));
    return tutorial;
}

void CTutorialManager::addStepToTutorial(TUTORIAL* tutorial, STEP step)
{
    if(tutorial->size() <= step.getIndex())
        tutorial->resize(step.getIndex() + 1);
    
    tutorial->at(step.getIndex()) = step;
}
