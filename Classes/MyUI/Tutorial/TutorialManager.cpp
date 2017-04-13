#include "TutorialManager.hpp"
#include "TutorialStep.hpp"
#include "../../MyUI/MyButton.h"

using namespace cocos2d;

void assertion(std::string content){
    CCLOG("%s", content.c_str());
    CCASSERT(false, content.c_str());
};

CTutorialManager* CTutorialManager::m_Instance = nullptr;
CTutorialManager::CTutorialManager()
: m_CurrentTutorial(nullptr)
, m_IsRunning(false)
, m_CurrentStepIndex(0){}

CTutorialManager::~CTutorialManager(){
    for(auto tutorial : m_TutorialList)
        for(auto tutorialObj : *tutorial.second)
            tutorialObj->release();
    
    m_Instance = nullptr;
}

CTutorialManager* CTutorialManager::Instance()
{
    if(m_Instance != nullptr) return m_Instance;
    
    m_Instance = new(std::nothrow) CTutorialManager();
    if (m_Instance && m_Instance->init())
    {
        m_Instance->autorelease();
        return m_Instance;
    }
    else
    {
        delete m_Instance;
        m_Instance = NULL;
        return NULL;
    }
}

bool CTutorialManager::init()
{
    if (!Node::init()) return false;
    this->scheduleUpdate();
    this->setContentSize(_director->getWinSize());
    return true;
}

void CTutorialManager::update(float delta)
{
    if(!m_IsRunning) return;
    
    // call update function of current step of current tutorial.
    auto currentStep = m_CurrentTutorial->at(m_CurrentStepIndex);
    currentStep->Update(delta);
}

void CTutorialManager::addTutorial(std::string key, CTutorialStep* newTutorial)
{
    auto iter = m_TutorialList.find(key);
    TUTORIAL* tutorial = nullptr;
    if(iter == m_TutorialList.end())    tutorial = this->addNewTutorialByKey(key);
    else                                tutorial = this->getTutorialByKey(key);
    
    newTutorial->retain();
    tutorial->emplace_back(newTutorial);
}

void CTutorialManager::ChangeTutorial(std::string key)
{
    if(key == "") return;
    
    // call end listener of current tutorial.
    this->stepEnd(key, m_CurrentStepIndex);
    m_CurrentTutorial = this->getTutorialByKey(key);
    m_CurrentStepIndex = 0;
    
    this->stepBegin(key, m_CurrentStepIndex);

    m_IsRunning = true;
}

void CTutorialManager::ChangeStep(int index)
{
    auto currentStep = m_CurrentTutorial->at(m_CurrentStepIndex);
    auto currentKey  = currentStep->getTutorialKey();
    
    this->stepEnd(currentKey, m_CurrentStepIndex);
    if(m_CurrentTutorial->size() <= index) {
        this->clear();
        return;
    }
    m_CurrentStepIndex = index;
    this->stepBegin(currentKey, m_CurrentStepIndex);
}

void CTutorialManager::NextStep()
{
    this->ChangeStep(m_CurrentStepIndex + 1);
}

void CTutorialManager::Again()
{
    this->ChangeStep(m_CurrentStepIndex);
}


CTutorialManager::TUTORIAL* CTutorialManager::addNewTutorialByKey(std::string key)
{
    TUTORIAL* newTutorial = new std::vector<CTutorialStep*>();
    m_TutorialList.emplace(std::pair<std::string, TUTORIAL*>(key, newTutorial));
    
    return newTutorial;
}

CTutorialManager::TUTORIAL* CTutorialManager::getTutorialByKey(std::string key)
{
    auto iter = m_TutorialList.find(key.c_str());
    if(iter == m_TutorialList.end())
        assertion(StringUtils::format("There is no tutorial for key : %s", key.c_str()));
    
    return iter->second;
}

CTutorialStep* CTutorialManager::getStepFromTutorial(std::string key, int index)
{
    auto tutorial = this->getTutorialByKey(key);
    if(tutorial->size() <= index)
        assertion(StringUtils::format("There is no step for index : %d", index));
    
    return tutorial->at(index);
}

void CTutorialManager::stepBegin(std::string key, int index)
{
    auto step = this->getStepFromTutorial(key, index);
    step->Begin();
    step->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(this->getContentSize() / 2)
    ->show(this);
}

void CTutorialManager::stepEnd(std::string key, int index)
{
    auto step = this->getStepFromTutorial(key, index);
    step->End();
    step->popupClose();
}

void CTutorialManager::clear()
{
    m_IsRunning = false;
    m_CurrentStepIndex = 0;
    m_CurrentTutorial = nullptr;
}