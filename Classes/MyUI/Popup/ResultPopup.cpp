#include "ResultPopup.h"
#include "RewardPopup.h"
#include "SharePopup.hpp"
#include "../MyButton.h"
#include "../ScoreUI.h"
#include "../UserCoinButton.h"
#include "../ComboScore.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/FreeRewardManager.hpp"
#include "../../DataManager/AchievementDataManager.hpp"
#include "../../DataManager/AchievementRewarder/AchievementRewarder.hpp"
#include "../../DataManager/UserLevelDataManager.hpp"
#include "../../DataManager/PlanetDataManager.hpp"
#include "../../DataManager/CostumeDataManager.hpp"
#include "../../Common/StringUtility.h"
#include "../../Scene/GameScene.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Player.h"
#include "../../SDKBOX/SDKBoxHeaders.h"
#include "../../Download/DownloadManager.h"
#include <array>

CResultPopup::~CResultPopup(){
    CCLOG("ResultPopup destroied");
}

CResultPopup* CResultPopup::create()
{
    CResultPopup *pRet = new(std::nothrow) CResultPopup();
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

bool CResultPopup::init()
{
    if (!CPopup::init()) return false;
    
    m_BG = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
    if (m_BG != nullptr){
        m_BG->setIgnoreAnchorPointForPosition(false);
        m_BG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_BG->setPosition(Vec2(this->getContentSize().width * 0.5f,
                               this->getContentSize().height * 1.5f));
        this->addChild(m_BG);
    }
    auto layerSize = m_BG->getContentSize();
    
    std::array<Vec2, 8> posDown  = {
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.4f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.3f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.2f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.1f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.0f),
        Vec2(layerSize.width * 0.5f, layerSize.height * -.1f),
        Vec2(layerSize.width * 0.5f, layerSize.height * -.2f),
        Vec2(layerSize.width * 0.5f, layerSize.height * -.5f),
    };
    
    std::array<Vec2, 7> posArray = {
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.7f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.65f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.6f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.55f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.45f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.4f),
    };
    
    std::array<std::string, 4> resultIcon = {
        "starIcon.png",
        "comboIcon.png",
        "coinIcon.png",
        "achievementIcon.png"
    };
    
    std::array<std::string, 4> resultContent = {
        TRANSLATE("RESULT_SCORE"),
        TRANSLATE("RESULT_COMBO"),
        TRANSLATE("RESULT_COIN"),
        TRANSLATE("RESULT_GOAL")
    };
    
    this->userDataUpdate();
    this->createScoreLayer(resultIcon[0], resultContent[0], GVALUE->STAR_SCORE);
    this->createScoreLayer(resultIcon[1], resultContent[1], GVALUE->BEST_COMBO);
    this->createBonusScoreLayer(resultIcon[2], resultContent[2], GVALUE->COIN_COUNT, 10);
    this->createBonusScoreLayer(resultIcon[3], resultContent[3], GVALUE->NORMAL_ACHIEVEMENT_CLEAR_COUNT, 100);
    this->createTotalScoreLayer(GVALUE->TOTAL_SCORE);
    this->createRankingLayer();
    this->createLevelLayer();
    
    // save exp - should be save after create level layer.
    {
        CUserDataManager::Instance()->ExpAdd(GVALUE->TOTAL_SCORE);
    }
    
    for(int index = 0; index < m_ScoreLayerList.size(); index++)
        m_ScoreLayerList.at(index)->setPosition(posArray[index]);
    
    // create button lambda
    auto createButton   = [=](const std::function<void(Node*)> &callback, std::string name, Vec2 pos, bool visible, bool use = true){
        auto button = CMyButton::create()
        ->addEventListener(callback)
        ->setButtonSingleUse(use)
        ->setButtonNormalImage(name)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(pos)
        ->show(this);
        button->setVisible(visible);
        return button;
    };
    
    std::array<Vec2, 3> btnPosArray = {
        Vec2(layerSize.width * 0.08f, layerSize.height * 0.05f),
        Vec2(layerSize.width * 0.92f, layerSize.height * 0.05f),
        Vec2(layerSize.width * 0.92f, layerSize.height * 0.05f),
    };
    
    std::array<std::string, 3> btnIconArray = {
        "homeIcon.png",
        "resetIcon.png",
        "endIcon.png",
    };
    
    std::array<std::function<void(Node*)>, 3> btnListenerArray = {
        [=](Node* sender) { this->home();             },
        [=](Node* sender) { this->reset();            },
        [=](Node* sender) { this->end();              }
    };
    
    std::array<bool, 3> btnVisibleArray = {
        (!m_GoalPopupOpen),
        (!m_GoalPopupOpen),
        ( m_GoalPopupOpen)
    };
    
    // create reward button
    {
        auto costumeCost = META_DATA("COSTUME_COST").asInt();
        std::array<bool, 4> rewardBtnVisibleArray = {
            ( random<int>(0, 1) == 1 ),
            ( (CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::COIN) >= costumeCost) &&
              (CCostumeDataManager::Instance()->getNewRandomCostume()) &&
              (random<int>(0, 1) == 1) ),
            ( CFreeRewardManager::Instance()->getRewardAble() ),
            ( GVALUE->HIDDEN_ACHIEVEMENT_CLEAR_COUNT > 0 )
        };
        
        std::array<std::string, 4> rewardBtnIconArray = {
            "unityAdsIcon.png",
            "costumeUFOIcon.png",
            "freeCoinIcon.png",
            "achievementCupIcon.png"
        };
        
        std::array<std::function<void(Node*)>, 4> rewardBtnListenerArray = {
            [=](Node* sender) { this->getCoinFromVideo(sender); },
            [=](Node* sender) { this->getNewCostume(sender);    },
            [=](Node* sender) { this->getFreeReward();          },
            [=](Node* sender) { this->openAchievementPopup();   }
        };
        
        std::vector<Node*> rewardBtnArray;
        for(int index = 0; index < rewardBtnVisibleArray.size(); index++)
        {
            if(!rewardBtnVisibleArray[index]) continue;
            
            auto rewardBtn = CMyButton::create()
            ->addEventListener(rewardBtnListenerArray[index])
            ->setButtonSingleUse(true)
            ->setButtonNormalImage(rewardBtnIconArray[index])
            ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
            ->setButtonPosition(Vec2(layerSize.width * 0.5f, layerSize.height * -0.5f))
            ->show(m_BG);
            rewardBtn->setVisible(false);

            rewardBtnArray.emplace_back(rewardBtn);
        }
        
        if(rewardBtnArray.size() > 0){
            auto rewardBtn = rewardBtnArray[rewardBtnArray.size()-1];
            m_ScoreLayerList.emplace_back(rewardBtn);
            CRewardPopup::createFlyAction(rewardBtn,
                                          Vec2(layerSize.width * 0.5f, layerSize.height * 0.25f),
                                          Vec2(layerSize.width * 0.5f, layerSize.height * 0.2f));
        }
    }
    
    
    // create button array
    std::array<CMyButton*, 3> btnArray;
    
    // create buttons
    for(int i = 0; i < 3; i++)
        btnArray[i] = createButton(btnListenerArray[i], btnIconArray[i], btnPosArray[i], btnVisibleArray[i]);
    
    
    // create result label
    auto resultLabel = Label::createWithSystemFont(TRANSLATE("RESULT_POPUP_TITLE"), FONT::MALGUNBD, 80);
    resultLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    resultLabel->setPosition(Vec2(layerSize.width * 0.5f,
                                  layerSize.height * 0.8f));
    this->addChild(resultLabel);
    
    
    // create user coin layer
    auto btnUserCoin = CUserCoinButton::create();
    btnUserCoin->setPosition(Vec2(layerSize.width * 0.5f,
                                  layerSize.height * 0.05f));
    btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(btnUserCoin);
    
    
    // create captured picture
    auto pictureBtn = createButton([=](Node* sender){
        this->share();
    }, "shareIcon_1.png", Vec2(layerSize.width * 0.2f, layerSize.height * 0.05f), true, false);
    
    auto captureBack = LayerColor::create(Color4B::WHITE, layerSize.width, layerSize.height);
    captureBack->setIgnoreAnchorPointForPosition(false);
    captureBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    captureBack->setPosition(Vec2(pictureBtn->getContentSize().width * 0.8f,
                                  pictureBtn->getContentSize().height * 0.8f));
    captureBack->setRotation(-45);
    captureBack->setScale(0.11f, 0.1f);
    captureBack->setCascadeOpacityEnabled(true);
    pictureBtn->addChild(captureBack, -1);
    
    auto captureNode = CObjectManager::Instance()->getCaptureNode();
    auto copiedNode  = Sprite::createWithTexture(captureNode->getTexture());
    copiedNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    copiedNode->setScale(0.85f, -0.9f);
    copiedNode->setPosition(captureBack->getContentSize() / 2);
    copiedNode->setCascadeOpacityEnabled(true);
    captureBack->addChild(copiedNode);
    
    
    this->setOpenAnimation([=](Node* sender){
        // clear all bullets
        CObjectManager::Instance()->ReturnToMemoryBlockAll();
        
        auto moveAction = MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f));
        auto easeAction = EaseExponentialInOut::create(moveAction);
        m_BG->runAction(easeAction);
        
        
        auto action = [=](Node* owner){
            auto delay = DelayTime::create(1.f);
            auto fade  = FadeIn::create(0.5f);
            auto sequence = Sequence::createWithTwoActions(delay, fade);
            owner->setOpacity(0);
            owner->runAction(sequence);
        };
        
        for(auto btn : btnArray)
            action(btn);
        
        action(resultLabel);
        action(btnUserCoin);
        action(pictureBtn);
    }, 1.2f);
    
    this->setCloseAnimation([=](Node* sender){
        
        for(auto btn : btnArray)
            btn->runAction(FadeTo::create(0.3f, 0));
        
        resultLabel->runAction(FadeTo::create(0.3f, 0));
        btnUserCoin->runAction(FadeTo::create(0.3f, 0));
        pictureBtn->runAction(FadeTo::create(0.3f, 0));
        
        if( m_GoalPopupOpen ){
            auto action = [=](Node* sprite, Vec2 pos){
                auto move       = MoveTo::create(0.35f, pos);
                auto sine       = EaseSineIn::create(move);
                auto fadeout    = FadeTo::create(0.2f, 0);
                auto spawn      = Spawn::createWithTwoActions(sine, fadeout);
                
                sprite->runAction(spawn);
            };
            
            for(int index = 0; index < m_ScoreLayerList.size(); index++){
                action(m_ScoreLayerList.at(index), posDown.at(index));
            }
            
            
        }
        else {
            auto move = MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f,
                                                  layerSize.height * 1.5f));
            auto ease = EaseExponentialInOut::create(move);
            m_BG->runAction(ease);
        }
    });
    
    // if there are some reward from achievement. open reward popup after end of result popup.
    if ( m_GoalPopupOpen ) this->setDefaultCallback([=](Node* sender){ this->end();  });
    else                   this->setDefaultCallback([=](Node* sender){ this->home(); });
    
    return true;
}

