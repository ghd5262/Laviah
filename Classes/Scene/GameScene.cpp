#include "GameScene.h"
#include "EmptyScene.h"
#include "MenuScene.h"
#include "SceneTransition.h"
#include "../Task/PoolingManager.h"
#include "../GameObject/Planet.h"
#include "../GameObject/Player.h"
#include "../GameObject/ObjectManager.h"
#include "../GameObject/ItemManager.h"
#include "../GameObject/Bullet/Bullet.h"
#include "../GameObject/BackGround.h"
#include "../GameObject/BulletCreator.h"
#include "../GameObject/Rocket.h"
#include "../GameObject/ItemRange.h"
#include "../GameObject/MagnetEffect.h"
#include "../MyUI/MyButton.h"
#include "../MyUI/MenuLayer.hpp"
#include "../MyUI/UILayer.hpp"
#include "../MyUI/BonusTimeLayer.hpp"
#include "../MyUI/ComboScore.h"
#include "../MyUI/CountDown.hpp"
#include "../MyUI/Tutorial/TutorialHelper.hpp"
#include "../MyUI/FacebookRivalRankLayer.hpp"
#include "../MyUI/UrlSprite.hpp"
#include "../MyUI/Popup.h"
#include "../MyUI/Popup/PausePopup.h"
#include "../MyUI/Popup/ResultPopup.h"
#include "../MyUI/Popup/VideoPopup.h"
#include "../MyUI/Popup/GoalPopup.h"
#include "../MyUI/Popup/HelpPopup.h"
#include "../MyUI/Popup/RewardPopup.h"
#include "../MyUI/Popup/Option/OptionPopup.hpp"
#include "../MyUI/Popup/WorkshopPopup.h"
#include "../MyUI/Popup/CharacterPopup.hpp"
#include "../MyUI/Popup/CharacterCostumePopup.hpp"
#include "../MyUI/Popup/GameEndPopup.hpp"
#include "../MyUI/Popup/FacebookAPITestPopup.hpp"
#include "../MyUI/Popup/FacebookRankPopup.hpp"
#include "../MyUI/Popup/FacebookRankUpPopup.hpp"
#include "../MyUI/Popup/AchievementPopup.hpp"
#include "../MyUI/Popup/SharePopup.hpp"
#include "../MyUI/Popup/DownloadPopup.hpp"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/CharacterDataManager.h"
#include "../DataManager/AchievementDataManager.hpp"
#include "../DataManager/AchievementRewarder/AchievementRewarder.hpp"
#include "../DataManager/NetworkManager.hpp"
#include "../DataManager/FreeRewardManager.hpp"
#include "../DataManager/BulletPatternDataManager.h"
#include "../AI/States/RocketStates.h"
#include "../SDKBOX/SDKBoxHeaders.h"

#include <array>

USING_NS_CC;


CGameScene* CGameScene::m_GameScene = nullptr;
cocos2d::Layer* CGameScene::m_ZoomLayer = nullptr;
cocos2d::Layer* CGameScene::m_PopupLayer = nullptr;

Scene* CGameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
#if CC_USE_PHYSICS
    scene->initWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0, -98 * 40));
    
#endif
    // 'layer' is an autorelease object
    auto layer = CGameScene::create();
    // add layer as a child to scene
    scene->setAnchorPoint(Vec2(0, 0));
    layer->setAnchorPoint(Vec2(0, 0));
    scene->addChild(layer);
    
    return scene;
}

CGameScene::CGameScene()
: m_UILayer(nullptr)
, m_MenuLayer(nullptr)
, m_BonusTimeLayer(nullptr)
, m_TutorialLayer(nullptr)
, m_RivalRankLayer(nullptr)
, m_ScreenFade(nullptr)
, m_CountDown(nullptr)
, m_KeyBoardSpace(false)
, m_NeedTutorial(false)
, m_DailyResetRemain(0L)
, m_WeeklyResetRemain(0L)
, m_GamePlayTime(0L)
, m_Time(0.f){}

CGameScene::~CGameScene()
{
    CObjectManager::Instance()->RemoveAllObject();
    removeAllChildrenWithCleanup(true); // 이부분 검토
    CPoolingManager::Instance()->DeleteAllMemory();
    CAudioManager::Instance()->Clear();
}

void CGameScene::update(float delta)
{
//    if (m_KeyBoardSpace) CObjectManager::Instance()->RotationObject(-2.f);
    m_Time += delta;
    m_GamePlayTime = (long)m_Time;
    CObjectManager::Instance()->Execute(delta);
}

