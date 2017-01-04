#include "ChallengePopup.h"
#include "ChallengePopupDP.h"
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

	auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
	if (bg != nullptr){
		bg->setIgnoreAnchorPointForPosition(false);
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(this->getContentSize() / 2);
		this->addChild(bg);
	}
    
	auto challengesLabel = Label::createWithTTF("CHALLENGES", FONT::MALGUNBD, 80);
	if (challengesLabel != nullptr)
	{
		challengesLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		challengesLabel->setPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.6f));
		challengesLabel->setColor(COLOR::DARKGRAY);
		this->addChild(challengesLabel);
		challengesLabel->setOpacity(0);
	}

    Vec2 posArray[] = {
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.5f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.45f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.4f)
    };
    
	std::vector<CPopup*> dpVector;

    int idx = 0;
    auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_LIST);
    for(auto index : *list)
    {
		auto challengeData = CChallengeDataManager::Instance()->getChallengeByIndex(index);
		dpVector.emplace_back(CChallengePopupDP::create(challengeData)
			->addSkipEventListner([=](CChallengePopupDP* sender){
			this->skip(sender);
		})
			->setBackgroundVisible(false)
			->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->setPopupPosition(posArray[idx++])
			->show(this, ZORDER::POPUP));
	}

	this->setOpenAnimation([=](Node* sender){
		challengesLabel->runAction(FadeIn::create(0.5f));

	});

	this->setCloseAnimation([=](Node* sender){
		challengesLabel->runAction(FadeTo::create(0.5f, 0));

		for (auto dp : dpVector)
			dp->popupClose();
	});

	return true;
}

void CChallengePopup::skip(CChallengePopupDP* sender)
{
    
}