void CResultPopup::reset(){
    CGameScene::getGameScene()->GameStart();
    this->exit();
}

void CResultPopup::home(){
    CGameScene::getGameScene()->OpenGameMenuLayer();
    this->exit();
}

void CResultPopup::end(){
    CGameScene::getGameScene()->OpenGoalPopup();
    this->exit();
}

void CResultPopup::getCoinFromVideo(Node* sender)
{
    CUnityAdsManager::Instance()->ShowUnityAds([=](){
        auto coin = META_DATA("BONUS_COIN").asInt();
        this->createRewardPopup(TRANSLATE("REWARD_TITLE_BONUS_COIN"),
                                ACHIEVEMENT_REWARD_KEY::REWARD_COIN_RANDOM, coin);
        
        CGoogleAnalyticsManager::LogEventAction(GA_CATEGORY::WATCH_ADS, GA_ACTION::ADS_BONUS);
        CGoogleAnalyticsManager::LogScreen(GA_SCREEN::REWARD_BONUS);
        CGoogleAnalyticsManager::LogEventCoin(GA_ACTION::COIN_GET_BONUS, coin);
    });
    CUnityAdsManager::Instance()->setUnityAdsFailedCallback([=](){
        auto button = dynamic_cast<CMyButton*>(sender);
        if(!button) return;
        
        button->setTouchEnable(true);
    });
}

