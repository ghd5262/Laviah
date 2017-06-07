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
#include "../MyUI/Popup/CharacterSelectPopup.h"
#include "../MyUI/Popup/GameEndPopup.hpp"
#include "../MyUI/Popup/FacebookAPITestPopup.hpp"
#include "../MyUI/Popup/FacebookRankPopup.hpp"
#include "../MyUI/Popup/FacebookRankUpPopup.hpp"
#include "../MyUI/Popup/AchievementPopup.hpp"
#include "../DataManager/UserDataManager.h"
#include "../DataManager/CharacterDataManager.h"
#include "../DataManager/AchievementDataManager.hpp"
#include "../DataManager/NetworkManager.hpp"
#include "../DataManager/FreeRewardManager.hpp"
#include "../AI/States/RocketStates.h"
#include "../SDKUtil/SDKUtil.h"
#include "../Facebook/FacebookManager.hpp"

#include <array>

USING_NS_CC;

namespace GAMESCENE_DEFINE {
    static int ROTATION_TAG = 1000;
    static std::string RESUME_ROTATION = "RESUME_ROTATION";
}

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
, m_NeedTutorial(false){}

CGameScene::~CGameScene()
{
    CObjectManager::Instance()->RemoveAllObject();
    removeAllChildrenWithCleanup(true); // 이부분 검토
    CPoolingManager::Instance()->DeleteAllMemory();
    CAudioManager::Instance()->Clear();
}

void CGameScene::update(float delta)
{
    if (m_KeyBoardSpace) CObjectManager::Instance()->RotationObject(-2.f);
    
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
    this->createCountDown();
    this->createScreenFade();
    this->createItemRanges();
    this->createComboUI();
    this->createMenuLayer();
    this->createUILayer();
    this->createRivalRankLayer();
    this->createTutorialLayer();
//    this->createIntroUI();
    this->initKeyboardListener();
    this->setTimestamp();
    this->intro();
    
    CAudioManager::Instance()->Clear();
    CAudioManager::Instance()->PlayBGM("sounds/menuBGM.mp3", true);
    
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
//    this->pauseRotation();
    
    m_UILayer->setVisible(true);
    m_UILayer->setDefaultCallbackToTopAgain();
    
    CObjectManager::Instance()->ZoomIn();
    CObjectManager::Instance()->getPlayer()->GameStart();
    CObjectManager::Instance()->getRocket()->ChangeState(CFlyAway::Instance());
    dynamic_cast<CFacebookRivalRankLayer*>( m_RivalRankLayer )->InitListView();
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
    CObjectManager::Instance()->ZoomInRank();    
    this->createResultPopup();
    this->GamePause();
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

void CGameScene::WatchVideo()
{
    this->createVideoPopup();
    this->GamePause();
    m_ZoomLayer->pause();
}

void CGameScene::ShowAchievement()
{
    this->createGoalPopup();
    this->GamePause();
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
        CObjectManager::Instance()->ZoomIn();
        this->menuOpen();
    });
}

void CGameScene::OpenOptionPopup(int scrollIndex/* = 0*/)
{
    //    CObjectManager::Instance()->MoveAction(CGameScene::getZoomLayer(), MOVE_DIRECTION::RIGHT);
    CObjectManager::Instance()->ZoomInRank();
    this->createOptionPopup(scrollIndex);
    this->MenuFadeOut();
}

void CGameScene::OpenWorkshopPopup()
{
    CObjectManager::Instance()->ZoomInRank();
    this->createWorkshopPopup();
    this->MenuFadeOut();
}

void CGameScene::OpenCharacterSelectPopup()
{
    CObjectManager::Instance()->ZoomIn2();
    this->createCharacterSelectPopup();
    this->MenuFadeOut();
}

void CGameScene::OpenRankPopup()
{
    CObjectManager::Instance()->ZoomInRank();
    this->createRankPopup();
    this->MenuFadeOut();
}

void CGameScene::OpenRankUpPopup()
{
    this->createRankUpPopup();
    this->GamePause();
    m_ZoomLayer->pause();
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
    CObjectManager::Instance()->ZoomInRank();
    this->createAchievementPopup();
    this->MenuFadeOut();
}

void CGameScene::RandomCoin()
{
    this->createRandomCoin();
}

void CGameScene::ScreenFade(const FADE_CALLBACK& callback/* = nullptr*/)
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
    });
    auto sequenceAction = Sequence::create(delayAction, fadeAction, callFunc, nullptr);
    m_MenuLayer->runAction(sequenceAction);
    
    // Update the achievement button.
    bool enable = CAchievementDataManager::Instance()->ExistCompletedHiddenAchievement();
    CMenuLayer::Instance()->AchievementButtonState(enable);
    
    if(!enable){
        // Save the index of the last completed achievement
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::LAST_COM_ACHIEVEMENT, 0);
    }
}

