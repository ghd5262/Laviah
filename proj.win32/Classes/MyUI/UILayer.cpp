#include "UILayer.hpp"
#include "UIManager.h"
#include "HealthBarUI.h"
#include "BonusTimeUI.h"
#include "ScoreUI.h"
#include "MultipleScore.h"
#include "MyButton.h"
#include "../GameObject/ItemManager.h"
#include "../GameObject/ObjectManager.h"
#include "../Scene/GameScene.h"
#include <array>

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

CUILayer* CUILayer::create()
{
    CUILayer *pRet = new(std::nothrow) CUILayer();
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

bool CUILayer::init()
{
    if (!CPopup::init()) return false;
    
    Size popupSize = this->getContentSize();
    this->setCascadeOpacityEnabled(true);
    
    auto createScoreUI = [=](string iconImg, string id, Vec2 labelAnchor, Vec2 pos){
        auto scoreUI = CScoreUI::create("fonts/Number.ttf", 38, iconImg);
        scoreUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        scoreUI->setLabelAnchor(labelAnchor);
        scoreUI->setPosition(pos);
        this->addChild(scoreUI);
        if (!CUIManager::Instance()->AddUIWithName(scoreUI, id))
            CCASSERT(false, StringUtils::format("%s can not initialize", id.c_str()).c_str());
    };
    
    array<Vec2, 3> scoreUIPos = {
        Vec2(popupSize.width * 0.96f, popupSize.height * 0.96f ),
        Vec2(popupSize.width * 0.1f,  popupSize.height * 0.96f ),
        Vec2(popupSize.width * 0.96f, popupSize.height * 0.925f)
    };
    
    createScoreUI("score.png",      "StarScoreUI", Vec2::ANCHOR_MIDDLE_RIGHT, scoreUIPos[0]);
    createScoreUI("coinIcon_2.png", "CoinScoreUI", Vec2::ANCHOR_MIDDLE_LEFT,  scoreUIPos[1]);
    createScoreUI("run.png",        "RunScoreUI",  Vec2::ANCHOR_MIDDLE_RIGHT, scoreUIPos[2]);
    
    
    auto multipleScoreUI = CMultipleScore::create();
    this->addChild(multipleScoreUI); // referenceCount를 위하여 addChild
    multipleScoreUI->setVisible(false);
    if (!CUIManager::Instance()->AddUIWithName(multipleScoreUI, "MultipleScoreUI"))
        CCASSERT(false, "MultipleScoreUI CAN NOT INIT");
    
//    auto bonusTime = CBonusTimeUI::create();
//    bonusTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//    bonusTime->setPosition(Vec2(popupSize.width * 0.06f, popupSize.height * 0.925f));
//    this->addChild(bonusTime, 102);
//    if (!CUIManager::Instance()->AddUIWithName(bonusTime, "BonusTime"))
//        CCASSERT(false, "BonusTime CAN NOT INIT");
    
    
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
    
    this->setOpenAnimation([=](Node* sender){
    });
    
    this->setCloseAnimation([=](Node* sender){
    });
    
    return true;
}

void CUILayer::onPauseButton(cocos2d::Node* sender)
{
    CGameScene::getGameScene()->GamePause();
}

void CUILayer::initItemTestButton()
{
    auto createItemTest = [=](eITEM_TYPE type, Vec2 pos){
        CMyButton::create()
        ->addEventListener([=](Node* sender){
            CItemManager::Instance()->StartItemTimer(type);
        })
        ->setButtonNormalImage(StringUtils::format("playItem_%d.png", type))
        ->setButtonPosition(pos)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->show(this);
    };
    
    Size popupSize = this->getContentSize();

    array<Vec2, 6> itemPosArray = {
        Vec2(popupSize.width * 0.08f, popupSize.height * 0.5f   ),
        Vec2(popupSize.width * 0.08f, popupSize.height * 0.475f ),
        Vec2(popupSize.width * 0.08f, popupSize.height * 0.45f  ),
        Vec2(popupSize.width * 0.08f, popupSize.height * 0.425f ),
        Vec2(popupSize.width * 0.08f, popupSize.height * 0.4f   ),
        Vec2(popupSize.width * 0.08f, popupSize.height * 0.375f )
    };
    
    for (int idx = 0; idx < itemPosArray.size(); idx++)
    {
        createItemTest((eITEM_TYPE)(eITEM_TYPE::eITEM_TYPE_health + idx), itemPosArray[idx]);
    }
}