void CResultPopup::getNewCostume(Node* sender)
{
    auto cost = META_DATA("COSTUME_COST").asInt();
    CGameScene::getGameScene()->Reward([=](bool isPlay){
        if(isPlay) {
            this->popupClose();
            return;
        }
        
        auto button = dynamic_cast<CMyButton*>(sender);
        if(!button) return;
        button->setTouchEnable(true);
    }, {}, "", -cost, true);
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::REWARD_COSTUME);
}

void CResultPopup::getFreeReward()
{
    auto coin = META_DATA("FREE_REWARD_COIN").asInt();
    auto next = CFreeRewardManager::Instance()->getNextRewardRemainTime();
    time_t sec_t = next;
    struct tm* time;
    time = gmtime(&sec_t);
    std::string result = TRANSLATE("REWARD_TITLE_FREE_COIN_NEXT_1");
    
    if(time->tm_hour > 0)
        result += StringUtils::format(" %d ", time->tm_hour) + TRANSLATE("TIME_HOUR");
    if(time->tm_min > 0)
        result += StringUtils::format(" %d ", time->tm_min) + TRANSLATE("TIME_MIN");
    
    result += " " + TRANSLATE("REWARD_TITLE_FREE_COIN_NEXT_2");
    
    auto title = TRANSLATE("REWARD_TITLE_FREE_COIN") + "-" + result;

    this->createRewardPopup(title, ACHIEVEMENT_REWARD_KEY::REWARD_COIN_RANDOM, coin);
    CGameScene::getGameScene()->getFreeReward();
    
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::REWARD_FREE);
}