bool CGameScene::init()
{
    if (!Layer::init()) return false;
    
    m_GameScene = this;
    m_VisibleSize = Director::getInstance()->getVisibleSize();
    m_TouchPos = m_VisibleSize / 2;
    this->scheduleUpdate();
    this->initMemoryPool();
    this->createFacebookManager();
    this->createPopupLayer();
    this->createZoomLayer();
    this->createBulletCreator();
    this->createBackground();
    this->createPlanet();
    this->createPlayer();
    this->createRocket();
//    this->createSlowPoint();
    this->createCountDown();
    this->createScreenFade();
    this->createItemRanges();
    this->createComboUI();
    this->createMenuLayer();
    this->createUILayer();
    this->createRivalRankLayer();
    this->createTutorialLayer();
    this->createCaptureNode();
    this->createBackKeyButton();
    this->initKeyboardListener();
    this->intro();
    this->dailyGoalResetCheck(true);
    this->facebookRankingResetCheck();
    this->schedule([=](float delta){
        if(CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::DATA_SAVE_AUTO))
            CUserDataManager::Instance()->SaveUserData(true, true);
    }, 300.f, "AutoSave");
    
//    this->ScreenFade([=](){
//        this->Reward([=](bool isPlay){
//            CObjectManager::Instance()->ZoomMoveMiddle();
//            this->menuOpen();
//        }, {
//            sREWARD_DATA(ACHIEVEMENT_REWARD_KEY::REWARD_COSTUME_RANDOM, 0),
//            sREWARD_DATA(ACHIEVEMENT_REWARD_KEY::REWARD_COSTUME_RANDOM, 0)
//
//        }, 0, true);
//    });
//    
//    CObjectManager::Instance()->setPhotoShareAble(true);
//    this->createResultPopup();
    
    CAudioManager::Instance()->Clear();
    CAudioManager::Instance()->PlayBGM("sounds/menuBGM.wav", false);
    
    return true;
}

void CGameScene::GameExit(bool resume/* = false*/)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
    return;
#endif
    this->GamePause();
    this->createExitPopup(resume);
}

void CGameScene::GameStart()
{
    //    this->ScreenFade([=](){
    this->clearData();
    this->GameResume();
    this->MenuFadeOut();
    
    m_UILayer->setVisible(true);
    m_UILayer->setDefaultCallbackToTopAgain();
    
    CObjectManager::Instance()->ZoomMoveMiddle();
    CObjectManager::Instance()->getPlayer()->GameStart();
    CObjectManager::Instance()->getRocket()->ChangeState(CFlyAway::Instance());
    CObjectManager::Instance()->getPlanet()->StopRotation();
    dynamic_cast<CFacebookRivalRankLayer*>( m_RivalRankLayer )->Reset();
    CAudioManager::Instance()->StopBGM();
    CAudioManager::Instance()->PlayBGM("sounds/inGameBGM.mp3", true, false);

    
    //        CAudioManager::Instance()->PlayBGM("sounds/bgm_1.mp3", true);
    
    //    });
}

void CGameScene::GameResume()
{
    m_CountDown->Reset();
    this->turnUpSound();
}

void CGameScene::GamePause()
{
    CObjectManager::Instance()->setIsGamePause(true);
    this->turnDownSound();
}

void CGameScene::GameResult()
{
    CObjectManager::Instance()->ZoomMoveDown();
    this->createResultPopup();
    this->GamePause();
    this->dailyGoalResetCheck();
}

void CGameScene::GameEnd()
{
    this->createEndPopup();
    this->GamePause();
    m_ZoomLayer->pause();
}

void CGameScene::GameHelp()
{
    this->createHelpPopup();
}

void CGameScene::OpenGamePausePopup()
{
    // 이미 Pause인 상태면 리턴한다.
    if(CObjectManager::Instance()->getIsGamePause())
        return;
    
    this->GamePause();
    this->createPausePopup();
    m_ZoomLayer->pause();
}

void CGameScene::OpenGameMenuLayer()
{
    this->ScreenFade([=](){
        CAudioManager::Instance()->StopBGM();
        CObjectManager::Instance()->ZoomMoveMiddle();
        this->menuOpen();
    });
}

void CGameScene::OpenOptionPopup(int scrollIndex/* = 0*/)
{
    CObjectManager::Instance()->ZoomMoveDown();
    this->createOptionPopup(scrollIndex);
    this->MenuFadeOut();
}

void CGameScene::OpenWorkshopPopup()
{
    CObjectManager::Instance()->ZoomMoveDown();
    this->createWorkshopPopup();
    this->MenuFadeOut();
}

void CGameScene::OpenCharacterPopup()
{
    CObjectManager::Instance()->ZoomMoveDown();
    this->createCharacterPopup();
    this->MenuFadeOut();
}

void CGameScene::OpenCostumePopup(const VOID_CALLBACK& callback,
                                  int index)
{
    CObjectManager::Instance()->ZoomMoveDown();
    this->createCostumePopup(callback, index);
    this->MenuFadeOut();
}

void CGameScene::OpenRankPopup()
{
    CObjectManager::Instance()->ZoomMoveDown();
    this->createRankPopup();
    this->MenuFadeOut();
}

void CGameScene::OpenRankUpPopup()
{
    this->createRankUpPopup();
//    this->GamePause();
//    m_ZoomLayer->pause();
}

