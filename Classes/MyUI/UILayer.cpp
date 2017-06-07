#include "UILayer.hpp"
#include "HealthBarUI.h"
#include "BonusTimeUI.h"
#include "ScoreUI.h"
#include "ComboScore.h"
#include "MyButton.h"
#include "CountDown.hpp"
#include "ItemProgress.hpp"
#include "AchievementProgressBar.hpp"
#include "FacebookRivalRankLayer.hpp"
#include "Popup/AchievementCompleteNoticePopup.h"
#include "../GameObject/ItemManager.h"
#include "../GameObject/ObjectManager.h"
#include "../GameObject/Player.h"
#include "../Scene/GameScene.h"
#include "../AI/States/GameStates.h"
#include "../DataManager/GradientDataManager.h"
#include <array>

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

CUILayer* CUILayer::m_Instance = nullptr;

CUILayer::~CUILayer()
{
    m_Instance = nullptr;
}

CUILayer* CUILayer::Instance()
{
    if(m_Instance != nullptr) return m_Instance;
    
    m_Instance = new(std::nothrow) CUILayer();
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

bool CUILayer::init()
{
    if (!CPopup::init()) return false;
    
    this->scheduleUpdate();
    Size popupSize = this->getContentSize();
    this->setCascadeOpacityEnabled(true);
    
    auto createScoreUI = [=](int& score, string iconImg, Vec2 labelAnchor, Vec2 pos){
        auto scoreUI = CScoreUI::create(score)
        ->setFont(FONT::MALGUNBD, 45)
        ->setIcon(iconImg)
        ->setScoreAnchorPoint(labelAnchor)
        ->show(this);
        scoreUI->setPosition(pos);
        scoreUI->setOpacity(255 * 0.8f);
//        scoreUI->setAnchorPoint(labelAnchor);
        return scoreUI;
    };
    
    array<Vec2, 3> scoreUIPos = {
        Vec2(popupSize.width * 0.025f, popupSize.height * 0.96f ),
        Vec2(popupSize.width * 0.025f, popupSize.height * 0.925f ),
		Vec2(popupSize.width * 0.9f,   popupSize.height * 0.96f)
    };
	m_StarScoreUI = createScoreUI(GLOBAL->STAR_SCORE, "starIcon_s.png", Vec2::ANCHOR_MIDDLE_LEFT, scoreUIPos[0]);
	createScoreUI(GLOBAL->COIN_SCORE, "coinIcon_s.png", Vec2::ANCHOR_MIDDLE_LEFT, scoreUIPos[1]);
//	createScoreUI(GLOBAL->RUN_SCORE, "runIcon_s.png", Vec2::ANCHOR_MIDDLE, scoreUIPos[2]);

//    auto bonusTime = CBonusTimeUI::create();
//    bonusTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//    bonusTime->setPosition(Vec2(popupSize.width * 0.06f, popupSize.height * 0.925f));
//    this->addChild(bonusTime, 102);
//    if (!CUIManager::Instance()->AddUIWithName(bonusTime, "BonusTime"))
//        CCASSERT(false, "BonusTime CAN NOT INIT");
    
    m_StarScoreLabel = Label::createWithTTF("", FONT::MALGUNBD, 40);
    m_StarScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    this->addChild(m_StarScoreLabel);
    
    m_LevelLabel = Label::createWithTTF("", FONT::MALGUNBD, 150);
    m_LevelLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_LevelLabel->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.825f));
    this->addChild(m_LevelLabel);
    
    CMyButton::create()
    ->addEventListener(std::bind(&CObjectManager::RotationObject, CObjectManager::Instance(), -2.f), eMYBUTTON_STATE::EXECUTE)
    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, popupSize.width, popupSize.height))
    ->setEnableSound(false)
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(popupSize / 2)
    ->show(this);
    
    m_PauseBtn = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->onPauseButton(sender);
    })
    ->setButtonNormalImage("pauseIcon.png")
    ->setButtonPosition(Vec2(popupSize.width * 0.92f, popupSize.height * 0.05f))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(this);
    m_PauseBtn->setCascadeOpacityEnabled(true);
    m_PauseBtn->setOpacity(0);
    
	m_ProgressPosArray = {
		Vec2(popupSize.width * 0.5f, popupSize.height * 0.76f),
		Vec2(popupSize.width * 0.5f, popupSize.height * 0.74f),
		Vec2(popupSize.width * 0.5f, popupSize.height * 0.72f),
		Vec2(popupSize.width * 0.5f, popupSize.height * 0.7f),
		Vec2(popupSize.width * 0.5f, popupSize.height * 0.68f),
		Vec2(popupSize.width * 0.5f, popupSize.height * 0.66),
		Vec2(popupSize.width * 0.5f, popupSize.height * 0.64f)
	};
	std::fill(m_ProgressList.begin(), m_ProgressList.end(), nullptr);
    for(int count = 1; count < eITEM_TYPE_MAX; count++)
		this->createItemTimerUI((eITEM_TYPE)count, Color3B::WHITE);
    
    m_AchievementProgressBar = CAchievementProgressBar::create()
    ->setBarBGColor(COLOR::WHITEGRAY_ALPHA)
    ->setBarColor(COLOR::GOLD)
    ->setBarAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP)
    ->setBarPosition(Vec2(popupSize.width * 0.5f, popupSize.height))
    ->show(this);
    
	CAchievementCompleteNoticePopup::create()
    ->setDefaultCallbackEnable(false)
    ->setBackgroundVisible(false)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.8f))
    ->show(this);