void CResultPopup::openAchievementPopup()
{
    this->home();
    CGameScene::getGameScene()->scheduleOnce([=](float delta){
        CGameScene::getGameScene()->OpenAchievementPopup();
    }, 1.f, "OpenAchievementPopup");
}

void CResultPopup::share()
{
    auto node = CObjectManager::Instance()->getCaptureNode();
    auto text = StringUtils::format(TRANSLATE("SCORE_SHARE_TEXT").c_str(), GVALUE->TOTAL_SCORE);
    CShareManager::Instance()->setShareText(text);
    CShareManager::Instance()->setShareTitle(TRANSLATE("SCORE_SHARE_TITLE"));
    
    CGoogleAnalyticsManager::LogScreen(GA_SCREEN::SHARE_SCORE);
    CGameScene::getGameScene()->OpenSharePopup([=](){
        CGoogleAnalyticsManager::LogEventAction(GA_CATEGORY::SHARE, GA_ACTION::SHARE_SCORE);
    }, node->getTexture(), SIZE_TYPE::FULL_SIZE, true, GVALUE->TOTAL_SCORE);
}

void CResultPopup::exit()
{
    if(m_GoalPopupOpen) this->popupClose();
    else                this->popupClose(1.3f);
}

void CResultPopup::createRewardPopup(std::string title, std::string key, int value)
{
    CGameScene::getGameScene()->Reward([=](bool isPlay){}, {
        sREWARD_DATA(key, value)
    }, title);
}

