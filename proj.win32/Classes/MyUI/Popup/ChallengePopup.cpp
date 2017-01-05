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
/*
	auto popupSize = this->getContentSize();
	auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
	if (bg != nullptr){
		bg->setIgnoreAnchorPointForPosition(false);
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(popupSize / 2);
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
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.425f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.35f)
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

	auto btnEnd = CMyButton::create()
		->addEventListener([=](Node* sender){
		this->End(sender);
	})
		->setButtonNormalImage("endIcon.png")
		->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setButtonPosition(Vec2(popupSize.width * 0.92f, popupSize.height * 0.05f))
		->show(this);

	btnEnd->setOpacity(0);
	btnEnd->setCascadeOpacityEnabled(true);

	this->setOpenAnimation([=](Node* sender){
		btnEnd->runAction(FadeIn::create(0.5f));
		challengesLabel->runAction(FadeIn::create(0.5f));
	});

	this->setCloseAnimation([=](Node* sender){
		btnEnd->runAction(FadeTo::create(0.5f, 0));
		challengesLabel->runAction(FadeTo::create(0.5f, 0));
		for (auto dp : dpVector)
			dp->popupClose();
	});
*/
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
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.425f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.35f)
	};

	std::vector<CPopup*> dpVector;

	int idx = 0;
	auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_LIST);
	for (auto index : *list)
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

	auto btnEnd = CMyButton::create()
		->addEventListener([=](Node* sender){
		this->End(sender);
	})
		->setButtonNormalImage("endIcon.png")
		->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setButtonPosition(Vec2(popupSize.width * 0.92f, popupSize.height * 0.05f))
		->show(this);

	btnEnd->setOpacity(0);
	btnEnd->setCascadeOpacityEnabled(true);
	btnEnd->setVisible(false);
	if (m_EndButtonVisible) btnEnd->setVisible(true);

	this->setOpenAnimation([=](Node* sender){
		btnEnd->runAction(FadeIn::create(0.5f));
		challengesLabel->runAction(FadeIn::create(0.5f));
	});

	this->setCloseAnimation([=](Node* sender){
		btnEnd->runAction(FadeTo::create(0.5f, 0));
		challengesLabel->runAction(FadeTo::create(0.5f, 0));
		for (auto dp : dpVector)
			dp->popupClose();
	});


	return 	CPopup::show(parent, zOrder);
}


CPopup* CChallengePopup::EndButtonVisible(bool visible)
{
	m_EndButtonVisible = visible;
	return this;
}

void CChallengePopup::skip(CChallengePopupDP* sender)
{
    
}

void CChallengePopup::End(Node* sender){
	CGameScene::getGameScene()->GameResult();
	this->popupClose();
}
