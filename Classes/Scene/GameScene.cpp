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
#include "../MyUI/Popup/RevivePopup.hpp"
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
#include "../MyUI/Popup/PlanetSelectPopup.hpp"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/CharacterDataManager.h"
#include "../DataManager/AchievementDataManager.hpp"
#include "../DataManager/AchievementRewarder/AchievementRewarder.hpp"
#include "../DataManager/NetworkManager.hpp"
#include "../DataManager/FreeRewardManager.hpp"
#include "../DataManager/BulletPatternDataManager.h"
#include "../AI/States/RocketStates.h"
#include "../SDKBOX/SDKBoxHeaders.h"
#include "../Download/DownloadManager.h"

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
, m_NeedReview(false)
, m_RewardAble(false)
, m_CheckBox(false)
, m_FirstCountDown(false)
, m_DailyResetRemain(0L)
, m_WeeklyResetRemain(0L)
, m_GameStartTime(0L){}

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
    CObjectManager::Instance()->Execute(delta);
}

bool CGameScene::init()
{
    if (!Layer::init()) return false;
    
    m_GameScene     = this;
    m_VisibleSize   = Director::getInstance()->getVisibleSize();
    m_TouchPos      = m_VisibleSize / 2;
    m_GameStartTime = time_t(time(nullptr));
    auto userDataMng = CUserDataManager::Instance();
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
//    this->createPlanetSelectLayer();
    this->createTutorialLayer();
    this->createCaptureNode();
    this->createBackKeyButton();
    this->initKeyboardListener();
    this->intro();
    this->dailyGoalResetCheck(true);
    this->facebookRankingResetCheck();
    this->schedule([=](float delta){
        if(userDataMng->getUserData_Number(USERDATA_KEY::DATA_SAVE_AUTO))
            userDataMng->SaveUserData(true, true);
    }, 300.f, "AutoSave");
    
    m_NeedTutorial = (userDataMng->getUserData_Number(USERDATA_KEY::TUTORIAL_COUNT) <= 0);
    
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
    CAudioManager::Instance()->PlayBGM("sounds/menuBGM.mp3", false);
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::TITLE);
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
    CObjectManager::Instance()->getPlanet()->GameStart();
    CObjectManager::Instance()->getRocket()->ChangeState(CFlyAway::Instance());
//    CObjectManager::Instance()->getPlanet()->setVisible(true);
    dynamic_cast<CFacebookRivalRankLayer*>( m_RivalRankLayer )->Reset();
    CAudioManager::Instance()->StopBGM();
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::INGAME);
    
    m_FirstCountDown = true;
    
    //        CAudioManager::Instance()->PlayBGM("sounds/bgm_1.mp3", true);
    
    //    });
}

void CGameScene::GameResume()
{
    m_CountDown->Reset();
//    this->turnUpSound();
//    this->resumeSound();
}

void CGameScene::GamePause()
{
    CObjectManager::Instance()->setIsGamePause(true);
//    this->turnDownSound();
    this->pauseSound();
}

void CGameScene::GameResult()
{
    CAudioManager::Instance()->StopBGM();
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
            CGoogleAnalyticsManager::LogScreen(GA_SCREEN::FACEBOOK_LOGIN);
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

void CGameScene::OpenSharePopup(const std::function<void(void)>& callback,
                                cocos2d::Texture2D* texture,
                                int sizeType, bool logo,
                                int score/* = 0*/)
{
    CSharePopup::create()
    ->setCapturedTexture(texture)
    ->setSizeType(SIZE_TYPE(sizeType))
    ->setLogoEnable(logo)
    ->setScore(score)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
    
    CShareManager::Instance()->setExitCallback(callback);
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
        this->createReviewPopup();
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
                        std::string title,
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
    ->setTitle(title)
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
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::PAUSE);
    CPausePopup::create()
    ->setBackgroundColor(COLOR::DARKGRAY_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createVideoPopup()
{
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::VIDEO);
    CRevivePopup::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createGoalPopup()
{
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::GOAL);
    CGoalPopup::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createResultPopup()
{
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::RESULT);
    CResultPopup::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createEndPopup()
{
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::END);
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
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::OPTION);
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
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::WORKSHOP);
    CWorkshopPopup::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createCharacterPopup()
{
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::CHARACTER);
    CCharacterPopup::create()
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_PopupLayer, ZORDER::POPUP);
}