cocos2d::Node* CResultPopup::createIconLayer(std::string iconName, std::string text)
{
    auto layer = Sprite::create("resultPopup_2.png");
    layer->setCascadeOpacityEnabled(true);
    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_BG->addChild(layer);
    
    auto layerSize = layer->getContentSize();
    
    auto icon = Sprite::create(iconName);
    icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    icon->setPosition(Vec2(layerSize.width * 0.1f, layerSize.height * 0.5f));
    icon->setScale(100 / icon->getContentSize().height);
    layer->addChild(icon);
    
    auto textLabel = Label::createWithSystemFont(text, FONT::MALGUNBD, 50);
    textLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    textLabel->setPosition(Vec2(layerSize.width * 0.15f,  layerSize.height * 0.5f));
    layer->addChild(textLabel);
    
    return layer;
}

void CResultPopup::createScoreLayer(std::string iconName, std::string text, int value)
{
    auto layer      = this->createIconLayer(iconName, text);
    auto valueLabel = Label::createWithTTF(StringUtility::toCommaString(value), FONT::MALGUNBD, 50);
    valueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    valueLabel->setPosition(Vec2(layer->getContentSize().width * 0.9f,
                                 layer->getContentSize().height * 0.5f));
    layer->addChild(valueLabel);
    
    m_ScoreLayerList.emplace_back(layer);
}

void CResultPopup::createBonusScoreLayer(std::string iconName, std::string text,
                                         int value, int bonus)
{
    auto layer         = this->createIconLayer(iconName, text);
    auto layerSize     = layer->getContentSize();
    
    auto valueLabel    = Label::createWithTTF(StringUtility::toCommaString(value), FONT::MALGUNBD, 50);
    valueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    valueLabel->setPosition(Vec2(layerSize.width * 0.9f, layerSize.height * 0.5f));
    layer->addChild(valueLabel);
    
    auto labelSize     = valueLabel->getContentSize();
    auto bonusLebelPos = Vec2(valueLabel->getPosition().x - labelSize.width, layerSize.height * 0.4f);
    auto bonusStr      = StringUtils::format("%s x  ", StringUtility::toCommaString(bonus).c_str());
    auto bonusLabel    = Label::createWithTTF(bonusStr, FONT::MALGUNBD, 25);
    bonusLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    bonusLabel->setPosition(bonusLebelPos);
    layer->addChild(bonusLabel);
    
    m_ScoreLayerList.emplace_back(layer);
}

void CResultPopup::createTotalScoreLayer(int value)
{
    auto bestScore   = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BEST_SCORE);
    auto isBestScore = (GVALUE->TOTAL_SCORE > bestScore);
    
    auto layer = Sprite::create("resultPopup_1.png");
    layer->setCascadeOpacityEnabled(true);
    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_BG->addChild(layer);
    
    auto layerSize = layer->getContentSize();
    
    auto textLabel = Label::createWithSystemFont(TRANSLATE("RESULT_TOTAL_SCORE"), FONT::MALGUNBD, 60);
    textLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    textLabel->setPosition(Vec2(layerSize.width * 0.1f,  layerSize.height * 0.5f));
    layer->addChild(textLabel);
    
    if(isBestScore){
        this->createChangeLabelAction(textLabel, TRANSLATE("RESULT_TOTAL_SCORE"),
                                      TRANSLATE("RESULT_BEST_SCORE"));
    }
    
    auto valueLabel = Label::createWithTTF(StringUtility::toCommaString(value), FONT::MALGUNBD, 60);
    valueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    valueLabel->setPosition(Vec2(layerSize.width * 0.9f,  layerSize.height * 0.5f));
    layer->addChild(valueLabel);
    
    m_ScoreLayerList.emplace_back(layer);
}

