#include "ChallengePopup.h"
#include "../MyButton.h"
#include "../../DataManager/UserDataManager.h"
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

	this->setOpenAnimation([=](Node* sender){
		
	});

	this->setCloseAnimation([=](Node* sender){

	});

	return true;
}