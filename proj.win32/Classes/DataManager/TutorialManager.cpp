#include "TutorialManager.hpp"
#include "DataManagerUtils.h"
#include "../Common/HSHUtility.h"
#include "../MyUI/MyButton.h"
#include "../json/json.h"

using namespace cocos2d;

CTutorialStep* CTutorialStep::create()
{
    CTutorialStep *pRet = new(std::nothrow) CTutorialStep();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

CTutorialStep* CTutorialStep::build(std::string tutorialKey)
{
    CTutorialManager::Instance()->addStepToByTutorialKey(tutorialKey, this);
    return this;
}

CTutorialStep* CTutorialStep::addEventListener(const TUTORIAL_LISTENER& listener, TUTORIAL_EVENT type)
{
    if(!m_EventListener)
    {
        auto size = _director->getWinSize();
        m_EventListener = CMyButton::create()
        ->setLayer(LayerColor::create(COLOR::BRIGHTGRAY_ALPHA, size.width, size.height))
        ->show(this);
    }
    
    switch (type) {
        case TUTORIAL_EVENT::BEGIN:  m_BeginListener  = listener; break;
        case TUTORIAL_EVENT::UPDATE: m_UpdateListener = listener; break;
        case TUTORIAL_EVENT::END:    m_EndListener    = listener; break;
        default: m_EventListener->addEventListener([=](Node* sender){
            this->callListener(listener);
        }, (eMYBUTTON_STATE)type); break;
    }
    
    return this;
}

CTutorialStep* CTutorialStep::addBubble(cocos2d::LayerColor* layer, std::string content, cocos2d::Node* anchor)
{
    return this;
}

void CTutorialStep::callListener(TUTORIAL_LISTENER listener)
{
    if(listener)
    {
        this->retain();
        listener(this);
        this->release();
    }
}


CTutorialManager* CTutorialManager::m_Instance = nullptr;
CTutorialManager::CTutorialManager()
: m_CurrentStep(nullptr)
, m_IsRunning(false)
, m_CurrentIndex(-1)
, m_CurrentKey(""){}

CTutorialManager::~CTutorialManager(){
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_TutorialList);
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

void CTutorialManager::update(float delta)
{
    // call update function of current step of current tutorial.
    if(m_CurrentStep) m_CurrentStep->Update();
}

void CTutorialManager::addStepToByTutorialKey(std::string key, STEP* step)
{
    auto iter = m_TutorialList.find(key.c_str());
    TUTORIAL* tutorial = nullptr;
    
    // if there is no tutorial for the key.
    if (iter == m_TutorialList.end())   tutorial = this->addNewTutorial(key);
    else                                tutorial = iter->second;
    
    this->addStepToTutorial(tutorial, step);
}

void CTutorialManager::ChangeStep(std::string key, int index)
{
    if(key == "")   return;
    else            m_CurrentKey = key;
    
    if(index == -1)
        m_CurrentIndex++;
    else
        m_CurrentIndex = index;
    
    auto assertion = [=](std::string content){
        content += (std::string(" : ") + key);
        CCLOG("%s", content.c_str());
        CCASSERT(false, content.c_str());
    };
    
    auto iter = m_TutorialList.find(key.c_str());
    if(m_CurrentStep) m_CurrentStep->End();
    if(iter == m_TutorialList.end()) assertion("There is no tutorial for key");
    
    auto tutorial = iter->second;
    if(tutorial->size() <= m_CurrentIndex){
        this->Clear();
        return;
    }
    
    m_CurrentStep = tutorial->at(m_CurrentIndex);
    m_CurrentStep->Begin();
    m_IsRunning = true;
}

void CTutorialManager::Again()
{
    this->ChangeStep(m_CurrentKey, m_CurrentIndex);
}

void CTutorialManager::Clear()
{
    m_IsRunning = false;
    m_CurrentKey = "";
    m_CurrentIndex = -1;
    if(m_CurrentStep)
    {
        m_CurrentStep->removeFromParent();
        m_CurrentStep = nullptr;
    }
}

bool CTutorialManager::init()
{
    if (!Node::init()) return false;
    this->scheduleUpdate();
    return true;
}

CTutorialManager::TUTORIAL* CTutorialManager::addNewTutorial(std::string key)
{
    auto tutorial = new TUTORIAL();
    m_TutorialList.emplace(std::pair<std::string, TUTORIAL*>(key, tutorial));
    return tutorial;
}

void CTutorialManager::addStepToTutorial(TUTORIAL* tutorial, STEP* step)
{
    this->addChild(step);
    tutorial->emplace_back(step);
}
