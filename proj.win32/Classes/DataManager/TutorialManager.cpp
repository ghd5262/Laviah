#include "TutorialManager.hpp"
#include "DataManagerUtils.h"
#include "../Common/HSHUtility.h"
#include "../json/json.h"

using namespace cocos2d;
CTutorialManager::CTutorialManager(){}

CTutorialManager::~CTutorialManager(){
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_TutorialList);
}

CTutorialManager* CTutorialManager::Instance()
{
    static CTutorialManager instance;
    return &instance;
}

void CTutorialManager::RunAtFirst(std::string key)
{
    auto iter = m_TutorialList.find(key.c_str());
    if(iter == m_TutorialList.end())
        CCASSERT(false, StringUtils::format("There is no key for data : %s", key.c_str()).c_str());
    
    auto tutorial = iter->second;
    for(auto step : *tutorial)
    {
        auto listener = step.getStartListener();
        if(listener) listener();
    }
}

void CTutorialManager::RunByIndex(std::string key, int index)
{
    
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
