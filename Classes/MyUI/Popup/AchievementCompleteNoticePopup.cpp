#include "AchievementCompleteNoticePopup.h"
#include "../MyButton.h"
#include "../../Scene/GameScene.h"
#include "../../GameObject/ObjectManager.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/AchievementDataManager.hpp"
#include "../../SDKBOX/SDKBoxHeaders.h"
USING_NS_CC;

namespace ACHIEVEMENT_COMPLETE_NOTICE{
	static const std::string TAG_UPDATE = "checkAchievementCompleteOnRealTime";
	static const std::string TAG_SHOW   = "achievementNoticeShow";
    static const float STAY_LIMIT_TIME  = 3.f;
};

CAchievementCompleteNoticePopup* CAchievementCompleteNoticePopup::create()
{
	CAchievementCompleteNoticePopup *pRet = new(std::nothrow) CAchievementCompleteNoticePopup();
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

bool CAchievementCompleteNoticePopup::init()
{
	if (!CPopup::init()) return false;
	this->schedule([=](float delta){
		this->checkAchievementCompleteOnRealTime();
	}, 0.5f, ACHIEVEMENT_COMPLETE_NOTICE::TAG_UPDATE);

	m_LayerBG = Sprite::create("resultPopup_2.png");
	this->setContentSize(m_LayerBG->getContentSize());
	m_LayerBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_LayerBG->setPosition(this->getContentSize() / 2);
	m_LayerBG->setOpacity(0);
	m_LayerBG->setCascadeOpacityEnabled(true);
	this->addChild(m_LayerBG);

	auto popupSize = this->getContentSize();

	m_AchievementLabel = Label::createWithSystemFont("", FONT::MALGUNBD, 45,
		Size(popupSize.width * 0.8f, popupSize.height),
		TextHAlignment::CENTER,
		TextVAlignment::CENTER);
	m_AchievementLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_AchievementLabel->setPosition(popupSize / 2);
	m_LayerBG->addChild(m_AchievementLabel);

	auto completeIcon = Sprite::create("completeIcon.png");
	completeIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	completeIcon->setPosition(Vec2(popupSize.width * 0.92f, popupSize.height * 0.5f));
	m_LayerBG->addChild(completeIcon);

	return true;
}

void CAchievementCompleteNoticePopup::checkAchievementCompleteOnRealTime()
{
	if (CObjectManager::Instance()->getIsGamePause()) return;
    if (!m_Checkable) return;

	auto data = CAchievementDataManager::Instance()->CompleteCheckRealTime(false);
	if (data != nullptr) {
        CGoogleAnalyticsManager::LogEventValue(GA_CATEGORY::GAME_PLAY, GA_ACTION::GOAL_COMPLETE, data->_index);
        auto contents = CAchievementDataManager::Instance()->getAchievementContentsByIndex(data->_index, false);
        m_Checkable   = false;
        m_AchievementLabel->setString(contents);
		this->show();
	}
}

void CAchievementCompleteNoticePopup::show()
{
	this->scheduleOnce([=](float delta){

		Vec2 startPos = Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 2.f);
		Vec2 targetPos = this->getContentSize() / 2;

		m_LayerBG->setPosition(startPos);

		auto downAction = Spawn::createWithTwoActions(
			EaseExponentialOut::create(
			MoveTo::create(0.5f, targetPos)),
			FadeTo::create(0.3f, 255 * 0.8f));
		auto delayAction = DelayTime::create(ACHIEVEMENT_COMPLETE_NOTICE::STAY_LIMIT_TIME);
		auto upAction = Spawn::createWithTwoActions(
			EaseSineIn::create(
			MoveTo::create(0.3f, startPos)),
			FadeTo::create(0.1f, 0));
        auto callFunc = CallFunc::create([=](){
            m_Checkable = true;
        });
		auto sequenceAction = Sequence::create(downAction, delayAction, upAction, callFunc, nullptr);

		m_LayerBG->runAction(sequenceAction);

	}, 0.f, ACHIEVEMENT_COMPLETE_NOTICE::TAG_SHOW);
}

