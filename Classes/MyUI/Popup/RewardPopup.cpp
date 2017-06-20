#include "RewardPopup.h"
#include "RewardPopupDP.hpp"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/AchievementDataManager.hpp"
#include "../../DataManager/AchievementRewarder/AchievementRewarder.hpp"
#include "../../Scene/GameScene.h"
#include "../../GameObject/ObjectManager.h"
#include "../../DataManager/DataManagerUtils.h"
#include <array>

CRewardPopup* CRewardPopup::create()
{
	CRewardPopup *pRet = new(std::nothrow) CRewardPopup();
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

CPopup* CRewardPopup::show(cocos2d::Node* parent/*  = nullptr*/, int zOrder/* = 0*/)
{
    
    auto popupSize = this->getContentSize();
    
    // touch disable
    auto createTouchDisable = [=](Node* parent, Size size, int zOrder = 0){
        CMyButton::create()
        ->addEventListener([](Node* sender){})
        ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
        ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, size.width, size.height))
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(parent->getContentSize() / 2)
        ->show(parent, zOrder);
    };
    
    // disable the background
    createTouchDisable(this, Size(1080, 1920 * 3.f));
    
    CMyButton::create()
    ->addEventListener([=](Node* sender){
        if(m_IsPaidFeature) return;
        this->open();
    })
    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, popupSize.width, popupSize.height))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(popupSize / 2)
    ->show(this);
    
    
    m_GetButton = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->open();
    })
    ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 430, 150))
    ->setContents("Get")
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.2f))
    ->show(this);
    
    
    m_PlayButton = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->play();
    })
    ->setLayer(LayerColor::create(COLOR::BRIGHTRED_ALPHA, 430, 150))
    ->setContents("Play")
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(popupSize.width * 0.275f, popupSize.height * 0.2f))
    ->show(this);
    m_PlayButton->setVisible(false);
    
    // disable the background of Get button
    createTouchDisable(m_GetButton, Size(1080, 1920 * 2.f), -1);
    
    auto btnEnd = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->exit();
    })
    ->setButtonSingleUse(true)
    ->setButtonNormalImage("endIcon.png")
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(popupSize.width * 0.92f, popupSize.height * 0.05f))
    ->show(this);
    if(!m_IsPaidFeature) btnEnd->setVisible(false);
    
    auto btnUserCoin = CUserCoinButton::create();
    btnUserCoin->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.05f));
    btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(btnUserCoin);
    
    this->setOpenAnimation([=](Node* sender){
        auto action = [=](Node* owner){
            auto delay = DelayTime::create(1.f);
            auto fade  = FadeIn::create(0.5f);
            auto sequence = Sequence::createWithTwoActions(delay, fade);
            owner->setOpacity(0);
            owner->runAction(sequence);
        };
        
        action(btnUserCoin);
        action(btnEnd);
        this->showButtons();
        
    }, 1.2f);
    
    this->setCloseAnimation([=](Node* sender){
        btnUserCoin->runAction(FadeTo::create(0.3f, 0));
        btnEnd->runAction(FadeTo::create(0.3f, 0));
        m_GetButton->runAction(FadeTo::create(0.3f, 0));
        m_PlayButton->runAction(FadeTo::create(0.3f, 0));
    });
    
    if(m_IsPaidFeature) this->setDefaultCallback([=](Node* sender){ this->exit(); });
    else                this->setDefaultCallback([=](Node* sender){ this->open(); }, false);

    return CPopup::show(parent, zOrder);
}

CRewardPopup* CRewardPopup::AddRewardToList(std::vector<sREWARD_DATA> list)
{
    DATA_MANAGER_UTILS::copyList(list, m_RewardList);
    return this;
}

CRewardPopup* CRewardPopup::setExitCallback(const EXIT_CALLBACK &callback)
{
    m_ExitCallback = callback;
    return this;
}

CRewardPopup* CRewardPopup::setIsPaidFeature(int cost)
{
    m_Cost = cost;
    m_IsPaidFeature = (cost != 0);
    return this;
}

