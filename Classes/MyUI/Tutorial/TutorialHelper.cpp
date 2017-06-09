#include "TutorialHelper.hpp"
#include "../MyButton.h"
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
                                       bool tailEnable/* = true */)
{
    auto layerSize = Director::getInstance()->getWinSize();
    auto button    = CMyButton::create()
    ->addEventListener([](Node* sender){
        CTutorialManager::Instance()->NextStep();
    })
    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, layerSize.width, layerSize.height))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(layerSize / 2)
    ->setEnableSound(false);

    
    CTutorialStep::create()
    ->addBeginListener([](Node* sender){
        CObjectManager::Instance()->SpeedControl(0, 0, true);
        CGameScene::getZoomLayer()->pause();
    })
    ->addEndListener([](Node* sender){
        CObjectManager::Instance()->SpeedControl(0, 90, true);
        CGameScene::getZoomLayer()->resume();
    })
    ->addButton(button)
    ->addMessageBox(msg, tailEnable)
    ->build(key)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA);
}

void CTutorialHelper::CreateMessageBoxWithRotation(std::string key,
                                                   std::string msg,
                                                   bool tailEnable/* = true */)
{
    auto layerSize = Director::getInstance()->getWinSize();
    auto button    = CMyButton::create()
    ->addEventListener([](Node* sender){
        CObjectManager::Instance()->SpeedControl(0, 90, true);
        CGameScene::getZoomLayer()->resume();
    }, eMYBUTTON_STATE::BEGIN)
    ->addEventListener([](Node* sender){
        CTutorialManager::Instance()->NextStep();
    }, eMYBUTTON_STATE::END)
    ->addEventListener([](Node* sender){
        CObjectManager::Instance()->RotationObject(-2);
    }, eMYBUTTON_STATE::EXECUTE)
    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, layerSize.width, layerSize.height))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(layerSize / 2)
    ->setEnableSound(false);
    
    CTutorialStep::create()
    ->addBeginListener([](Node* sender){
        CObjectManager::Instance()->SpeedControl(0, 0, true);
        CGameScene::getZoomLayer()->pause();
    })
    ->addButton(button)
    ->addMessageBox(msg, tailEnable)
    ->build(key)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA);
}

void CTutorialHelper::NextStepAfterDelay(std::string key, float delay)
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
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA);
}

void CTutorialHelper::CreateBulletPattern(std::string key, int patternIdx)
{
    CTutorialStep::create()
    ->addBeginListener([=](CTutorialStep* sender){
        auto data = CBulletPatternDataManager::Instance()->getTutorialPatternByIndex(patternIdx);
        CObjectManager::Instance()->getBulletCreator()->setRotationAngle(0.f);
        CObjectManager::Instance()->getBulletCreator()->setPattern(data);
    })
    ->addUpdateListener([=](float delta, CTutorialStep* sender){
            CTutorialManager::Instance()->NextStep();
    })
    ->build(TUTORIAL_KEY::BEGINER)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA);
}
