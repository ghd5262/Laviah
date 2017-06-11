#include "TutorialManager.hpp"
#include "TutorialStep.hpp"
#include "../../MyUI/MyButton.h"
#include "../../DataManager/DataManagerUtils.h"
#include "../../GameObject/ObjectManager.h"
#include "../../Scene/GameScene.h"

using namespace cocos2d;

void assertion(std::string content){
    CCLOG("%s", content.c_str());
    CCASSERT(false, content.c_str());
};

CTutorialManager* CTutorialManager::m_Instance = nullptr;
CTutorialManager::CTutorialManager()
: m_CurrentTutorial(nullptr)
, m_IsRunning(false)
, m_IsRotationEnable(true)
, m_OriginTutorialKey("")
, m_CurrentStepIndex(0){}

CTutorialManager::~CTutorialManager(){
    this->Clear();
    
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
    auto size = this->getContentSize();
    
    CMyButton::create()
    ->addEventListener([=](Node* sender){
        if(!m_IsRunning) return;
        if(!m_IsRotationEnable) return;
        CObjectManager::Instance()->RotationObject(-2);
    }, eMYBUTTON_STATE::EXECUTE)
    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, size.width, size.height))
    ->setEnableSound(false)
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(size / 2)
    ->show(this, ZORDER::POPUP)
    ->setSwallowTouches(false);
    
    return true;
}

void CTutorialManager::update(float delta)
{
    if(CObjectManager::Instance()->getIsGamePause()) return;
    if(!m_IsRunning) return;
    
    // call update function of current step of current tutorial.
    auto currentStep = m_CurrentTutorial->at(m_CurrentStepIndex);
    currentStep->Update(delta);
}

void CTutorialManager::addStep(std::string key, CTutorialStep* newTutorial)
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
    if(m_OriginTutorialKey != "")
        this->stepEnd(m_OriginTutorialKey, m_CurrentStepIndex);
    
    m_CurrentTutorial = this->getTutorialByKey(key);
    m_CurrentStepIndex = 0;
    m_OriginTutorialKey = key;
    
    this->stepBegin(key, m_CurrentStepIndex);

    m_IsRunning = true;
}

void CTutorialManager::ChangeStep(int index)
{
    auto currentStep = m_CurrentTutorial->at(m_CurrentStepIndex);
    auto currentKey  = currentStep->getTutorialKey();
    
    this->stepEnd(currentKey, m_CurrentStepIndex);
    if(m_CurrentTutorial->size() <= index) {
        this->reset();
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
    auto step = m_CurrentTutorial->at(m_CurrentStepIndex);
    step->removeFromParent(); // 같은 포인터를 다시 show하기 때문에 parent를 초기화 해줘야한다.
    this->ChangeStep(m_CurrentStepIndex);
}

void CTutorialManager::LoadLastSavedStep(std::string key)
{
    if(!m_IsRunning)        return;
    if(!m_CurrentTutorial)  return;
    
    auto step = m_CurrentTutorial->at(m_CurrentStepIndex);
    if(step->getTutorialKey() != key)
        this->ChangeTutorial(key);
    
    
    auto savedStepIndex = this->getSavedStepIndexByKey(key);
    this->ChangeStep(savedStepIndex);
}

void CTutorialManager::SaveCurrentStep(std::string key)
{
    auto iter = m_SavedStepList.find(key.c_str());
    if(iter == m_SavedStepList.end())
        assertion(StringUtils::format("There is no tutorial for key : %s", key.c_str()));
    
    iter->second = m_CurrentStepIndex;
}

void CTutorialManager::Clear()
{
    this->reset();
    
    // Clean the saved step list.
    m_SavedStepList.clear();
    
    for(auto data : m_TutorialList){
        auto tutorial = *data.second;
        DATA_MANAGER_UTILS::listDeleteAndClean(tutorial);
    }
    DATA_MANAGER_UTILS::mapDeleteAndClean(m_TutorialList);
}

CTutorialManager::TUTORIAL* CTutorialManager::addNewTutorialByKey(std::string key)
{
    TUTORIAL* newTutorial = new std::vector<CTutorialStep*>();
    m_TutorialList.emplace(std::pair<std::string, TUTORIAL*>(key, newTutorial));
    
    // add saved step
    m_SavedStepList.emplace(std::pair<std::string, int>(key, 0));
    
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

int CTutorialManager::getSavedStepIndexByKey(std::string key)
{
    auto iter = m_SavedStepList.find(key.c_str());
    if(iter == m_SavedStepList.end())
        assertion(StringUtils::format("There is no tutorial for key : %s", key.c_str()));

    return iter->second;
}

void CTutorialManager::stepBegin(std::string key, int index)
{
    auto step = this->getStepFromTutorial(key, index);
    step->Begin();
    step->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setDefaultCallbackEnable(false)
    ->setPopupPosition(this->getContentSize() / 2)
    ->show(this);
}

void CTutorialManager::stepEnd(std::string key, int index)
{
    auto step = this->getStepFromTutorial(key, index);
    step->End();
    step->popupClose();
}

void CTutorialManager::reset()
{
    if(m_IsRunning){
        auto currentStep = m_CurrentTutorial->at(m_CurrentStepIndex);
        currentStep->removeFromParent();
    }
    m_IsRunning = false;
    m_CurrentStepIndex = 0;
    m_CurrentTutorial = nullptr;
    m_OriginTutorialKey = "";
}