void CRewardPopup::AddRewardToList(std::string key, int value)
{
	m_RewardList.emplace_back(sREWARD_DATA(key, value));
}

CPopup* CRewardPopup::createRewardDP(sREWARD_DATA data)
{
    return CRewardPopupDP::create()
    ->setExitCallback([=](){
        if(this->isItemRemain()) this->open();
        else                     this->showButtons();
    })
    ->setRewardData(data)
//    ->setDefaultCallbackEnable(false)
    ->setBackgroundVisible(false)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(this->getContentSize() / 2)
    ->show(this, ZORDER::POPUP);
}

void CRewardPopup::open()
{
	if (m_RewardDP != nullptr){
		m_RewardDP->popupClose();
		m_RewardDP = nullptr;
	}
    
    if (m_IsPaidFeature){
        if(!CCharacterDataManager::Instance()->getNewRandomCharacter()) {
            CGameScene::getGameScene()->CreateAlertPopup()
            ->setPositiveButton([=](Node* sender){}, TRANSLATE("BUTTON_OK"))
            ->setMessage("There are no more characters to get.")
            ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
        }
        else this->AddRewardToList(ACHIEVEMENT_REWARD_KEY::REWARD_CHARACTER_RANDOM, 0);
    }
    
    if (!this->isItemRemain() || !CUserDataManager::Instance()->CoinUpdate(m_Cost)){
        this->exit();
		return;
	}

    m_GetButton->setVisible(false);
    m_PlayButton->setVisible(false);
    
	m_LastSavedData = m_RewardList.at(m_RewardIndex);
	m_LastSavedData = CAchievementDataManager::Instance()->RewardByKey(m_LastSavedData._key,
                                                                       m_LastSavedData._value);
	m_RewardDP      = this->createRewardDP(m_LastSavedData);
	m_RewardIndex++;
}

void CRewardPopup::play()
{
    this->exit();
    
    auto index = m_LastSavedData._value;
    if(!CUserDataManager::Instance()->getUserData_IsItemHave(USERDATA_KEY::CHARACTER_LIST, index))
        return;
    
    CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::CHARACTER, index);
    CObjectManager::Instance()->ChangeCharacter();
    CGameScene::getGameScene()->GameStart();
}

void CRewardPopup::end()
{
    CObjectManager::Instance()->MoveAction(MOVE_DIRECTION::MIDDLE);
    this->popupClose(1.3f);
}

void CRewardPopup::exit()
{
    this->retain();
    if (m_ExitCallback){
        m_ExitCallback();
        m_ExitCallback = nullptr;
    }
    this->end();
    this->release();
}

void CRewardPopup::showButtons()
{
    auto action = [=](Node* owner){
        auto delay = DelayTime::create(1.f);
        auto fade  = FadeIn::create(0.5f);
        auto sequence = Sequence::createWithTwoActions(delay, fade);
        owner->setVisible(true);
        owner->setOpacity(0);
        owner->runAction(sequence);
    };
    
    auto layerSize = this->getContentSize();
    
    if(m_LastSavedData._key == ACHIEVEMENT_REWARD_KEY::REWARD_CHARACTER_RANDOM ||
       m_LastSavedData._key == ACHIEVEMENT_REWARD_KEY::REWARD_CHARACTER){
        m_PlayButton->setPosition(Vec2(layerSize.width * 0.275f, layerSize.height * 0.2f));
        m_GetButton->setPosition(Vec2(layerSize.width * 0.725f, layerSize.height * 0.2f));
        action(m_PlayButton);
    }
    
    if(!isItemRemain()) m_GetButton->changeContents("Confirm");
    if(m_IsPaidFeature) m_GetButton->changeContents(StringUtils::format("Get %d", m_Cost));
    
    action(m_GetButton);
}

bool CRewardPopup::isItemRemain()
{
    return (m_RewardIndex < m_RewardList.size());
}