#include "RewardPopup.h"
#include "../MyButton.h"
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

	auto sizeDown = ScaleTo::create(0.8f, 1.f);
	auto sizeUp = ScaleTo::create(0.8f, 1.2f);
	auto sequence = Sequence::createWithTwoActions(sizeDown, sizeUp);
	auto repeat = RepeatForever::create(sequence);

	rewardIcon->runAction(repeat);

	auto touchScreen = CMyButton::create()
		->addEventListener([=](Node* sender){
	
		if (m_RewardIndex >= m_RewardList.size()){
			this->popupClose();
			return;
		}

		auto data = m_RewardList.at(m_RewardIndex);
		CChallengeDataManager::Instance()->RewardByKey(data._key, data._value);
		m_RewardIndex++;
	})
		->setDefaultClickedAnimation(eCLICKED_ANIMATION::NONE)
		->setLayer(LayerColor::create(COLOR::TRANSPARENT_ALPHA, popupSize.width, popupSize.height))
		->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setButtonPosition(this->getContentSize() / 2)
		->show(this); 

	return true;
}

void CRewardPopup::AddRewardToList(std::string key, int value)
{
	m_RewardList.emplace_back(sREWARD_DATA(key, value));
}