void CGameScene::createCostumePopup(const VOID_CALLBACK& callback,
                                    int index)
{
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::COSTUME);
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
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::FACEBOOK_RANK);
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
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::ACHIEVEMENT);
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
//    auto data = CBulletPatternDataManager::Instance()->getRandomConstellationPatternByLevel(1, true);
//    CBulletCreator::CreateConstellation(data);
}

void CGameScene::menuOpen()
{
    this->clearData();
    //        this->createRandomCoin();
    this->freeRewardCheck();
    this->dailyGoalResetCheck();
    this->MenuFadeIn();
//    this->turnUpSound();
    this->resumeSound();
    
    m_UILayer->setVisible(false);
    m_MenuLayer->setDefaultCallbackToTopAgain();
    CObjectManager::Instance()->getRocket()->ComebackHome();
    CObjectManager::Instance()->getPlanet()->StartRotation();
//    CObjectManager::Instance()->getPlanet()->setVisible(false);
    CObjectManager::Instance()->getPlayer()->setVisible(false);
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::MENU);
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

void CGameScene::pauseSound()
{
    CAudioManager::Instance()->AllPause();
}

void CGameScene::resumeSound()
{
    CAudioManager::Instance()->AllResume();
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
        auto currentTimestamp  = time_t(data["currentSeconds"].asDouble());
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
        auto currentTimestamp  = time_t(data["currentSeconds"].asDouble());
        
        CFreeRewardManager::Instance()->setRewardAble(false);
        
        // reward level up
        CFreeRewardManager::Instance()->FreeRewardLevelUP();
        
        // set time stamp again
        CUserDataManager::Instance()->setFreeRewardTimestamp(currentTimestamp);
        
    }, SERVER_REQUEST_KEY::TIMESTAMP_PHP);
}

void CGameScene::getRankReward(){
    SERVER_REQUEST([=](Json::Value data){
        auto newTimestamp = data["weeklyResetSeconds"].asDouble();
        
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::RANK, 30);
        
        // set time stamp again
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::WEEKLY_RESET_SAVED_TIME, newTimestamp);
        
        m_RewardAble = false;
        
    }, SERVER_REQUEST_KEY::TIMESTAMP_PHP);
}

void dailyGoalResetResponse(Json::Value data)
{
    auto lastTimestamp = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::DAILY_RESET_SAVED_TIME);
    auto newTimestamp = data["dailyResetSeconds"].asDouble();
    auto remainTime   = data["dailyResetRemains"].asDouble();

    CCLOG("Daily reset remain seconds is %lf", remainTime);
    CGameScene::getGameScene()->setDailyResetRemain(remainTime);
    
    if(lastTimestamp != newTimestamp){
        CCLOG("Daily goal reset");
        
        // reset daily achievements
        CAchievementDataManager::Instance()->ResetNormalAchievements();
        CAchievementDataManager::Instance()->getNewAchievements();
        
        // set time stamp again
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::DAILY_RESET_SAVED_TIME, newTimestamp);
        
        // notice popup (for debug)
        //            this->CreateAlertPopup()
        //            ->setPositiveButton([=](Node* sender){}, TRANSLATE("BUTTON_OK"))
        //            ->setMessage("normal achievement reseted")
        //            ->show(m_PopupLayer, ZORDER::POPUP);
    }
}

