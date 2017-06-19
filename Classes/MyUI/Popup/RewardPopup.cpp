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

CPopup* CRewardPopup::show(cocos2d::Node* parent, unsigned zOrder/* = 0*/)
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
    
    
    m_BtnGet = CMyButton::create()
    ->addEventListener([=](Node* sender){
        m_BtnGet->setVisible(false);
        this->open();
    })
    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 400, 150))
    ->setContents("Get")
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.2f))
    ->show(this);
    
    
    m_BtnEnd = CMyButton::create()
    ->addEventListener([=](Node* sender){
        this->end();
    })
    ->setButtonSingleUse(true)
    ->setButtonNormalImage("endIcon.png")
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(Vec2(popupSize.width * 0.92f, popupSize.height * 0.05f))
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
        
        action(m_BtnEnd);
        action(btnUserCoin);
        
    }, 1.2f);
    
    this->setCloseAnimation([=](Node* sender){
        btnUserCoin->runAction(FadeTo::create(0.3f, 0));
        m_BtnEnd->runAction(FadeTo::create(0.3f, 0));
    });
    
    this->setDefaultCallback([=](Node* sender){
        this->end();
    });
    
    this->uiUpdate();
    
    return CPopup::show(parent, zOrder);
}

CRewardPopup* CRewardPopup::setExitCallback(const EXIT_CALLBACK &callback)
{
    m_ExitCallback = callback;
    return this;
}

void CRewardPopup::AddItemToList(std::string key, int value, int cost)
{
	m_ItemList.emplace_back(sREWARD_DATA(key, value), cost);
}

CPopup* CRewardPopup::createRewardDP(sREWARD_DATA data)
{
    return CRewardPopupDP::create()
    ->setExitCallback([=](){
        if(!this->isItemRemain()) return;
        
        m_BtnGet->setVisible(true);
        this->uiUpdate();
    })
    ->setRewardData(data)
    ->setDefaultCallbackEnable(false)
    ->setBackgroundVisible(false)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(getContentSize() / 2)
    ->show(this, ZORDER::POPUP);
}

void CRewardPopup::uiUpdate()
{
    if(m_BtnGet == nullptr) return;
    if(!this->isItemRemain()) return;
    
    std::string content = "Get";
    auto cost = m_ItemList.at(m_ItemIndex)._cost;
    if(cost != 0) content = StringUtils::format("Buy -%d", cost);
    m_BtnGet->changeContents(content);
}

void CRewardPopup::open()
{
    if(!m_OpenEnable) return;
    
    m_OpenEnable = false;
    
    // Open enable after 5 seconds.
    this->scheduleOnce([=](float delta){
        m_OpenEnable = true;
    }, 5.f, "RewardOpenAble");
    
    this->changeDefaultCallback([=](Node* sender){ this->open(); });
    this->setDefaultCallbackCleanUp(false);
    m_BtnEnd->setTouchEnable(false);
    m_BtnEnd->runAction(FadeTo::create(0.3f, 0));
    
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

	auto data  = m_ItemList.at(m_ItemIndex)._rewardData;
	data       = CAchievementDataManager::Instance()->RewardByKey(data._key, data._value);
	m_RewardDP = this->createRewardDP(data);
	m_ItemIndex++;
}

void CRewardPopup::end()
{
    CObjectManager::Instance()->MoveAction(MOVE_DIRECTION::MIDDLE);
    this->popupClose(1.3f);
}

bool CRewardPopup::isItemRemain()
{
    return (m_ItemIndex < m_ItemList.size());
}