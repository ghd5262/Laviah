#include "TutorialLayer.hpp"
#include "DataManagerUtils.h"
#include "../MyUI/MyButton.h"
#include "../MyUI/Popup/MessageBox.hpp"
#include "../json/json.h"

using namespace cocos2d;


CTutorialLayer* CTutorialLayer::m_Instance = nullptr;
CTutorialLayer::CTutorialLayer()
: m_CurrentTutorial(nullptr)
, m_IsRunning(false){}

CTutorialLayer::~CTutorialLayer(){
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_TutorialList);
    m_Instance = nullptr;
}

CTutorialLayer* CTutorialLayer::Instance()
{
    if(m_Instance != nullptr) return m_Instance;
    
    m_Instance = new(std::nothrow) CTutorialLayer();
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

void CTutorialLayer::update(float delta)
{
    // call update function of current step of current tutorial.
    if(m_CurrentStep) m_CurrentStep->Update();
}

void CTutorialLayer::addStepToByTutorialKey(std::string key, CTutorialStep* step)
{
    auto iter = m_TutorialList.find(key.c_str());
    CTutorialObject* tutorial = nullptr;
    
    // if there is no tutorial for the key.
    if (iter == m_TutorialList.end())   tutorial = this->addNewTutorial(key);
    else                                tutorial = (iter->second)._tutorial;
    
    this->addStepToTutorial(tutorial, step);
}

void CTutorialLayer::ChangeTutorial(std::string key)
{
    if(key == "") return;
    
    // call end listener of current tutorial.
    if(m_CurrentTutorial) m_CurrentTutorial->End();
    
    // keep new tutorial.
    m_CurrentTutorial = this->getTutorialByKey(key);

    m_IsRunning = true;
}

void CTutorialLayer::ChangeStep(int index)
{
    m_CurrentTutorial->ChangeStep(index);
}

void CTutorialLayer::NextStep()
{
    m_CurrentTutorial->NextStep();
}

void CTutorialLayer::Again()
{
    m_CurrentTutorial->Again();
}

void CTutorialLayer::Clear()
{
    m_IsRunning = false;
}

bool CTutorialLayer::init()
{
    if (!CPopup::init()) return false;
    this->scheduleUpdate();
    return true;
}

CTutorialObject* CTutorialLayer::addNewTutorial(std::string key)
{
    
    
    m_TutorialList.emplace(std::pair<std::string, CTutorialObject*>(key, tutorial));
    return tutorial;
}

void CTutorialLayer::addStepToTutorial(CTutorialObject* tutorial, CTutorialStep* step)
{
    this->addChild(step);
    tutorial->emplace_back(step);
}

CTutorialObject* CTutorialLayer::getTutorialByKey(std::string key)
{
    auto iter = m_TutorialList.find(key.c_str());
    if(iter == m_TutorialList.end())
        DATA_MANAGER_UTILS::assertion(StringUtils::format("There is no tutorial for key : %s", key.c_str()));
    
    return iter->second;
}






CTutorialObject* CTutorialObject::create()
{
    CTutorialObject *pRet = new(std::nothrow) CTutorialObject();
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

CTutorialObject* CTutorialObject::addBeginListener(const TUTORIAL_LISTENER& listener)
{
    m_BeginListener = listener;
    return this;
}

CTutorialObject* CTutorialObject::addEndListener(const TUTORIAL_LISTENER& listener)
{
    m_EndListener = listener;
    return this;
}

CTutorialObject* CTutorialObject::addTutorialStep(CTutorialStep* step)
{
    m_StepList.emplace_back(step);
    return this;
}

void CTutorialObject::update(float delta)
{
    // call update function of current step of current tutorial.
    if(m_CurrentStep) m_CurrentStep->Update();
}

CTutorialStep* CTutorialObject::getStepByIndex(int index) const
{
    if(index < m_StepList->size())
        return m_StepList->at(index);

    DATA_MANAGER_UTILS::assertion(StringUtils::format("There is no step for index : %d", index));
}

void CTutorialObject::Begin()
{
    m_CurrentStepIndex = 0;
    this->callListener(m_BeginListener);
}

void CTutorialObject::End()
{
    this->callListener(m_EndListener);
}

void CTutorialObject::NextStep()
{
    // if there is next step.
    // change to next.
    if(m_CurrentStepIndex + 1 < m_StepList.size())
        this->ChangeStep(++m_CurrentStepIndex);
}

void CTutorialObject::ChangeStep(int index)
{
    // call end listener of current step of tutorial.
    (this->getStepByIndex(m_CurrentStepIndex))->End();
    
    // keep new step index
    m_CurrentStepIndex = index;
    
    // call begin listener of new step
    (this->getStepByIndex(m_CurrentStepIndex))->Begin();
}

void CTutorialObject::Again()
{
    this->ChangeStep(m_CurrentStepIndex);
}

void CTutorialObject::callListener(TUTORIAL_LISTENER listener)
{
    if(listener)
    {
        this->retain();
        listener(this);
        this->release();
    }
}




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

bool CTutorialStep::init()
{
    if (!Node::init()) return false;
    this->createEventListener();
    this->setContentSize(_director->getWinSize());
    return true;
}

CTutorialStep* CTutorialStep::build(std::string tutorialKey)
{
    if(m_MessageList.size())
    {
        auto messagebox = CMessageBox::create()
        ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 430, 300))
        ->setTailEnable(true)
        ->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
        ->setMessage("")
        ->setPopupPosition(m_MessageBoxPosition)
        ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM)
        ->setDefaultCallbackEnable(false)
        ->setBackgroundVisible(false)
        ->show(this);
        m_MessageBox = dynamic_cast<CMessageBox*>(messagebox);
    }
    
    {
        CTutorialLayer::Instance()->addStepToByTutorialKey(tutorialKey, this);
    }
    return this;
}

CTutorialStep* CTutorialStep::addEventListener(const STEP_LISTENER& listener,
                                               TUTORIAL_EVENT type)
{
    switch (type) {
        case TUTORIAL_EVENT::BEGIN:  m_BeginListener  = listener; break;
        case TUTORIAL_EVENT::UPDATE: m_UpdateListener = listener; break;
        case TUTORIAL_EVENT::END:    m_EndListener    = listener; break;
        default: m_EventListener->addEventListener([=](Node* sender){
            
            // if there are message boxes. show message boxes before than listeners.
            // call listeners after show all message boxes.
            if(m_CurrentMessageIndex < m_MessageList.size())
            {
                m_MessageBox->changeMessage(m_MessageList.at(m_CurrentMessageIndex++));
                return;
            }
            
            this->callListener(listener);
        }, (eMYBUTTON_STATE)type); break;
    }
    
    return this;
}

CTutorialStep* CTutorialStep::addMessageBox(cocos2d::Node* parent,
                                            std::string message)
{
    m_MessageBoxPosition = parent->convertToWorldSpace(parent->getContentSize() / 2);
    m_MessageList.emplace_back(message);
    return this;
}

void CTutorialStep::Begin()
{
    this->clear();
    this->callListener(m_BeginListener);
}

void CTutorialStep::callListener(STEP_LISTENER listener)
{
    if(listener)
    {
        this->retain();
        listener(this);
        this->release();
    }
}

void CTutorialStep::createEventListener()
{
    auto size = _director->getWinSize();
    m_EventListener = CMyButton::create()
    ->setEnableSound(false)
    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setLayer(LayerColor::create(COLOR::BRIGHTGRAY_ALPHA, size.width, size.height))
    ->show(this);
}

void CTutorialStep::clear()
{
    m_CurrentMessageIndex = 0;
}