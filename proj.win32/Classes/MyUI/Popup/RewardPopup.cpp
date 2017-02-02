#include "RewardPopup.h"
#include "RewardPopupDP.hpp"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/ChallengeDataManager.hpp"
#include "../../Scene/GameScene.h"
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

bool CRewardPopup::init()
{
	if (!CPopup::init()) return false;

	auto popupSize = this->getContentSize();

	auto rewardIcon = Sprite::create("rewardIconBig.png");
	rewardIcon->setColor(COLOR::GOLD);
	rewardIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	rewardIcon->setPosition(popupSize / 2);
	this->addChild(rewardIcon);


	auto rewardBack = Sprite::create("rewardBack_2.png");
	rewardBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	rewardBack->setPosition(popupSize / 2);
	rewardBack->setVisible(false);
	this->addChild(rewardBack);
	rewardBack->runAction(RepeatForever::create(RotateBy::create(5.f, 360.f)));

	//auto sizeDown = ScaleTo::create(0.8f, 1.f);
	//auto sizeUp = ScaleTo::create(0.8f, 1.2f);
	//auto sequence = Sequence::createWithTwoActions(sizeDown, sizeUp);
	//auto repeat = RepeatForever::create(sequence);

	//rewardIcon->runAction(repeat);
	
	auto btnUserCoin = CUserCoinButton::create();
	if (btnUserCoin != nullptr)
	{
		btnUserCoin->setPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.05f));
		btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		btnUserCoin->setCascadeOpacityEnabled(true);
		btnUserCoin->setOpacity(0);
		this->addChild(btnUserCoin);
	}

	CMyButton::create()
    ->addEventListener([=](Node* sender){
		this->Tab();
		rewardBack->setVisible(true);
    })
    ->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
    ->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, popupSize.width, popupSize.height))
    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setButtonPosition(this->getContentSize() / 2)
    ->show(this);
    
    this->setOpenAnimation([=](Node* sender){
		btnUserCoin->runAction(FadeIn::create(0.5f));
	});

	this->setCloseAnimation([=](Node* sender){
		btnUserCoin->runAction(FadeTo::create(0.5f, 0));	
	});

	this->setDefaultCallback([=](Node* sender){
		this->Tab();
		rewardBack->setVisible(true);
	});

	return true;
}

CPopup* CRewardPopup::setExitCallback(const EXIT_CALLBACK &callback)
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
	return CRewardPopupDP::create(data)
		->setDefaultCallbackEnable(false)
		->setBackgroundVisible(false)
		->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setPopupPosition(getContentSize() / 2)
		->show(this, ZORDER::POPUP);
}

void CRewardPopup::Tab()
{
	if (m_RewardDP != nullptr){
		m_RewardDP->popupClose();
		m_RewardDP = nullptr;
	}

	if (m_RewardIndex >= m_RewardList.size()){
		this->retain();
		if (m_ExitCallback){
			m_ExitCallback();
			m_ExitCallback = nullptr;
		}
		this->popupClose();
		this->release();
		return;
	}

	auto data = m_RewardList.at(m_RewardIndex);
	data = CChallengeDataManager::Instance()->RewardByKey(data._key, data._value);
	m_RewardDP = this->createRewardDP(data);
	m_RewardIndex++;
}