void CResultPopup::createRankingLayer()
{
    auto layer = Sprite::create("resultPopup_2.png");
    layer->setCascadeOpacityEnabled(true);
    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_BG->addChild(layer);
    
    auto layerSize = layer->getContentSize();
    
    auto icon = Sprite::create("googleLeaderboardsIcon.png");
    icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    icon->setPosition(Vec2(layerSize.width * 0.1f, layerSize.height * 0.5f));
    icon->setScale(100 / icon->getContentSize().height);
    layer->addChild(icon);
    
    auto textLabel = Label::createWithSystemFont("", FONT::MALGUNBD, 50);
    textLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    textLabel->setPosition(Vec2(layerSize.width * 0.15f,  layerSize.height * 0.5f));
    layer->addChild(textLabel);
    
    auto valueLabel = Label::createWithTTF("", FONT::MALGUNBD, 50);
    valueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    valueLabel->setPosition(Vec2(layer->getContentSize().width * 0.9f,
                                 layer->getContentSize().height * 0.5f));
    layer->addChild(valueLabel);
    
    
    if(CPlayManager::Instance()->IsLoggedIn()){
        auto planet = CObjectManager::Instance()->getPlanetParam();
        auto key    = planet->_leaderboard;
        
        this->retain();
        
        CPlayManager::Instance()->ScoreLoad([=](){
            
            auto data      = CPlayManager::Instance()->getLeaderboardData(key);
            auto bestScore = data->_allTimeScore;
            auto ranking   = data->_rank;
            this->createChangeLabelAction(textLabel, TRANSLATE("RESULT_RANK"), TRANSLATE("RESULT_BEST_SCORE"));
            this->createChangeLabelAction(valueLabel, StringUtility::toCommaString(ranking),
                                          StringUtility::toCommaString(bestScore));
            
            this->release();
        }, key, GVALUE->TOTAL_SCORE, sdkbox::TIME_SCOPE::ALL_TIME);
        
    }
    else{
        auto bestScore = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BEST_SCORE);
        textLabel->setString(TRANSLATE("RESULT_BEST_SCORE"));
        valueLabel->setString(StringUtility::toCommaString(bestScore));
    }
    m_ScoreLayerList.emplace_back(layer);
}

void CResultPopup::createLevelLayer()
{
    auto createBar    = [=](Node* parent, Color4B color){
        auto bar      = Sprite::create("expProgress.png");
        bar->setColor(Color3B(color.r, color.g, color.b));
        auto progress = ProgressTimer::create(bar);
        progress->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        progress->setPosition(parent->getContentSize() / 2);
        progress->setMidpoint(Vec2(0, 0));
        progress->setType(ProgressTimer::Type::BAR);
        progress->setBarChangeRate(Vec2(1, 0));
        progress->setOpacity(color.a);
        progress->setCascadeOpacityEnabled(true);
        parent->addChild(progress, -1);
        return progress;
    };
    auto getPercent   = [=](float value, float max){
        if(value != 0 && max != 0)
            if(value >= max) return 100.f;
        return (value / max) * 100.f;
        return 0.f;
    };
    auto progressRun  = [=](ProgressTimer* bar, int max, int value, float duration){
        bar->runAction(ProgressTo::create(duration, getPercent(value, max)));
    };
    
    auto currentLevel = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL);
    auto currentEXP   = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::EXP);
    auto text         = StringUtils::format("%s %d", TRANSLATE("RESULT_PLAYER_LEVEL").c_str(), currentLevel);
    auto levelData    = CUserLevelDataManager::Instance()->getLevelDataByIndex(currentLevel);
    
    auto layer        = this->createIconLayer("characterIcon.png", text);
    auto layerSize    = layer->getContentSize();
    
    auto valueLabel   = Label::createWithTTF(StringUtility::toCommaString(currentEXP), FONT::MALGUNBD, 50);
    valueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    valueLabel->setPosition(Vec2(layerSize.width * 0.9f, layerSize.height * 0.5f));
    layer->addChild(valueLabel);
    
    auto labelSize    = valueLabel->getContentSize();
    auto expLebelPos  = Vec2(valueLabel->getPosition().x - labelSize.width, layerSize.height * 0.4f);
    auto expStr       = StringUtils::format("%s +  ", StringUtility::toCommaString(GVALUE->TOTAL_SCORE).c_str());
    auto expLabel     = Label::createWithTTF(expStr, FONT::MALGUNBD, 25);
    expLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    expLabel->setPosition(expLebelPos);
    layer->addChild(expLabel);
    
    auto bar_1        = createBar(layer, Color4B(255, 223, 0, 255 * 0.4f));
    auto bar_2        = createBar(layer, Color4B(255, 223, 0, 255));
    progressRun(bar_1, levelData._levelExp, currentEXP + GVALUE->TOTAL_SCORE, 2.f);
    progressRun(bar_2, levelData._levelExp, currentEXP, 0.f);
    
    m_ScoreLayerList.emplace_back(layer);
}

