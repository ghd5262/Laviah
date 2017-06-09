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
                                       eMYBUTTON_STATE action/* = eMYBUTTON_STATE::END */)
{
    auto layerSize = Director::getInstance()->getWinSize();
    auto button    = CMyButton::create()
    ->addEventListener([](Node* sender){
        CObjectManager::Instance()->SpeedControl(0, 90, true);
        CGameScene::getZoomLayer()->resume();
        CTutorialManager::Instance()->NextStep();
    }, action)
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

void CTutorialHelper::RotationEnable(std::string key, bool enable)
{
    CTutorialStep::create()
    ->addBeginListener([=](CTutorialStep* sender){
        CTutorialManager::Instance()->setIsRotationEnable(enable);
    })
    ->addUpdateListener([=](float delta, CTutorialStep* sender){
        CTutorialManager::Instance()->NextStep();
    })
    ->build(TUTORIAL_KEY::BEGINER)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA);
}