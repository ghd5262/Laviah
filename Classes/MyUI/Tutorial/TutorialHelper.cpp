#include "TutorialHelper.hpp"
#include "../../GameObject/ObjectManager.h"
#include "../../Scene/GameScene.h"

using namespace cocos2d;

CTutorialHelper::CTutorialHelper(){}

CTutorialHelper::~CTutorialHelper(){}

CTutorialHelper* CTutorialHelper::Instance()
{
    static CTutorialHelper instance;
    return &instance;
}

void CTutorialHelper::CreateMessageBox(std::string msg, std::string key)
{
    CTutorialStep::create()
    ->addTouchListener([=](Node* sender){
        CTutorialManager::Instance()->NextStep();
    })
    ->addBeginListener([=](Node* sender){
        CObjectManager::Instance()->SpeedControl(0.3f, 1);
        CGameScene::getZoomLayer()->pause();
    })
    ->addEndListener([](Node* sender){
        CGameScene::getZoomLayer()->resume();
    })
    ->addMessageBox(msg)
    ->build(key)
    ->setBackgroundVisible(false);
}

void CTutorialHelper::NextStepAfterDelay(float delay, std::string key)
{
    CTutorialStep* step = CTutorialStep::create()
    ->addUpdateListener([=](float delta, CTutorialStep* sender){
        
        if(sender->getTime() > delay)
            CTutorialManager::Instance()->NextStep();
    })
    ->build(key);
    step->setBackgroundVisible(false);
}