void CResultPopup::createChangeLabelAction(cocos2d::Label* label, std::string text1, std::string text2)
{
    if(label == nullptr) return;
    
    auto fadeOut  = FadeTo::create(0.5f, 0);
    auto callFunc = CallFunc::create([=](){
        auto origin  = label->getString();
        auto bestStr = text1;
        if(origin != bestStr)   label->setString(bestStr);
        else                    label->setString(text2);
    });
    auto fadeIn   = FadeIn::create(0.5f);
    auto delay    = DelayTime::create(1.f);
    auto seq      = Sequence::create(fadeOut, callFunc, fadeIn, delay, NULL);
    label->runAction(RepeatForever::create(seq));
    label->setString(text1);
}

void CResultPopup::createButtonLayer(std::function<void(Node*)> &callback,
                                     std::string iconName, std::string text, Color3B color)
{
    auto button = CMyButton::create()
    ->addEventListener(callback)
    ->setButtonSingleUse(true)
    ->setButtonNormalImage("resultPopup_1.png")
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(m_BG);
    button->setCascadeOpacityEnabled(true);
    
    auto btnSize = button->getContentSize();
    
    auto icon = Sprite::create(iconName);
    icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    icon->setPosition(Vec2(btnSize.width * 0.1f, btnSize.height * 0.5f));
    icon->setScale(100 / icon->getContentSize().height);
    button->addChild(icon);
    
    auto textLabel = Label::createWithSystemFont(text, FONT::MALGUNBD, 50);
    textLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    textLabel->setPosition(Vec2(btnSize.width * 0.15f,  btnSize.height * 0.5f));
    button->addChild(textLabel);
    
    m_ScoreLayerList.emplace_back(button);
}

