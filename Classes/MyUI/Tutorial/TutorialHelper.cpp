#include "TutorialHelper.hpp"
#include "../../GameObject/ObjectManager.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/BulletPatternDataManager.h"
#include "../../GameObject/BulletCreator.h"

using namespace cocos2d;

CTutorialHelper::CTutorialHelper(){}

CTutorialHelper::~CTutorialHelper(){}

CTutorialHelper* CTutorialHelper::Instance()
{
    static CTutorialHelper instance;
    return &instance;
}

void CTutorialHelper::CreateMessageBox(std::string key,
                                       std::string msg,
                                       bool tailEnable/* = true */,
                                       bool backgroundVisible/* = false */)
{
    CTutorialStep::create()
    ->addTouchListener([=](Node* sender){
        CTutorialManager::Instance()->NextStep();
    })
    ->addBeginListener([=](Node* sender){
        CObjectManager::Instance()->SpeedControl(0, 0, true);
        CGameScene::getZoomLayer()->pause();
    })
    ->addEndListener([](Node* sender){
        CObjectManager::Instance()->SpeedControl(0, 90, true);
        CGameScene::getZoomLayer()->resume();
    })
    ->addMessageBox(msg, tailEnable)
    ->build(key)
    ->setBackgroundVisible(backgroundVisible)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA);
}

void CTutorialHelper::NextStepAfterDelay(std::string key,
                                         float delay,
                                         bool backgroundVisible/* = false */)
{
    CTutorialStep::create()
    ->addBeginListener([=](CTutorialStep* sender){
        sender->setTime(0);
    })
    ->addUpdateListener([=](float delta, CTutorialStep* sender){
        
        if(sender->getTime() > delay)
            CTutorialManager::Instance()->NextStep();
    })
    ->build(key)
    ->setBackgroundVisible(backgroundVisible)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA);
}

void CTutorialHelper::CreateBulletPattern(std::string key,
                                          int tutorialPatternIdx,
                                          bool backgroundVisible/* = false */)
{
    CTutorialStep::create()
    ->addBeginListener([=](CTutorialStep* sender){
        auto data = CBulletPatternDataManager::Instance()->getTutorialPatternByIndex(tutorialPatternIdx);
        CObjectManager::Instance()->getBulletCreator()->setPattern(data);
    })
    ->addUpdateListener([=](float delta, CTutorialStep* sender){
            CTutorialManager::Instance()->NextStep();
    })
    ->build(TUTORIAL_KEY::BEGINER)
    ->setBackgroundVisible(backgroundVisible)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA);
}
