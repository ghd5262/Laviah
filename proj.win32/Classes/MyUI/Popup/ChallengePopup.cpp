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
    
    Vec2 posArray[] = {
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.5),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.4),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.3)
    };
    
    int idx = 0;
    auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_LIST);
    for(auto index : *list)
    {
        auto challengeData = CChallengeDataManager::Instance()->getChallengeByIndex(index);
        CChallengePopupDP::create(challengeData)
        ->addSkipEventListner([=](CChallengePopupDP* sender){
            this->skip(sender);
        })
        ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setPopupPosition(posArray[idx++])
        ->show(this, ZORDER::POPUP);
    }

	this->setOpenAnimation([=](Node* sender){
		
	});

	this->setCloseAnimation([=](Node* sender){

	});

	return true;
}

void CChallengePopup::skip(CChallengePopupDP* sender)
{
    
}