void CResultPopup::userDataUpdate()
{
    auto calculateScore = [=](int value){
        if ((GVALUE->TOTAL_SCORE + value < INT_MAX))
            GVALUE->TOTAL_SCORE += value;
    };
    
    auto calculateBonusScore = [=](int value, int bonus){
        if ((GVALUE->TOTAL_SCORE + (value * bonus) < INT_MAX))
            GVALUE->TOTAL_SCORE += value * bonus;
    };
    auto userDataMng = CUserDataManager::Instance();
    
    // combo reset
    CComboScore::Instance()->ComboScoreReset();
    
    // play count +1
    userDataMng->setUserData_NumberAdd(USERDATA_KEY::PLAY_COUNT, 1);
    
    // update coin
    userDataMng->CoinUpdate(GVALUE->COIN_COUNT);
    
    // calculate total score
    calculateScore(GVALUE->STAR_SCORE);
    calculateScore(GVALUE->BEST_COMBO);
    calculateBonusScore(GVALUE->COIN_COUNT, 10);
    calculateBonusScore(GVALUE->NORMAL_ACHIEVEMENT_CLEAR_COUNT, 100);
    
    // combo가 user best combo면 저장한다.
    auto bestCombo = userDataMng->getUserData_Number(USERDATA_KEY::BEST_COMBO);
    if (GVALUE->BEST_COMBO > bestCombo)
        userDataMng->setUserData_Number(USERDATA_KEY::BEST_COMBO, GVALUE->BEST_COMBO);
    
    
    // total score가 best score면 저장한다.
    auto bestScore = userDataMng->getUserData_Number(USERDATA_KEY::BEST_SCORE);
    if (GVALUE->TOTAL_SCORE > bestScore)
        userDataMng->setUserData_Number(USERDATA_KEY::BEST_SCORE, GVALUE->TOTAL_SCORE);
    

    // save score to facebook
    if (CFacebookManager::IsScoresEnabled()){
        this->retain();
        
        CDownloadManager::IsNetworkConnected([=](bool isConnected){
            if(isConnected){
                auto oldScore    = CFacebookManager::Instance()->getMyFacebookData()->_score;
                auto friendCount = (int)CFacebookManager::Instance()->getFBUserList().size();
                auto setUserData = [=](){
                    auto newRank = CFacebookManager::Instance()->getMyRank();
                    userDataMng->setUserData_Number(USERDATA_KEY::RANK, newRank);
                    userDataMng->setUserData_Number(USERDATA_KEY::FRIENDS_COUNT, friendCount);
                };
                if(GVALUE->TOTAL_SCORE > oldScore){
                    
                    // save score to facebook data
                    CFacebookManager::Instance()->SaveScore(GVALUE->TOTAL_SCORE);
                    CFacebookManager::Instance()->setSaveScoreListener([=](){
                        setUserData();
                        this->release();
                    });
                }else{
                    CFacebookManager::RequestFriendList();
                    CFacebookManager::Instance()->setFriendListListener([=](){
                        setUserData();
                        this->release();
                    });
                }
            }
            else{
                this->release();
            }
        });
    }
    
    // save score to leaderboard
    if(CPlayManager::Instance()->IsLoggedIn()){
        auto planet = CObjectManager::Instance()->getPlanetParam();
        auto key    = planet->_leaderboard;
        CPlayManager::Instance()->ScoreSave([=](){}, key, GVALUE->TOTAL_SCORE);
    }
    
    // Check all of achievement.
    bool achievementAll = CAchievementDataManager::Instance()->CheckCompleteAll();
    m_GoalPopupOpen     = (GVALUE->NORMAL_ACHIEVEMENT_CLEAR_COUNT || achievementAll);
    
    // Check hidden achievement has been completed.
    CAchievementDataManager::Instance()->CompleteCheckRealTime(true);
    
    // Update google analytics
    CGoogleAnalyticsManager::LogEventAction(GA_CATEGORY::RESULT, GA_ACTION::TOTAL_SCORE, GVALUE->TOTAL_SCORE);
    CGoogleAnalyticsManager::LogEventAction(GA_CATEGORY::RESULT, GA_ACTION::STAR_SCORE,  GVALUE->STAR_SCORE);
    CGoogleAnalyticsManager::LogEventAction(GA_CATEGORY::RESULT, GA_ACTION::COIN_SCORE,  GVALUE->COIN_SCORE);
    CGoogleAnalyticsManager::LogEventAction(GA_CATEGORY::RESULT, GA_ACTION::COMBO_SCORE, GVALUE->BEST_COMBO);

    CGoogleAnalyticsManager::LogEventCoin(GA_ACTION::COIN_GET_INGAME, GVALUE->COIN_COUNT);
    CGoogleAnalyticsManager::LogEventValue(GA_CATEGORY::GAME_PLAY, GA_ACTION::PLAY_CHARACTER, GVALUE->CURRENT_CHARACTER);
    CGoogleAnalyticsManager::LogEventValue(GA_CATEGORY::GAME_PLAY, GA_ACTION::PLAY_COSTUME, GVALUE->CURRENT_COSTUME);
}