void CGameScene::dailyGoalResetCheck(bool serverCall/* = false*/)
{
//    bool exist = (CAchievementDataManager::Instance()->getPickedAchievementList().size() > 0);
//    if(exist) return;
    
    auto seconds = time_t(time(nullptr));
    auto tm   = gmtime(&seconds);
    auto lastTimestamp = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::DAILY_RESET_SAVED_TIME);
    auto year = tm->tm_year + 1900;
    auto mon  = tm->tm_mon  + 1;
    auto day  = tm->tm_mday;
    auto hour = tm->tm_hour;
    auto min  = tm->tm_min;
    auto sec  = tm->tm_sec;
    auto resetDay = day + 1;
    
    std::string timeString = StringUtils::format("%d-%d-%d %d:%d:%d", year, mon, resetDay, 0, 0, 0);
    
    CCLOG("Current local GMT is %d-%d-%d %d:%d:%d", year, mon, day, hour, min, sec);
    CCLOG("local daily reset GMT is %s", timeString.c_str());
    CCLOG("Last saved day is %d", lastTimestamp);

    struct tm tartm;
    strptime(timeString.c_str(),"%Y-%m-%d %H:%M:%S",&tartm);
    auto target = mktime(&tartm) + tartm.tm_gmtoff;

    if((target == lastTimestamp) && !serverCall) return;
    
    SERVER_REQUEST([=](Json::Value data){
        dailyGoalResetResponse(data);
    }, SERVER_REQUEST_KEY::TIMESTAMP_PHP);
}

void weeklyRankingResetResponse(Json::Value data){

    auto lastTimestamp = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::WEEKLY_RESET_SAVED_TIME);
    auto gameScene    = CGameScene::getGameScene();
    auto newTimestamp = data["weeklyResetSeconds"].asDouble();
    auto remainTime   = data["weeklyResetRemains"].asDouble();
    
    CCLOG("Weekly reset remain seconds is %lf", remainTime);
    gameScene->setWeeklyResetRemain(remainTime);
    
    if(lastTimestamp != newTimestamp){
        CCLOG("Facebook rank reset");
        
        if(CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::RANK) < 3)
            gameScene->setRewardAble(true);
        else gameScene->getRankReward();
    }
}

