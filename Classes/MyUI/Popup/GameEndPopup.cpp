#include "GameEndPopup.hpp"
#include "../MyButton.h"
#include "../Tutorial/TutorialManager.hpp"
#include "../Tutorial/TutorialHelper.hpp"
#include "../../Scene/GameScene.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Player.h"

CGameEndPopup* CGameEndPopup::create()
{
    CGameEndPopup *pRet = new(std::nothrow) CGameEndPopup();
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

bool CGameEndPopup::init()
{
    if (!CPopup::init()) return false;
    
    auto popupSize = this->getContentSize();
    
    // create crashed!!
    auto btnScreen = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->End(sender);
    })
    ->setLayer(cocos2d::LayerColor::create(COLOR::TRANSPARENT_ALPHA, popupSize.width, popupSize.height))
    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setButtonSingleUse(true)
    ->setButtonPosition(popupSize / 2)
    ->setEnableSound(false)
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(this);
    btnScreen->setOpacity(0);
    
    auto labelCrash = Label::createWithSystemFont(" 충돌!!", FONT::MALGUNBD, 80);
    labelCrash->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    labelCrash->setPosition(Vec2(btnScreen->getContentSize().width * 0.5f,
                                 btnScreen->getContentSize().height * 0.8f));
    btnScreen->addChild(labelCrash);
    
    auto labelCrashTip = Label::createWithSystemFont(" 항상 총알을 조심하세요!", FONT::MALGUNBD, 40);
    labelCrashTip->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    labelCrashTip->setPosition(Vec2(btnScreen->getContentSize().width * 0.5f,
                                    btnScreen->getContentSize().height * 0.75f));
    btnScreen->addChild(labelCrashTip);
    
    this->setOpenAnimation([=](Node* sender){
        auto delay = DelayTime::create(0.5f);
        auto fade  = FadeIn::create(0.5f);
        auto sequence = Sequence::createWithTwoActions(delay, fade);
        btnScreen->runAction(sequence);
    }, 1.f);
    
    this->setCloseAnimation([=](Node* sender){
        btnScreen->runAction(FadeTo::create(0.5f, 0));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->End(sender);
    });
    
    return true;
}

void CGameEndPopup::End(Node* sender){
    if (CTutorialManager::Instance()->getIsRunning()){
        CObjectManager::Instance()->ReturnToMemoryBlockAll();
        CObjectManager::Instance()->getPlayer()->PlayerAlive();
        CGameScene::getGameScene()->GameResume();
        CTutorialManager::Instance()->ChangeTutorial(TUTORIAL_KEY::REVIVE);
    }
    else{
        if (GVALUE->RUN_SCORE < 0 || GVALUE->REVIVE_COUNT)	CGameScene::getGameScene()->GameResult();
        else                                                CGameScene::getGameScene()->OpenVideoPopup();
    }

    this->popupClose();
}