void CGameScene::OpenPermRequestPopup(const VOID_CALLBACK& callback)
{
    auto login = [=](){
        CFacebookManager::Instance()->Login([=](){
            
            CFacebookManager::Instance()->ClearData();
            CFacebookManager::RequestMyInfo();
            CFacebookManager::Instance()->setMyInfoListener([=](){
                CFacebookManager::Instance()->RequestPermission([=](){
                    CFacebookManager::RequestFriendList();
                    CFacebookManager::Instance()->setFriendListListener([=](){
                        CFacebookManager::Instance()->RequestPermission(callback, sdkbox::FB_PERM_PUBLISH_POST);
                    });
                }, sdkbox::FB_PERM_READ_USER_FRIENDS);
            });
        });
    };
    
    if(CFacebookManager::IsScoresEnabled()) callback();
    else {
        std::string message = TRANSLATE("FACEBOOK_ALERT_MESSAGE_FIRST") + std::string("\n(");
        bool createAlert = false;
        if(!sdkbox::PluginFacebook::isLoggedIn()){
            createAlert = true;
            message += std::string(TRANSLATE("FACEBOOK_PERMISSION_LOGIN"));
        }if(!CFacebookManager::IsPermissionAllowed(sdkbox::FB_PERM_READ_USER_FRIENDS)){
            createAlert = true;
            message += std::string(TRANSLATE("FACEBOOK_PERMISSION_FRIEND"));
        }if(!CFacebookManager::IsPermissionAllowed(sdkbox::FB_PERM_PUBLISH_POST)){
            createAlert = true;
            message += std::string(TRANSLATE("FACEBOOK_PERMISSION_PUBLISH"));
        }
        message.pop_back();// 마지막 쉼표를 빼줌
        message += std::string(" )\n") + std::string(TRANSLATE("FACEBOOK_ALERT_MESSAGE_LAST"));
        
        if(createAlert){
            this->CreateAlertPopup()
            ->setPositiveButton([=](Node* sender){
                login();
            }, TRANSLATE("BUTTON_YES"))
            ->setNegativeButton([=](Node* sender){}, TRANSLATE("BUTTON_NO"))
            ->setMessage(message)
            ->show(m_PopupLayer, ZORDER::POPUP);
        }else{
            login();
        }
    }
}

void CGameScene::OpenFBTestPopup()
{
    CFacebookAPITestPopup::create()
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::OpenAchievementPopup()
{
    CObjectManager::Instance()->ZoomMoveDown();
    this->createAchievementPopup();
    this->MenuFadeOut();
}

void CGameScene::OpenGoalPopup()
{
    this->createGoalPopup();
    this->GamePause();
}

void CGameScene::OpenVideoPopup()
{
    this->createVideoPopup();
    this->GamePause();
    m_ZoomLayer->pause();
}

void CGameScene::OpenSharePopup()
{
    CSharePopup::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::OpenDownloadPopup()
{
    CObjectManager::Instance()->ReturnToMemoryBlockAll();
    CObjectManager::Instance()->ZoomMoveDown();
    CDownloadPopup::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
    this->MenuFadeOut();
}

void CGameScene::RandomCoin()
{
    this->createRandomCoin();
}

void CGameScene::ScreenFade(const VOID_CALLBACK& callback/* = nullptr*/)
{
    auto fadeOut = FadeTo::create(0.5f, 1);
    auto fadeIn  = FadeIn::create(0.5f);
    auto callFunc = CallFunc::create([=](){
        callback();
    });
    auto delayAction = DelayTime::create(0.3f);
    auto visibleOff = CallFunc::create([=](){ m_ScreenFade->setVisible(false); });
    auto visibleOn  = CallFunc::create([=](){ m_ScreenFade->setVisible(true);  });
    
    
    m_ScreenFade->runAction(Sequence::create(visibleOn,
                                             fadeIn,
                                             callFunc,
                                             delayAction,
                                             fadeOut,
                                             visibleOff,
                                             nullptr));
}

void CGameScene::MenuFadeIn()
{
    m_MenuLayer->setVisible(true);
    m_MenuLayer->setOpacity(0);
    auto delayAction    = DelayTime::create(1.0f);
    auto fadeAction     = FadeIn::create(0.5f);
    auto callFunc       = CallFunc::create([=](){
        m_IsMenuLayerFront = true;
        this->startAppreciatePlanet();
    });
    auto sequenceAction = Sequence::create(delayAction, fadeAction, callFunc, nullptr);
    m_MenuLayer->runAction(sequenceAction);
    
    // Update the achievement button.
    bool enable = CAchievementDataManager::Instance()->ExistCompletedHiddenAchievement();
    CMenuLayer::Instance()->AchievementButtonState(enable);
    
//    if(!enable){
        // Save the index of the last completed achievement
//        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::LAST_COM_ACHIEVEMENT, 0);
//    }
}

void CGameScene::MenuFadeOut()
{
    auto fadeAction     = FadeTo::create(0.3f, 0);
    auto callFunc       = CallFunc::create([=](){
        m_MenuLayer->setVisible(false);
        m_IsMenuLayerFront = false;
        this->stopAppreciatePlanet();
    });
    auto sequenceAction = Sequence::createWithTwoActions(fadeAction, callFunc);
    m_MenuLayer->runAction(sequenceAction);
}

CPopup* CGameScene::CreateAlertPopup()
{
    return CPopup::create()
    ->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2);
}

void CGameScene::BonusTimeStart()
{
    this->createBonusTimeLayer();
}

void CGameScene::BonusTimeEnd()
{
    this->removeBonusTimeLayer();
}

void CGameScene::Reward(std::function<void(bool)> exitCallback,
                        std::vector<sREWARD_DATA> list,
                        int cost/* = 0*/,
                        bool ufoEnable/* = false*/)
{
    CObjectManager::Instance()->MoveAction(m_PopupLayer,
                                           Vec2(m_VisibleSize.width * 0.5f,
                                                m_VisibleSize.height * -.5f));
    
    CRewardPopup::create()
    ->AddRewardToList(list)
    ->setExitCallback([=](bool isPlay){
        exitCallback(isPlay);
        CObjectManager::Instance()->MoveAction(m_PopupLayer, m_VisibleSize / 2);
    })
    ->setIsPaidFeature(cost)
    ->setIsUFOEnable(ufoEnable)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * 1.5f))
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::clearData()
{
    CObjectManager::Instance()->Clear();
    CItemManager::Instance()->Clear();
    CTutorialManager::Instance()->Clear();
    this->cleanGlobalData();
    this->removeBonusTimeLayer();
}