//    this->initItemTestButton();
//    CGameScene::getGameScene()->GameResume();
    
    this->setDefaultCallback([=](Node* sender){
        this->onPauseButton(sender);
    }, false);
    
    return true;
}

void CUILayer::ScoreAction(int score)
{
    if(!m_StarScoreLabel) return;
    if(m_StarScoreLabel->getActionByTag(100))
        m_StarScoreLabel->stopActionByTag(100);
    
    auto uiPos  = m_StarScoreUI->getPosition();
    auto uiSize = m_StarScoreUI->getContentSize();
    
    m_StarScoreLabel->setString(StringUtils::format("+ %d", score));
    m_StarScoreLabel->setPosition(Vec2(uiPos.x + uiSize.width, uiPos.y));
    m_StarScoreLabel->setColor(CGradientDataManager::Instance()->getScoreColorByLevel(GLOBAL->COMBO_LEVEL));
    auto fadeIn   = FadeTo::create(0.3f, 255 * 0.8);
    auto delay    = DelayTime::create(0.3f);
    auto fadeTo   = FadeTo::create(0.3f, 0);
    auto sequence = Sequence::create(fadeIn, delay, fadeTo, NULL);
    sequence->setTag(100);
    m_StarScoreLabel->runAction(sequence);
}

void CUILayer::LevelUPNotice()
{
    if(!m_LevelLabel) return;
    
    this->scheduleOnce([=](float delta){
        if(CObjectManager::Instance()->getIsGamePause()) return;
        
        Vec2 startPos     = Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.9f);
        Vec2 targetPos    = Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.8f);
        auto moveDown     = EaseExponentialOut::create(MoveTo::create(0.5f, targetPos));
        auto downAction   = Spawn::createWithTwoActions(moveDown, FadeTo::create(0.3f, 255 * 0.8f));
        auto delayAction  = DelayTime::create(2.f);
        auto moveUp       = EaseSineIn::create(MoveTo::create(0.3f, startPos));
        auto upAction     = Spawn::createWithTwoActions(moveUp, FadeTo::create(0.1f, 0));
        auto sequence     = Sequence::create(downAction, delayAction, upAction, nullptr);
        
        m_LevelLabel->setOpacity(0);
        m_LevelLabel->setColor(CGradientDataManager::Instance()->getBulletColorByLevel(GLOBAL->PATTERN_LEVEL));
        m_LevelLabel->setString(StringUtils::format("LEVEL %d", GLOBAL->PATTERN_LEVEL + 1));
        m_LevelLabel->runAction(sequence);
        
    }, 4.f, "LevelNoticeDelay");
    
}

