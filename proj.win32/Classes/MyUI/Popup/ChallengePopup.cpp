#include "ChallengePopup.h"
#include "ChallengePopupDP.h"
#include "RewardPopup.h"
#include "../MyButton.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/ChallengeDataManager.hpp"
#include "../../Scene/GameScene.h"
#include <array>

CChallengePopup* CChallengePopup::create()
{
	CChallengePopup *pRet = new(std::nothrow) CChallengePopup();
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

bool CChallengePopup::init()
{
	if (!CPopup::init()) return false;
	return true;
}

CPopup* CChallengePopup::show(Node* parent, int zOrder/* = 0*/)
{
	auto popupSize = this->getContentSize();
	auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
	if (bg != nullptr){
		bg->setIgnoreAnchorPointForPosition(false);
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(popupSize / 2);
		this->addChild(bg);
	}

	auto challengesLabel = Label::createWithTTF("Challenge", FONT::MALGUNBD, 80);
	if (challengesLabel != nullptr)
	{
		if (!m_EndButtonVisible) { 
			challengesLabel->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.6f));
			challengesLabel->setColor(COLOR::DARKGRAY);
		}
		else { 
			challengesLabel->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.8f)); 
			challengesLabel->setColor(COLOR::BRIGHTGRAY);
		}
		
		challengesLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		challengesLabel->setOpacity(0);
		this->addChild(challengesLabel);
	}

	std::array<Vec2, 4> startPos = {
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.3f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.1f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.0f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * -0.1f)
	};

	std::array<Vec2, 4> targetPos = {
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.6f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.5),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.4),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.2f)
	};

	std::vector<CPopup*> dpVector;

	int idx = 0;
	auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_LIST);
	for (auto index : *list)
	{
		auto challengeData = CChallengeDataManager::Instance()->getChallengeByIndex(index);
		auto dp = CChallengePopupDP::create(challengeData, idx)
			->addSkipEventListner([=](CChallengePopupDP* sender, int sequence){
			this->Skip(sender, sequence);
		})
			->setBackgroundVisible(false)
			->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->setPopupPosition(startPos[idx])
			->show(this, ZORDER::POPUP);
		dp->setOpenAnimation(nullptr);
		dp->setCloseAnimation(nullptr);

		dpVector.emplace_back(dp);
        idx++;
	}

	auto createBtn = [=](const std::function<void(Node*)> &callback, std::string icon, Vec2 pos){
		return CMyButton::create()
			->addEventListener(callback)
			->setButtonNormalImage(icon)
			->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->setButtonPosition(pos)
			->show(this);
	};

	auto btnEnd = createBtn([=](Node* sender){
		this->End(sender);
	}, "endIcon.png", Vec2(popupSize.width * 0.92f, popupSize.height * 0.05f));

	btnEnd->setOpacity(0);
	btnEnd->setCascadeOpacityEnabled(true);
	btnEnd->setVisible(false);
	if (m_EndButtonVisible) btnEnd->setVisible(true);


	auto btnReward = createBtn([=](Node* sender){
		auto popup = CGameScene::getGameScene()->Reward();
		auto rewardPopup = dynamic_cast<CRewardPopup*>(popup);
		for (auto node : dpVector)
		{
			auto dp = dynamic_cast<CChallengePopupDP*>(node);
			auto data = dp->getChallengeParam();
			rewardPopup->AddRewardToList(data._rewardKey, data._rewardValue);
		}
	}, "rewardIcon.png", startPos[3]);
	if (CChallengeDataManager::Instance()->CheckCompleteAll()){
		btnReward->setColor(COLOR::GOLD);
	}

	this->setOpenAnimation([=](Node* sender){
		btnEnd->runAction(FadeIn::create(0.5f));
		challengesLabel->runAction(FadeIn::create(0.5f));

		auto action = [=](Node* btn, Vec2 pos){
			btn->runAction(
				Spawn::createWithTwoActions(
				EaseExponentialOut::create(
				MoveTo::create(1.3f, pos)),
				FadeIn::create(1.f)));
		};
		int dpIndex = 0;
		for (auto dp : dpVector)
			action(dp, targetPos[dpIndex++]);

		action(btnReward, targetPos[3]);
	});

	this->setCloseAnimation([=](Node* sender){
		btnEnd->runAction(FadeTo::create(0.5f, 0));
		challengesLabel->runAction(FadeTo::create(0.5f, 0));

		auto action = [=](Node* btn, Vec2 pos){
			btn->runAction(
				Spawn::createWithTwoActions(
				EaseSineIn::create(
				MoveTo::create(0.35f, pos)),
				FadeTo::create(0.2f, 0)));
		};

		int dpIndex = 0;
		for (auto dp : dpVector)
			action(dp, startPos[dpIndex++]);

		action(btnReward, startPos[3]);
	});

    this->setDefaultCallback([=](Node* sender){
        this->End(sender);
    });

	return 	CPopup::show(parent, zOrder);
}


CPopup* CChallengePopup::EndButtonVisible(bool visible)
{
	m_EndButtonVisible = visible;
	return this;
}

void CChallengePopup::Skip(CChallengePopupDP* sender, int posIndex)
{
    
}

void CChallengePopup::End(Node* sender){
	CGameScene::getGameScene()->GameResult();
	this->popupClose();
}