void CGameScene::facebookRankingResetCheck()
{
    SERVER_REQUEST([=](Json::Value data){
        weeklyRankingResetResponse(data);
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

void createCheckbox(Node* parent)
{
    auto checkbox = cocos2d::ui::CheckBox::create("autoSaveButton_0.png", "autoSaveButton_1.png");
    checkbox->addEventListener([=](Ref* sender, cocos2d::ui::CheckBox::EventType event){
        auto check = false;
        if (cocos2d::ui::CheckBox::EventType::SELECTED == event)        check = true;
        else if (cocos2d::ui::CheckBox::EventType::UNSELECTED == event) check = false;
        
        CGameScene::getGameScene()->setCheckBox(check);
    });
    checkbox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    checkbox->setPosition(Vec2(parent->getContentSize().width * 0.09f,
                               parent->getContentSize().height * 0.33f));
    parent->addChild(checkbox);
    
    auto label = Label::createWithSystemFont(TRANSLATE("BUTTON_DO_NOT_AGAIN"), FONT::MALGUN, 40);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setPosition(Vec2(parent->getContentSize().width * 0.12f,
                            parent->getContentSize().height * 0.33f));
    label->enableOutline(Color4B::BLACK, 2);
    parent->addChild(label);
}

void CGameScene::createReviewPopup()
{
    auto showable    = (CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::REVIEW) != 1);
    if(!showable)       return;
    if(!m_NeedReview)   return;
    if(!m_IsMenuLayerFront) return;
    if(!m_MenuLayer->isVisible()) return;
    
    auto storeURL    = CDownloadManager::Instance()->getAppUrl();
    auto facebookURL = CDownloadManager::Instance()->getFacebookPageLink();
    auto gaLogSend   = [=](std::string action){
        CGoogleAnalyticsManager::LogEventAction(GA_CATEGORY::REVIEW, action);
    };
    m_NeedReview     = false;
    
    
    this->CreateAlertPopup()
    ->setPositiveButton([=](Node* sender){
        
        auto reviewPopup = this->CreateAlertPopup()
        ->setPositiveButton([=](Node* sender){
            gaLogSend(GA_ACTION::REVIEW_LOVE_YES);
            CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::REVIEW, 1);
            Application::getInstance()->openURL(storeURL);

        }, TRANSLATE("BUTTON_THANKYOU"))
        ->setNegativeButton([=](Node* sender){
            if(m_CheckBox){
                gaLogSend(GA_ACTION::REVIEW_LOVE_NEVER);
                CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::REVIEW, 1);
            }else{
                gaLogSend(GA_ACTION::REVIEW_LOVE_NO);
            }
        }, TRANSLATE("BUTTON_LATER"))
        ->setMessage(TRANSLATE("REVIEW_POPUP_TEXT_2"))
        ->show(m_PopupLayer, ZORDER::POPUP);
        
        createCheckbox(reviewPopup);
        
    }, TRANSLATE("BUTTON_YES"))
    ->setNegativeButton([=](Node* sender){
        
        auto facebookPopup = this->CreateAlertPopup()
        ->setPositiveButton([=](Node* sender){
            gaLogSend(GA_ACTION::REVIEW_HATE_YES);
            Application::getInstance()->openURL(facebookURL);
            
        }, TRANSLATE("BUTTON_THANKYOU"))
        ->setNegativeButton([=](Node* sender){
            if(m_CheckBox){
                gaLogSend(GA_ACTION::REVIEW_HATE_NEVER);
                CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::REVIEW, 1);
            }else{
                gaLogSend(GA_ACTION::REVIEW_HATE_NO);
            }
        }, TRANSLATE("BUTTON_LATER"))
        ->setMessage(TRANSLATE("REVIEW_POPUP_TEXT_3"))
        ->show(m_PopupLayer, ZORDER::POPUP);
        
        createCheckbox(facebookPopup);
        
    }, TRANSLATE("BUTTON_NO"))
    ->setMessage(TRANSLATE("REVIEW_POPUP_TEXT_1"))
    ->show(m_PopupLayer, ZORDER::POPUP);
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
    rocket->setVisible(false);
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
        if(m_FirstCountDown){
            CAudioManager::Instance()->PlayBGM("sounds/testBGM.mp3", true, false);
            CObjectManager::Instance()->setGameStateByLevel();
            m_FirstCountDown = false;
        }
        this->resumeSound();
        this->startTutorial();
        CObjectManager::Instance()->setIsGamePause(false);
        m_ZoomLayer->resume();
        
//        if(GVALUE->REVIVED == 1)
//        {
//            auto list = std::vector<int>{ eITEM_TYPE_shield, eITEM_TYPE_coin,
//                eITEM_TYPE_star, eITEM_TYPE_giant
//            };
//            auto randomIndex  = random<int>(0, (int)list.size()-1);
//            auto randomItem   = (eITEM_TYPE)list.at(randomIndex);
//            auto metaDataName = StringUtils::format("REVIVE_ITEM_TIME_%d", (int)randomItem);
//            auto itemTime     = META_DATA(metaDataName).asDouble();
//            CItemManager::Instance()->StartItemTimer(randomItem, itemTime);
//            GVALUE->REVIVED = 0;
//        }
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
    auto createRange = [=](std::string textureName, float distance, float duration){
        auto range = CItemRange::create()
        ->setTargetDistance(distance)
        ->setTargetDuration(duration)
        ->setTextureName(textureName)
        ->show(m_ZoomLayer, ZORDER::PLAYER);
        range->setPosition(PLAYER_DEFINE::POSITION);
        range->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        return range;
    };
    
    CObjectManager::Instance()->setBarrierItemRange(createRange("barrier2.png", 1600.f, 0.2f));
    CObjectManager::Instance()->setStarItemRange(createRange("barrier3.png", 1600.f, 0.2f));
    CObjectManager::Instance()->setCoinItemRange(createRange("barrier3.png", 1600.f, 0.2f));
    
    
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
        
        if(rank == 0)
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

void CGameScene::createPlanetSelectLayer()
{
    CPlanetSelectPopup::create()
    ->setDefaultCallbackEnable(false)
    ->setBackgroundVisible(false)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(m_VisibleSize / 2)
    ->show(m_MenuLayer, ZORDER::POPUP);
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
