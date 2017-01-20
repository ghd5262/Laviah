#include "BonusTimeLayer.hpp"
#include "HealthBarUI.h"
#include "BonusTimeUI.h"
#include "ScoreUI.h"
#include "MultipleScore.h"
#include "MyButton.h"
#include "CountDown.hpp"
#include "../GameObject/ItemManager.h"
#include "../GameObject/ObjectManager.h"
#include "../GameObject/Player.h"
#include "../Scene/GameScene.h"
#include "../AI/States/GameStates.h"
#include <array>

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

CBonusTimeLayer* CBonusTimeLayer::create()
{
    CBonusTimeLayer *pRet = new(std::nothrow) CBonusTimeLayer();
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

bool CBonusTimeLayer::init()
{
    if (!CPopup::init()) return false;
    
    Size popupSize = this->getContentSize();
    this->scheduleUpdate();
    this->setCascadeOpacityEnabled(true);
    
	auto createScoreUI = [=](int& score, string iconImg, Vec2 labelAnchor, Vec2 pos){
		auto scoreUI = CScoreUI::create(score)
			->setFont(FONT::MALGUNBD, 40)
			->setIcon(iconImg)
			->setScoreAnchorPoint(labelAnchor)
			->show(this);
		scoreUI->setPosition(pos);
		return scoreUI;
	};
    
    array<Vec2, 3> scoreUIPos = {
        Vec2(popupSize.width * 0.96f, popupSize.height * 0.96f ),
        Vec2(popupSize.width * 0.1f,  popupSize.height * 0.96f ),
        Vec2(popupSize.width * 0.96f, popupSize.height * 0.925f)
    };
    
    m_StarScoreUI = createScoreUI(GLOBAL->STAR_SCORE, "starIcon_s.png", Vec2::ANCHOR_MIDDLE_RIGHT, scoreUIPos[0]);
    m_CoinScoreUI = createScoreUI(GLOBAL->COIN_SCORE, "coinIcon_s.png", Vec2::ANCHOR_MIDDLE_LEFT,  scoreUIPos[1]);
    m_RunScoreUI  = createScoreUI(GLOBAL->RUN_SCORE,  "runIcon_s.png",	Vec2::ANCHOR_MIDDLE_RIGHT, scoreUIPos[2]);
    
    CMyButton::create()
    ->addEventListener(std::bind(&CObjectManager::BonusTimeTouchEvent, CObjectManager::Instance(), -2.f), eMYBUTTON_STATE::EXECUTE)
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
    
    CGameScene::getGameScene()->GameResume();
    
    return true;
}

void CBonusTimeLayer::update(float delta)
{
    if(CObjectManager::Instance()->getIsGamePause() && !m_Pause)
        this->stop();
    else if(!CObjectManager::Instance()->getIsGamePause() && m_Pause)
        this->play();
    
   /* m_StarScoreUI->setValue(GLOBAL->STAR_SCORE);
    m_CoinScoreUI->setValue(GLOBAL->COIN_SCORE);
    m_RunScoreUI->setValue(GLOBAL->RUN_SCORE);*/
}

void CBonusTimeLayer::onPauseButton(cocos2d::Node* sender)
{
    CGameScene::getGameScene()->OpenGamePausePopup();
}

void CBonusTimeLayer::stop()
{
    m_Pause = true;
    m_PauseBtn->runAction(FadeTo::create(0.5f, 0));
}

void CBonusTimeLayer::play()
{
    m_Pause = false;
    m_PauseBtn->runAction(FadeIn::create(0.5f));
}