void CGameScene::MenuFadeOut()
{
    auto fadeAction     = FadeTo::create(0.3f, 0);
    auto callFunc       = CallFunc::create([=](){
        m_MenuLayer->setVisible(false);
        m_IsMenuLayerFront = false;
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

CPopup* CGameScene::Reward()
{
    CObjectManager::Instance()->MoveAction(MOVE_DIRECTION::DOWN);
    
    return CRewardPopup::create()
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
    GLOBAL->Clear();
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
    CSDKUtil::Instance()->AutoSave();
    
    this->CreateAlertPopup()
    ->setPositiveButton([=](Node* sender){
        Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    }, TRANSLATE("BUTTON_YES"))
    ->setNegativeButton([=](Node* sender){
        if (resume) this->GameResume();
    }, TRANSLATE("BUTTON_NO"))
    ->setDefaultCallback([=](Node* sender){
        if (resume) this->GameResume();
        
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

void CGameScene::createCharacterSelectPopup()
{
    CCharacterSelectPopup::create()
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
    this->getFreeReward();
    this->MenuFadeIn();
    m_UILayer->setVisible(false);
    m_MenuLayer->setDefaultCallbackToTopAgain();
    CObjectManager::Instance()->getRocket()->ComebackHome();
    
//    m_ZoomLayer->scheduleOnce([=](float delta){
//        this->resumeRotation();
//    }, 1.3f, GAMESCENE_DEFINE::RESUME_ROTATION);
}

void CGameScene::turnDownSound()
{
    auto userBGMVolume      = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BGM_VOLUME);
    auto userEffectVolume   = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::EFFECT_VOLUME);
    
    CAudioManager::Instance()->setBGMVolume((userBGMVolume / 100.f) * 0.1f);
    CAudioManager::Instance()->setEffectSoundVolume((userEffectVolume / 100.f) * 0.1f);
}

void CGameScene::turnUpSound()
{
    auto userBGMVolume      = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BGM_VOLUME);
    auto userEffectVolume   = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::EFFECT_VOLUME);
    
    CAudioManager::Instance()->setBGMVolume((userBGMVolume / 100.f));
    CAudioManager::Instance()->setEffectSoundVolume((userEffectVolume / 100.f));
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

void CGameScene::getFreeReward()
{
    SERVER_REQUEST([=](Json::Value data){
        auto rewardTimestamp   = CUserDataManager::Instance()->getFreeRewardTimestamp();
        auto currentTimestamp  = time_t(data["seconds"].asDouble());
        auto freeRewardTime    = CFreeRewardManager::Instance()->getFreeRewardTimeLimit();
        
        if((currentTimestamp - rewardTimestamp) > freeRewardTime){
            CFreeRewardManager::Instance()->setRewardAble(true);
//            CObjectManager::Instance()->getRocket()->ComebackHome();
//            CObjectManager::Instance()->getRocket()->Gift();
            
            // reward level up
            CFreeRewardManager::Instance()->FreeRewardLevelUP();
            
            // set time stamp again
            CUserDataManager::Instance()->setFreeRewardTimestamp(currentTimestamp);
            
            // notice popup (for debug)
            this->CreateAlertPopup()
            ->setPositiveButton([=](Node* sender){}, TRANSLATE("BUTTON_OK"))
            ->setMessage("free reward")
            ->show(m_PopupLayer, ZORDER::POPUP);
        }
        
    }, SERVER_REQUEST_KEY::TIMESTAMP_PHP);
}

void CGameScene::resumeRotation()
{
    if(!m_ZoomLayer) return;
    if(m_ZoomLayer->getActionByTag(GAMESCENE_DEFINE::ROTATION_TAG)) return;
    
    auto rotation = RotateBy::create(120.f, 360);
    auto repeat   = RepeatForever::create(rotation);
    repeat->setTag(GAMESCENE_DEFINE::ROTATION_TAG);
    m_ZoomLayer->runAction(repeat);
}

void CGameScene::pauseRotation()
{
    if(!m_ZoomLayer) return;

    m_ZoomLayer->stopActionByTag(GAMESCENE_DEFINE::ROTATION_TAG);
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
    CObjectManager::Instance()->ChangeCharacter();
    
    auto planet = CPlanet::create();
    planet->setPosition(m_VisibleSize / 2);
    planet->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_ZoomLayer->addChild(planet, ZORDER::PLANET);
    CObjectManager::Instance()->setPlanet(planet);
}

void CGameScene::createPlayer()
{
    auto player = CPlayer::create();
    player->setPosition(Vec2(m_VisibleSize.width * 0.5f, (m_VisibleSize.height * 0.5f) +
                             (PLANET_DEFINE::BOUNDING_RADIUS) +
                             (PLAYER_DEFINE::NORMAL_BOUNDING_RADIUS * 0.8f)));
    player->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_ZoomLayer->addChild(player, ZORDER::PLAYER);
    player->setVisible(false);
    CObjectManager::Instance()->setPlayer(player);
}

void CGameScene::createRocket()
{
    CObjectManager::Instance()->ChangeRocket();
    
    auto rocket = CRocket::create();
    rocket->setSpeed(ROCKET_DEFINE::SPEED);
    rocket->setDistance(ROCKET_DEFINE::FLYAROUND_DISTANCE);
    rocket->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rocket->setPosition(m_VisibleSize);
    rocket->setTargetPos(CBullet::getCirclePosition(90, ROCKET_DEFINE::FLYAWAY_DISTANCE, m_VisibleSize / 2));
    rocket->ChangeState(CFlyToTarget::Instance());
    m_ZoomLayer->addChild(rocket, ZORDER::POPUP);
    CObjectManager::Instance()->setRocket(rocket);
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
    ->setLabelPosition(Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * 0.8f))
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
}

void CGameScene::createComboUI()
{
    auto multiscore = CComboScore::Instance();
    m_PopupLayer->addChild(multiscore, ZORDER::POPUP);
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
        // create rival bullet
        auto data   = CFacebookManager::Instance()->getFriendByRank(rank);
        auto bullet = CBulletCreator::CreateBullet('8', random<int>(0, 360),
                                                   BULLETCREATOR::CREATE_DISTANCE, false);
        
        CUrlSprite::create()
        ->setUrl(data->_url)
        ->setSize(Size(55.f, 55.f))
        ->build(bullet)
        ->setPosition(Vec2(bullet->getContentSize().width,
                           bullet->getContentSize().height * 0.5f));
        
        auto number = Label::createWithTTF(StringUtils::format("%d", rank + 1), FONT::MALGUNBD, 55);
        number->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        number->setPosition(Vec2(bullet->getContentSize().width * 1.5f,
                                 bullet->getContentSize().height * 0.5f));
        bullet->addChild(number);
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
    this->addChild(tutorialMananger, ZORDER::POPUP);
}

void CGameScene::createIntroUI()
{
    std::array<Vec2, 9> startPos = {
        Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * 1.4f),
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

void CGameScene::setTimestamp()
{
    bool exist = (CAchievementDataManager::Instance()->NonCompleteAchievementExist() >= ACHIEVEMENT_DEFINE::LIMIT_COUNT);
    if(exist) return;
    
    SERVER_REQUEST([=](Json::Value data){
        auto lastTimestamp    = CUserDataManager::Instance()->getLastTimestamp();
        auto currentTimestamp = time_t(data["seconds"].asDouble());
        auto tm1              = gmtime(&currentTimestamp);
        auto today            = mktime(tm1);
        
        if((currentTimestamp - lastTimestamp) > 86400){
            // reset daily achievements
            CAchievementDataManager::Instance()->ResetNormalAchievements();
            
            // set time stamp again
            CUserDataManager::Instance()->setLastTimestamp(today);
            
            // notice popup (for debug)
            this->CreateAlertPopup()
            ->setPositiveButton([=](Node* sender){}, TRANSLATE("BUTTON_OK"))
            ->setMessage("normal achievement reseted")
            ->show(m_PopupLayer, ZORDER::POPUP);
        }
        
    }, SERVER_REQUEST_KEY::TIMESTAMP_PHP);
}

void CGameScene::intro()
{
    m_UILayer->setVisible(false);
    m_ZoomLayer->setPosition(Vec2(m_VisibleSize.width * 0.5f,
                                  m_VisibleSize.height * 1.5f));
    
    std::array<Vec2, 9> targetPos = {
        Vec2(m_VisibleSize.width * 0.5f, m_VisibleSize.height * -.3f),
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
        9.0f,
        7.0f,
        7.4f,
        7.8f,
        8.2f,
        8.6f,
        9.0f,
        9.4f,
        10.0f
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
    
    this->ScreenFade([=](){
        
        this->createIntroUI();
        
        CMyButton* skipBtn = CMyButton::create();
        
        auto introAction = [=](bool skip){
            uiListAction(skip);
            CObjectManager::Instance()->Intro(m_ZoomLayer, 12.5f, PLANET_DEFINE::MENU_POS, skip, [=](){            skipBtn->removeFromParent();
                this->menuOpen();
                uiListRemove();
            });
        };
        
        skipBtn->addEventListener([=](Node* sender){
            introAction(true);
        })
        ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
        ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, m_VisibleSize.width, m_VisibleSize.height))
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(m_VisibleSize / 2)
        ->setButtonSingleUse(true)
        ->show(this);
        
        
        introAction(false);
    });
}