void CGameScene::cleanGlobalData()
{
    GVALUE->Clear();
}

void CGameScene::createPausePopup()
{
    CPausePopup::create()
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createVideoPopup()
{
    CVideoPopup::create()
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createGoalPopup()
{
    CGoalPopup::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createResultPopup()
{
    CResultPopup::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createEndPopup()
{
    CGameEndPopup::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createHelpPopup()
{
    CHelpPopup::create()
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createExitPopup(bool resume)
{    
    this->CreateAlertPopup()
    ->setPositiveButton([=](Node* sender){
        Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    }, TRANSLATE("BUTTON_YES"))
    ->setNegativeButton([=](Node* sender){
        if (resume) this->GameResume();
        else        this->turnUpSound();
    }, TRANSLATE("BUTTON_NO"))
    ->setDefaultCallback([=](Node* sender){
        if (resume) this->GameResume();
        else        this->turnUpSound();
        auto popup = dynamic_cast<CPopup*>(sender);
        popup->popupClose();
    })
    ->setMessage(TRANSLATE("GAME_EXIT_CHECK"))
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createOptionPopup(int index)
{
    COptionPopup::create()
    ->setInitialScrollIndex(index)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createBonusTimeLayer()
{
    m_BonusTimeLayer = CBonusTimeLayer::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createWorkshopPopup()
{
    CWorkshopPopup::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createCharacterPopup()
{
    CCharacterPopup::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createCostumePopup(const VOID_CALLBACK& callback,
                                    int index)
{
    CCharacterCostumePopup::create()
    ->setExitCallback(callback)
    ->setCharacter(index)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createRankPopup()
{
    CFacebookRankPopup::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createRankUpPopup()
{
    CFacebookRankUpPopup::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createAchievementPopup()
{
    CAchievementPopup::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::removeBonusTimeLayer()
{
    if(m_BonusTimeLayer) {
        m_BonusTimeLayer->popupClose();
        m_BonusTimeLayer = nullptr;
    }
}

void CGameScene::createRandomCoin()
{
    auto data = CBulletPatternDataManager::Instance()->getRandomConstellationPatternByLevel(1, true);
    CBulletCreator::CreateConstellation(data);
}

void CGameScene::menuOpen()
{
    this->clearData();
    //        this->createRandomCoin();
    this->freeRewardCheck();
    this->dailyGoalResetCheck();
    this->MenuFadeIn();
    this->turnUpSound();

    m_UILayer->setVisible(false);
    m_MenuLayer->setDefaultCallbackToTopAgain();
    CObjectManager::Instance()->getRocket()->ComebackHome();
    CObjectManager::Instance()->getPlanet()->StartRotation();
    CObjectManager::Instance()->getPlayer()->setVisible(false);
}

void CGameScene::turnDownSound()
{
    auto userBGMVolume      = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BGM_VOLUME);
//    auto userEffectVolume   = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::EFFECT_VOLUME);
    
    CAudioManager::Instance()->setBGMVolume(userBGMVolume * 0.1f);
//    CAudioManager::Instance()->setEffectSoundVolume(userEffectVolume * 0.1f);
}

void CGameScene::turnUpSound()
{
    auto userBGMVolume      = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BGM_VOLUME);
//    auto userEffectVolume   = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::EFFECT_VOLUME);
    
    CAudioManager::Instance()->setBGMVolume(userBGMVolume);
//    CAudioManager::Instance()->setEffectSoundVolume(userEffectVolume);
}

void CGameScene::initKeyboardListener()
{
    EventListenerKeyboard * pListener = EventListenerKeyboard::create();
    pListener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* pEvent)
    {
        if(code == EventKeyboard::KeyCode::KEY_SPACE) m_KeyBoardSpace = true;
    };
    
    pListener->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* pEvent)
    {
        if(code == EventKeyboard::KeyCode::KEY_SPACE) m_KeyBoardSpace = false;
        if(code == EventKeyboard::KeyCode::KEY_BACK)  CPopup::DefaultCallback();
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, this);
}

void CGameScene::startTutorial()
{
    if(m_NeedTutorial)
    {
        m_NeedTutorial = false;
        CObjectManager::Instance()->InitTutorialStep();
    }
}

void CGameScene::freeRewardCheck()
{
    SERVER_REQUEST([=](Json::Value data){
        
        CCLOG("server request succeed");
        auto rewardTimestamp   = CUserDataManager::Instance()->getFreeRewardTimestamp();
        auto currentTimestamp  = time_t(data["seconds"].asDouble());
        auto freeRewardTime    = CFreeRewardManager::Instance()->getFreeRewardTimeLimit();
        auto passedTime        = currentTimestamp - rewardTimestamp;
        if(passedTime > freeRewardTime){
            
            CFreeRewardManager::Instance()->setRewardAble(true);
        }
    }, SERVER_REQUEST_KEY::TIMESTAMP_PHP);
}

void CGameScene::getFreeReward()
{
    SERVER_REQUEST([=](Json::Value data){
        CCLOG("server request succeed");
        auto currentTimestamp  = time_t(data["seconds"].asDouble());
        
        CFreeRewardManager::Instance()->setRewardAble(false);
        
        // reward level up
        CFreeRewardManager::Instance()->FreeRewardLevelUP();
        
        // set time stamp again
        CUserDataManager::Instance()->setFreeRewardTimestamp(currentTimestamp);
        
    }, SERVER_REQUEST_KEY::TIMESTAMP_PHP);
}

long getRemainTime(struct tm* target, struct tm* current){
    auto targetTime  = mktime(target);
    auto currentTime = mktime(current);
    auto remainTime  = time_t(targetTime - currentTime);
    auto tm = gmtime(&remainTime);
    
    auto year = tm->tm_year;
    auto mon  = tm->tm_mon;
    auto day  = tm->tm_mday;
    auto hour = tm->tm_hour;
    auto min  = tm->tm_min;
    auto sec  = tm->tm_sec;
    
    CCLOG("Remain GMT is %d-%d-%d %d : %d : %d", year, mon, day, hour, min, sec);
    
    return remainTime;
}

void CGameScene::dailyGoalResetCheck(bool serverCall/* = false*/)
{
//    bool exist = (CAchievementDataManager::Instance()->getPickedAchievementList().size() > 0);
//    if(exist) return;
    
    auto local   = time_t(time(nullptr));
    auto localtm = gmtime(&local);
    auto lastTimestamp = CUserDataManager::Instance()->getLastTimestamp();
    auto localYear = localtm->tm_year + 1900;
    auto localMon  = localtm->tm_mon  + 1;
    auto localDay  = localtm->tm_mday;
    auto localHour = localtm->tm_hour;
    auto localMin  = localtm->tm_min;
    auto localSec  = localtm->tm_sec;
    
    CCLOG("Current local GMT is %d-%d-%d %d : %d : %d",
          localYear, localMon, localDay,
          localHour, localMin, localSec);

    if((localDay == lastTimestamp) && !serverCall) return;
    
    SERVER_REQUEST([=](Json::Value data){
        auto currentTimestamp = time_t(data["seconds"].asDouble());
        auto tm1              = gmtime(&currentTimestamp);
        
        auto year = tm1->tm_year + 1900;
        auto mon  = tm1->tm_mon  + 1;
        auto day  = tm1->tm_mday;
        auto hour = tm1->tm_hour;
        auto min  = tm1->tm_min;
        auto sec  = tm1->tm_sec;
        
        struct tm tartm;
        tartm.tm_year = year - 1900;
        tartm.tm_mon  = mon  - 1;
        tartm.tm_mday = day  + 1;
        tartm.tm_hour = 0;
        tartm.tm_min  = 0;
        tartm.tm_sec  = 0;
        tartm.tm_isdst = 0;
        auto target = mktime( &tartm);
        auto targettm = localtime( &target);
        CCLOG("Daily reset time is %d-%d-%d %d : %d : %d",
              targettm->tm_year, targettm->tm_mon, targettm->tm_mday,
              targettm->tm_hour, targettm->tm_min, targettm->tm_sec);

        m_DailyResetRemain = getRemainTime(targettm, tm1);
        
        CCLOG("Current server GMT is %d-%d-%d %d : %d : %d", year, mon, day, hour, min, sec);
        CCLOG("Last saved day : %lld", lastTimestamp);
        if(lastTimestamp != day){
            CCLOG("Daily goal reset");
            
            // reset daily achievements
            CAchievementDataManager::Instance()->ResetNormalAchievements();
            CAchievementDataManager::Instance()->getNewAchievements();

            // set time stamp again
            CUserDataManager::Instance()->setLastTimestamp(day);
            
            // notice popup (for debug)
            //            this->CreateAlertPopup()
            //            ->setPositiveButton([=](Node* sender){}, TRANSLATE("BUTTON_OK"))
            //            ->setMessage("normal achievement reseted")
            //            ->show(m_PopupLayer, ZORDER::POPUP);
        }
        
    }, SERVER_REQUEST_KEY::TIMESTAMP_PHP);
}

void CGameScene::facebookRankingResetCheck()
{
    SERVER_REQUEST([=](Json::Value data){
        auto currentTimestamp = time_t(data["seconds"].asDouble());
        auto tm1              = gmtime(&currentTimestamp);
        
        auto year = tm1->tm_year + 1900;
        auto mon  = tm1->tm_mon  + 1;
        auto day  = tm1->tm_mday;
        auto wday = tm1->tm_wday;
        auto hour = tm1->tm_hour;
        auto min  = tm1->tm_min;
        auto sec  = tm1->tm_sec;
        
        struct tm tm2;
        tm2.tm_year = year - 1900;
        tm2.tm_mon  = mon  - 1;
        tm2.tm_mday = (day + ((7 - wday) % 7)) + 1; // 월요일 00시에 초기화
        tm2.tm_hour = 0;
        tm2.tm_min  = 0;
        tm2.tm_sec  = 0;
        tm2.tm_isdst = 0;
        auto target = mktime( &tm2);
        auto targettm = localtime( &target);
        CCLOG("Weekly reset time is %d-%d-%d %d : %d : %d",
              targettm->tm_year, targettm->tm_mon, targettm->tm_mday,
              targettm->tm_hour, targettm->tm_min, targettm->tm_sec);
        
        m_WeeklyResetRemain = getRemainTime(targettm, tm1);
        
        CCLOG("Current server GMT is %d-%d-%d %d : %d : %d", year, mon, day, hour, min, sec);
        
    }, SERVER_REQUEST_KEY::TIMESTAMP_PHP);
}

void CGameScene::startAppreciatePlanet()
{
    this->scheduleOnce([=](float delta){
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::APPRECIATE_PLANET, 600);
    }, 600.f, "START_APPRECIATE_PLANET");
}

void CGameScene::stopAppreciatePlanet()
{
    this->unschedule("START_APPRECIATE_PLANET");
}

// The following items are initialized only once.
void CGameScene::initMemoryPool()
{
#if(USE_MEMORY_POOLING)
    CPoolingManager::Instance()->CreateBulletList(600, 900);
#endif
}

void CGameScene::createFacebookManager()
{
    auto facebook = CFacebookManager::Instance();
    this->addChild(facebook);
}

void CGameScene::createPopupLayer()
{
    if(m_PopupLayer) return;
    
    m_PopupLayer = Layer::create();
    m_PopupLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_PopupLayer->setContentSize(m_VisibleSize);
    m_PopupLayer->setIgnoreAnchorPointForPosition(false);
    m_PopupLayer->setPosition(m_VisibleSize / 2);
    this->addChild(m_PopupLayer, POPUP);
}

void CGameScene::createZoomLayer()
{
    if(m_ZoomLayer) return;
    
    m_ZoomLayer = Layer::create();
    m_ZoomLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_ZoomLayer->setContentSize(m_VisibleSize);
    m_ZoomLayer->setIgnoreAnchorPointForPosition(false);
    m_ZoomLayer->setPosition(m_VisibleSize / 2);
    m_PopupLayer->addChild(m_ZoomLayer, POPUP);
}

void CGameScene::createBulletCreator()
{
    auto bulletCreator = CBulletCreator::create();
    this->addChild(bulletCreator);
    CObjectManager::Instance()->setBulletCreator(bulletCreator);
}

void CGameScene::createBackground()
{
    auto background = CBackGround::create();
    this->addChild(background, ZORDER::BACKGROUND);
    CObjectManager::Instance()->setBackground(background);
}

void CGameScene::createPlanet()
{
    auto planet = CPlanet::create();
    planet->setPosition(m_VisibleSize / 2);
    planet->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_ZoomLayer->addChild(planet, ZORDER::PLANETZ);
    CObjectManager::Instance()->setPlanet(planet);
    CObjectManager::Instance()->ChangePlanet();
}

void CGameScene::createPlayer()
{
    auto player = CPlayer::create();
    player->setPosition(PLAYER_DEFINE::POSITION);
    player->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_ZoomLayer->addChild(player, ZORDER::PLAYER);
    player->setVisible(false);
    CObjectManager::Instance()->setPlayer(player);
    CObjectManager::Instance()->ChangeCharacter();
}

void CGameScene::createRocket()
{
    auto rocket = CRocket::create();
    rocket->setSpeed(ROCKET_DEFINE::SPEED);
    rocket->setDistance(ROCKET_DEFINE::FLYAROUND_DISTANCE);
    rocket->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rocket->setPosition(CBullet::getCirclePosition(90, ROCKET_DEFINE::FLYAWAY_DISTANCE, m_VisibleSize / 2));
    rocket->ChangeState(CFlyToTarget::Instance());
    m_ZoomLayer->addChild(rocket, ZORDER::POPUP);
    CObjectManager::Instance()->setRocket(rocket);
    CObjectManager::Instance()->ChangeRocket();
}

void CGameScene::createSlowPoint()
{
    auto createSlowPoint = [=](Vec2 pos){
        auto slowPoint = LayerColor::create(COLOR::BRIGHT_WHITEGRAY_ALPHA, 80, 80);
        slowPoint->setIgnoreAnchorPointForPosition(false);
        slowPoint->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        slowPoint->setPosition(pos);
        m_ZoomLayer->addChild(slowPoint);
        return slowPoint;
    };
    
    createSlowPoint(Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * 0.78f));
//    createSlowPoint(Vec2(m_VisibleSize.width * 0.4f, m_VisibleSize.height * 0.75f));
}

void CGameScene::createCountDown()
{
    m_CountDown = CCountDown::create()
    ->addLastEventListner([=](Node* sender){
        this->startTutorial();
        CObjectManager::Instance()->setIsGamePause(false);
        CObjectManager::Instance()->setGameStateByLevel();
        m_ZoomLayer->resume();
    })
    ->setFont(Color4B::WHITE, 65)
    ->setMaxNumber(3)
    ->setMinNumber(0)
    ->setLastContent(TRANSLATE("COUNTDOWN_LAST"))
    ->setInterval(0.8f)
    ->setLabelPosition(Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * 0.75f))
    ->setLabelAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(m_PopupLayer, ZORDER::POPUP);
    m_CountDown->Pause();
    
    CTranslateManager::Instance()->addTranslateEventListener([=](){
        m_CountDown->setLastContent(TRANSLATE("COUNTDOWN_LAST"));
    });
}

void CGameScene::createScreenFade()
{
    m_ScreenFade = LayerColor::create(Color4B::BLACK, m_VisibleSize.width, m_VisibleSize.height);
    m_ScreenFade->setIgnoreAnchorPointForPosition(false);
    m_ScreenFade->setPosition(m_VisibleSize / 2);
    m_ScreenFade->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(m_ScreenFade, ZORDER::SCREENFADE);
}

void CGameScene::createItemRanges()
{
    auto createRange = [=](std::string textureName){
        auto range = CItemRange::create()
        ->setTextureName(textureName)
        ->show(m_ZoomLayer, ZORDER::PLAYER);
        range->setPosition(PLAYER_DEFINE::POSITION);
        range->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        return range;
    };
    
    CObjectManager::Instance()->setBarrierItemRange(createRange("barrier2.png"));
    CObjectManager::Instance()->setStarItemRange(createRange("barrier3.png"));
    CObjectManager::Instance()->setCoinItemRange(createRange("barrier3.png"));
    
    
    auto magnetRange = CMagnetEffect::create();
    magnetRange->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    magnetRange->setPosition(PLAYER_DEFINE::POSITION);
    m_ZoomLayer->addChild(magnetRange, ZORDER::PLAYER);
    CObjectManager::Instance()->setMagnetItemRange(magnetRange);
}

void CGameScene::createComboUI()
{
    auto multiscore = CComboScore::Instance();
    m_PopupLayer->addChild(multiscore, ZORDER::POPUP);
//    multiscore->setVisible(false);//ui less
}

void CGameScene::createMenuLayer()
{
    m_MenuLayer = CMenuLayer::Instance()
    ->setBackgroundVisible(false)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
    
    m_MenuLayer->setVisible(false);
    
//    this->OpenGameMenuLayer();
}

void CGameScene::createUILayer()
{
    m_UILayer = CUILayer::Instance()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
    
    m_UILayer->setVisible(false);
}

void CGameScene::createRivalRankLayer()
{
    m_RivalRankLayer = CFacebookRivalRankLayer::create()
    ->setRankUPListener([=](int rank){
        
        if(rank >= 3) return;
        
        // create rival bullet
        auto data   = CFacebookManager::Instance()->getFriendByRank(rank);
        auto bullet = CBulletCreator::CreateBullet('8', random<int>(0, 360), 2700, false);
        bullet->getBulletSprite()->setSpriteFrame(StringUtils::format("rivalBullet_%d.png", rank + 1));
        
        CObjectManager::Instance()->getPlayer()->CrownEnable(true);
        
        // cliper
        auto circleStencil = DrawNode::create();
        circleStencil->drawSolidCircle(Vec2(bullet->getContentSize().width * 0.65f,
                                            bullet->getContentSize().height * 0.5f),
                                            80, 0, 20, 1, 1, Color4F::GREEN);
        
        auto circleClipper = ClippingNode::create(circleStencil);
        circleClipper->setCascadeOpacityEnabled(true);
        circleClipper->setInverted(false);
        bullet->addChild(circleClipper);
        
        auto pic = CUrlSprite::create()
        ->setSize(Size(160, 160))
        ->setUrl(data->_url, data->_url)
        ->setSaveToFileEnable(true)
        ->build(circleClipper, -1);
        pic->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        pic->setPosition(Vec2(bullet->getContentSize().width * 0.65f,
                              bullet->getContentSize().height * 0.5f));
        pic->setRotation(90);
    })
    ->setDefaultCallbackEnable(false)
    ->setBackgroundVisible(false)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * 0.96f))
    ->show(m_UILayer);
}

void CGameScene::createTutorialLayer()
{
    auto tutorialMananger = CTutorialManager::Instance();
    tutorialMananger->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    tutorialMananger->setPosition(m_VisibleSize / 2);
    m_PopupLayer->addChild(tutorialMananger, ZORDER::POPUP);
}

void CGameScene::createBackKeyButton()
{
#if(CC_TARGET_PLATFORM == TARGET_OS_SIMULATOR)
    CMyButton::create()
    ->addEventListener([=](Node* sender){ CPopup::DefaultCallback(); })
    ->setButtonNormalImage("exitButton.png")
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(m_VisibleSize.width * 0.08f, m_VisibleSize.height * 0.1f))
    ->show(this, ZORDER::SCREENFADE);
#endif
}

void CGameScene::createIntroUI()
{
    std::array<Vec2, 9> startPos = {
        Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * 1.1f),
        Vec2(m_VisibleSize.width * 0.5f, 0),
        Vec2(m_VisibleSize.width * 0.5f, 0),
        Vec2(m_VisibleSize.width * 0.5f, 0),
        Vec2(m_VisibleSize.width * 0.5f, 0),
        Vec2(m_VisibleSize.width * 0.5f, 0),
        Vec2(m_VisibleSize.width * 0.5f, 0),
        Vec2(m_VisibleSize.width * 0.5f, 0),
        Vec2(m_VisibleSize.width * 0.5f, 0),
    };
    
    std::array<float, 9> zOrder = {
        8, 7, 6, 5, 4, 3, 2, 1, 0
    };
    
    for(int i = 0; i < 9; i++)
    {
        auto sprite = Sprite::create(StringUtils::format("background_%d.png", i));
        sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        sprite->setPosition(startPos[i]);
        this->addChild(sprite, zOrder[i]);
        m_IntroUIList.push_back(sprite);
    }
}

void CGameScene::createCaptureNode()
{
    auto captureNode = Sprite::create("empty_150x150.png");
    captureNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    captureNode->setPosition(Vec2(-500, -500));
    this->addChild(captureNode);
    CObjectManager::Instance()->setCaptureNode(captureNode);
}

void CGameScene::intro()
{
    m_UILayer->setVisible(false);
    m_ZoomLayer->setPosition(Vec2(m_VisibleSize.width * 0.5f,
                                  m_VisibleSize.height * 1.5f));
    
    std::array<Vec2, 9> targetPos = {
        Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * -0.15f),
        Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * -1.5f),
        Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * -1.5f),
        Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * -1.5f),
        Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * -1.5f),
        Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * -1.5f),
        Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * -1.5f),
        Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * -1.5f),
        Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * -1.5f),
    };
    
    std::array<float, 9> durations = {
        14.0f,
        9.0f,
        9.4f,
        9.8f,
        10.2f,
        10.6f,
        11.0f,
        11.4f,
        12.0f
    };
    
    auto uiListAction = [=](bool skip){
        for(int i = 0; i < m_IntroUIList.size(); i++)
            CObjectManager::Instance()->Intro(m_IntroUIList[i], durations[i], targetPos[i], skip);
    };
    
    auto uiListRemove = [=](){
        for(auto ui : m_IntroUIList)
            ui->removeFromParent();
        m_IntroUIList.clear();
    };
    
    this->createIntroUI();
    CMyButton* skipBtn = CMyButton::create();

    auto introAction = [=](bool skip){
        this->ScreenFade([=](){
            
            uiListAction(skip);
            CObjectManager::Instance()->Intro(m_ZoomLayer, 14.5f, PLANET_DEFINE::MENU_POS, skip, [=](){
                this->menuOpen();
                uiListRemove();
                skipBtn->removeFromParent();
            });
            
        });
    };
    
    skipBtn->addEventListener([=](Node* sender){
        introAction(true);
    })
    ->setEnableSound(false)
    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, m_VisibleSize.width, m_VisibleSize.height))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(m_VisibleSize / 2)
    ->setButtonSingleUse(true)
    ->show(this);
    
    
    introAction(false);
}
