#include "RewardPopup.h"
#include "RewardPopupDP.hpp"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/AchievementDataManager.hpp"
#include "../../Scene/GameScene.h"
#include "../../GameObject/ObjectManager.h"
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
    CMyButton::create()
    ->addEventListener([](Node* sender){})
    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080, 1920 * 3.f))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(popupSize / 2)
    ->show(this);
    
    
    CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->open();
    })
    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, popupSize.width, popupSize.height))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(popupSize / 2)
    ->show(this);
    
    
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
        
    }, 1.2f);
    
    this->setCloseAnimation([=](Node* sender){
        btnUserCoin->runAction(FadeTo::create(0.3f, 0));
    });
    
    this->setDefaultCallback([=](Node* sender){ this->open(); });
    this->setDefaultCallbackCleanUp(false);
    return CPopup::show(parent, zOrder);
}

CRewardPopup* CRewardPopup::setExitCallback(const EXIT_CALLBACK &callback)
{
    m_ExitCallback = callback;
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
        
    })
    ->setRewardData(data)
    ->setDefaultCallbackEnable(false)
    ->setBackgroundVisible(false)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(getContentSize() / 2)
    ->show(this, ZORDER::POPUP);
}

void CRewardPopup::open()
{
    if(!m_OpenEnable) return;
    
    m_OpenEnable = false;
    
    // Open enable after 5 seconds.
    this->scheduleOnce([=](float delta){
        m_OpenEnable = true;
    }, 5.f, "RewardSkipAble");
    
	if (m_RewardDP != nullptr){
		m_RewardDP->popupClose();
		m_RewardDP = nullptr;
	}

	if (!this->isItemRemain()){
		this->retain();
		if (m_ExitCallback){
			m_ExitCallback();
			m_ExitCallback = nullptr;
		}
		this->end();
		this->release();
		return;
	}

	auto data  = m_RewardList.at(m_RewardIndex);
	data       = CAchievementDataManager::Instance()->RewardByKey(data._key, data._value);
	m_RewardDP = this->createRewardDP(data);
	m_RewardIndex++;
}

void CRewardPopup::end()
{
    CObjectManager::Instance()->MoveAction(MOVE_DIRECTION::MIDDLE);
    this->popupClose(1.3f);
}

bool CRewardPopup::isItemRemain()
{
    return (m_RewardIndex < m_RewardList.size());
}