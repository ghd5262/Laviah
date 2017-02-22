#include "TutorialLayer.hpp"
#include "../MyUI/MyButton.h"
#include "../MyUI/Popup/MessageBox.hpp"
#include "../json/json.h"

using namespace cocos2d;

void assertion(std::string content){
    CCLOG("%s", content.c_str());
    CCASSERT(false, content.c_str());
};

CTutorialLayer* CTutorialLayer::m_Instance = nullptr;
CTutorialLayer::CTutorialLayer()
: m_CurrentTutorial(nullptr)
, m_IsRunning(false)
, m_CurrentStepIndex(0){}

CTutorialLayer::~CTutorialLayer(){
    for(auto tutorial : m_TutorialList)
        for(auto tutorialObj : *tutorial.second)
            tutorialObj->release();
    
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
    if(!m_IsRunning) return;
    // call update function of current step of current tutorial.
    (m_CurrentTutorial->at(m_CurrentStepIndex))->Update(delta);
}

void CTutorialLayer::addTutorial(std::string key, CTutorialObject* newTutorial)
{
    auto iter = m_TutorialList.find(key);
    TUTORIAL* tutorial = nullptr;
    if(iter == m_TutorialList.end())    tutorial = this->addNewTutorialByKey(key);
    else                                tutorial = this->getTutorialByKey(key);
    
    newTutorial->retain();
    tutorial->emplace_back(newTutorial);
}

void CTutorialLayer::ChangeTutorial(std::string key)
{
    if(key == "") return;
    
    // call end listener of current tutorial.
    this->stepEnd(key, m_CurrentStepIndex);
    m_CurrentTutorial = this->getTutorialByKey(key);
    m_CurrentStepIndex = 0;
    
    this->stepBegin(key, m_CurrentStepIndex);

    m_IsRunning = true;
}

void CTutorialLayer::ChangeStep(int index)
{
    if(m_CurrentTutorial->size() <= index) {
        this->clear();
        return;
    }
    
    auto currentKey = (m_CurrentTutorial->at(m_CurrentStepIndex))->getTutorialKey();
    this->stepEnd(currentKey, m_CurrentStepIndex);
    m_CurrentStepIndex = index;
    this->stepBegin(currentKey, m_CurrentStepIndex);
}

void CTutorialLayer::NextStep()
{
    this->ChangeStep(m_CurrentStepIndex + 1);
}

void CTutorialLayer::Again()
{
    this->ChangeStep(m_CurrentStepIndex);
}

bool CTutorialLayer::init()
{
    if (!CPopup::init()) return false;
    this->scheduleUpdate();
    return true;
}

CTutorialLayer::TUTORIAL* CTutorialLayer::addNewTutorialByKey(std::string key)
{
    TUTORIAL* newTutorial = new std::vector<CTutorialObject*>();
    m_TutorialList.emplace(std::pair<std::string, TUTORIAL*>(key, newTutorial));
    
    return newTutorial;
}

CTutorialLayer::TUTORIAL* CTutorialLayer::getTutorialByKey(std::string key)
{
    auto iter = m_TutorialList.find(key.c_str());
    if(iter == m_TutorialList.end())
        assertion(StringUtils::format("There is no tutorial for key : %s", key.c_str()));
    
    return iter->second;
}

CTutorialObject* CTutorialLayer::getStepFromTutorial(std::string key, int index)
{
    auto tutorial = this->getTutorialByKey(key);
    if(tutorial->size() <= index)
        assertion(StringUtils::format("There is no step for index : %d", index));
    return tutorial->at(index);
}

void CTutorialLayer::stepBegin(std::string key, int index)
{
    auto step = this->getStepFromTutorial(key, index);
    step->Begin();
    this->addChild(step);
}

void CTutorialLayer::stepEnd(std::string key, int index)
{
    auto step = this->getStepFromTutorial(key, index);
    step->End();
    step->removeFromParent();
}

void CTutorialLayer::clear()
{
    m_IsRunning = false;
    m_CurrentStepIndex = 0;
    m_CurrentTutorial = nullptr;
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

CTutorialObject* CTutorialObject::build(std::string key)
{
    m_TutorialKey = key;
    this->setContentSize(_director->getWinSize());
    
    if(!m_TouchEnable)
    {
        CMyButton::create()
        ->addEventListener([=](Node* sender){})
        ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
        ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080, 1920))
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(this->getContentSize() / 2)
        ->show(this);
    }
    
    if(m_TouchListener)
    {
        CMyButton::create()
        ->addEventListener([=](Node* sender){
            this->retain();
            if(m_TouchListener){
                m_TouchListener(this);
            }
            this->release();
        })
        ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
        ->setLayer(LayerColor::create(COLOR::BRIGHT_WHITEGRAY_ALPHA, 1080, 1920))
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(this->getContentSize() / 2)
        ->show(this);
    }
    
    CTutorialLayer::Instance()->addTutorial(m_TutorialKey, this);
    return this;
}

CTutorialObject* CTutorialObject::addMessageBox(std::string message)
{
    m_Message = message;
    return this;
}

CTutorialObject* CTutorialObject::addTouchListener(const SINGLE_LISTENER& listener)
{
    m_TouchListener = listener;
    return this;
}

CTutorialObject* CTutorialObject::addBeginListener(const SINGLE_LISTENER& listener)
{
    m_BeginListener = listener;
    return this;
}

CTutorialObject* CTutorialObject::addUpdateListener(const UPDATE_LISTENER& listener)
{
    m_UpdateListener = listener;
    return this;
}

CTutorialObject* CTutorialObject::addEndListener(const SINGLE_LISTENER& listener)
{
    m_EndListener = listener;
    return this;
}

CTutorialObject* CTutorialObject::setTouchEnable(bool enable)
{
    m_TouchEnable = enable;
    return this;
}

void CTutorialObject::Begin()
{
    // if there is message, create message box
    if(m_Message != "")  this->createMessageBox();
    
    // call begin function
    this->callListener(m_BeginListener);
}

void CTutorialObject::Update(float delta)
{
    // call update function
    if(m_UpdateListener) m_UpdateListener(delta);
}

void CTutorialObject::End()
{
    if(m_MessageBox)
    {
//        m_MessageBox->popupClose();
        m_MessageBox->removeFromParent();
        m_MessageBox = nullptr;
    }
    
    // call end function
    this->callListener(m_EndListener);
}

void CTutorialObject::createMessageBox()
{
    if(m_MessageBox) return;
    
    auto popup = CMessageBox::create()
    ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 600, 200))
    ->setTailEnable(true)
    ->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
    ->setMessage(m_Message)
    ->setPopupPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.7f))
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM)
    ->setDefaultCallbackEnable(false)
    ->setBackgroundVisible(false)
    ->show(this);
    
    m_MessageBox = dynamic_cast<CMessageBox*>(popup);
}

void CTutorialObject::callListener(SINGLE_LISTENER listener)
{
    if(listener)
    {
        this->retain();
        listener(this);
        this->release();
    }
}