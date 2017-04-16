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

void CTutorialHelper::CreateMessageBox(std::string key, std::string msg, bool backgroundVisible, bool tailEnable)
{
    CTutorialStep::create()
    ->addTouchListener([=](Node* sender){
        CTutorialManager::Instance()->NextStep();
    })
    ->addBeginListener([=](Node* sender){
        CObjectManager::Instance()->SpeedControl(0.5f, 1, true);
        CGameScene::getZoomLayer()->pause();
    })
    ->addEndListener([](Node* sender){
        CObjectManager::Instance()->SpeedControl(0.5f, 90, true);
        CGameScene::getZoomLayer()->resume();
    })
    ->addMessageBox(msg, tailEnable)
    ->build(key)
    ->setBackgroundVisible(backgroundVisible)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA);
}

void CTutorialHelper::NextStepAfterDelay(std::string key, bool backgroundVisible, float delay)
{
    CTutorialStep::create()
    ->addUpdateListener([=](float delta, CTutorialStep* sender){
        
        if(sender->getTime() > delay)
            CTutorialManager::Instance()->NextStep();
    })
    ->build(key)
    ->setBackgroundVisible(backgroundVisible)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA);
}

void CTutorialHelper::CreateBulletPattern(std::string key, bool backgroundVisible, int tutorialPatternIdx)
{
    CTutorialStep::create()
    ->addBeginListener([=](CTutorialStep* sender){
        auto data = CBulletPatternDataManager::Instance()->getTutorialPatternByIndex(tutorialPatternIdx);
        CObjectManager::Instance()->getBulletCreator()->CreateImmediately(data, 90, 2000);
    })
    ->addUpdateListener([=](float delta, CTutorialStep* sender){
            CTutorialManager::Instance()->NextStep();
    })
    ->build(TUTORIAL_KEY::BEGINER)
    ->setBackgroundVisible(backgroundVisible)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA);
}