void CUILayer::setItemTimer(eITEM_TYPE type, float limitTime)
{
	if (type == eITEM_TYPE_none) return;

    auto timer = m_ProgressList[type];
    timer->setLimitTime(limitTime);
	timer->setTime(0.f);
	if (limitTime > 0)
		timer->setVisible(true);
    
    if(timer->getIsPause())
    {
        // set new position
        timer->setIsPause(false);
        timer->setPosition(m_ProgressPosArray[m_TimerRunningCount]);
        m_TimerRunningCount++;
    }
}

void CUILayer::update(float delta)
{    
    if(CObjectManager::Instance()->getIsGamePause() && !m_Pause)
        this->stop();
    else if(!CObjectManager::Instance()->getIsGamePause() && m_Pause)
        this->play();
}


void CUILayer::stop()
{
    m_Pause = true;
    m_PauseBtn->runAction(FadeTo::create(0.5f, 0));

	auto move = MoveTo::create(0.01f, Vec2(this->getContentSize().width * 0.5f,
										   this->getContentSize().height * 1.1f));
	auto ease = EaseSineIn::create(move);
    auto call = CallFunc::create([=](){
        m_AchievementProgressBar->setVisible(false);
    });
    auto seq  = Sequence::createWithTwoActions(ease, call);
	m_AchievementProgressBar->runAction(seq);
}

void CUILayer::play()
{
    m_Pause = false;
    m_PauseBtn->runAction(FadeIn::create(0.5f));
    
    m_AchievementProgressBar->setVisible(true);
    auto move = MoveTo::create(0.5f, Vec2(this->getContentSize().width * 0.5f,
                                          this->getContentSize().height));
    auto exponential = EaseExponentialOut::create(move);
    m_AchievementProgressBar->runAction(exponential);
}

void CUILayer::onPauseButton(cocos2d::Node* sender)
{
    CGameScene::getGameScene()->OpenGamePausePopup();
}

void CUILayer::createItemTimerUI(eITEM_TYPE type, Color3B color)
{
    auto progress = CItemProgress::create()
    ->addLastEventListner([=](Node* sender){
        if(m_TimerRunningCount > 0)
            m_TimerRunningCount--;
    })
	->setIcon(StringUtils::format("itemBubbleIcon_%d.png", type))
    ->setBarColor(color)
    ->show(this);
    progress->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    progress->setPosition(m_ProgressPosArray[0]);
    progress->setVisible(false);
	m_ProgressList[type] = progress;
}

void CUILayer::initItemTestButton()
{
    auto createItemTest = [=](eITEM_TYPE type, Vec2 pos){
        CMyButton::create()
        ->addEventListener([=](Node* sender){
            CItemManager::Instance()->StartItemTimer(type);
//			CObjectManager::Instance()->StartBonusTime();
        })
        ->setButtonNormalImage(StringUtils::format("playItem_%d.png", type))
        ->setButtonPosition(pos)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->show(this);
    };
    
    Size popupSize = this->getContentSize();

    array<Vec2, 7> itemPosArray = {
        Vec2(popupSize.width * 0.08f, popupSize.height * 0.5f   ),
        Vec2(popupSize.width * 0.08f, popupSize.height * 0.475f ),
        Vec2(popupSize.width * 0.08f, popupSize.height * 0.45f  ),
        Vec2(popupSize.width * 0.08f, popupSize.height * 0.425f ),
        Vec2(popupSize.width * 0.08f, popupSize.height * 0.4f   ),
        Vec2(popupSize.width * 0.08f, popupSize.height * 0.375f ),
        Vec2(popupSize.width * 0.08f, popupSize.height * 0.35f  )
    };
    
    for (int idx = 0; idx < itemPosArray.size(); idx++)
    {
        createItemTest((eITEM_TYPE)(eITEM_TYPE::eITEM_TYPE_health + idx), itemPosArray[idx]);
    }
}