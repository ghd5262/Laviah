#include "ChallengePopup.h"
#include "ChallengePopupDP.h"
#include "RewardPopup.h"
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
	return true;
}

CPopup* CChallengePopup::show(Node* parent, int zOrder/* = 0*/)
{
    m_ChallengeList.resize(CHALLENGE::LIMIT_COUNT);
    std::fill(m_ChallengeList.begin(), m_ChallengeList.end(), nullptr);

    auto popupSize = this->getContentSize();
    m_DPStartPosArray = {
        Vec2(popupSize.width * 0.5f, popupSize.height * 0.3f),
        Vec2(popupSize.width * 0.5f, popupSize.height * 0.1f),
        Vec2(popupSize.width * 0.5f, popupSize.height * 0.0f),
        Vec2(popupSize.width * 0.5f, popupSize.height * -0.1f)
    };
    
    m_DPTargetPosArray = {
        Vec2(popupSize.width * 0.5f, popupSize.height * 0.6f),
        Vec2(popupSize.width * 0.5f, popupSize.height * 0.5),
        Vec2(popupSize.width * 0.5f, popupSize.height * 0.4),
        Vec2(popupSize.width * 0.5f, popupSize.height * 0.2f)
    };
    
    this->initChallengeList();
    
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
        challengesLabel->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.8f));
        challengesLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        challengesLabel->setColor(COLOR::BRIGHTGRAY);
		challengesLabel->setOpacity(0);
		this->addChild(challengesLabel);
	}


	auto createBtn = [=](const std::function<void(Node*)> &callback, std::string icon, Vec2 pos){
		auto btn = CMyButton::create()
			->addEventListener(callback)
			->setButtonNormalImage(icon)
			->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->setButtonPosition(pos)
			->show(this);
        btn->setOpacity(0);
        btn->setCascadeOpacityEnabled(true);
        
        return btn;
	};

    auto btnReset = createBtn([=](Node* sender){
		this->Reset(sender);
	}, "resetIcon.png", Vec2(popupSize.width * 0.92f, popupSize.height * 0.05f));
    
    auto btnHome = createBtn([=](Node* sender){
        this->GoHome(sender);
    }, "homeIcon.png", Vec2(popupSize.width * 0.08f, popupSize.height * 0.05f));
    
    auto btnReward = createBtn([=](Node* sender){
		auto popup = CGameScene::getGameScene()->Reward();
		auto rewardPopup = dynamic_cast<CRewardPopup*>(popup);
        rewardPopup->setExitCallback([=](){
            
			if (CChallengeDataManager::Instance()->NonCompleteChallengeExist(1, false) >= CHALLENGE::LIMIT_COUNT){
				// Do open challenge popup again.
				CGameScene::getGameScene()->ShowChallenge();
				this->popupClose();
			}
			else{
				this->GoHome(nullptr);
			}
        });
        
		for (auto node : m_ChallengeList)
		{
            if(node == nullptr) continue;
            
			auto dp = dynamic_cast<CChallengePopupDP*>(node);
			auto data = dp->getChallengeParam();
			rewardPopup->AddRewardToList(data._rewardKey, data._rewardValue);
		}
        
        // TODO: If there are no more challenges. do not open challenge popup
        CChallengeDataManager::Instance()->getNewChallenges();
        
	}, "rewardIcon.png", m_DPStartPosArray[3])
    ->setTouchEnable(false, Color3B::WHITE);
    
    this->setDefaultCallback([=](Node* sender){
        this->GoHome(sender);
    });
    
    // Do below when challenge was completed all.
	if (CChallengeDataManager::Instance()->CheckCompleteAll()){
        btnReward->setTouchEnable(true);
        btnReward->setColor(COLOR::GOLD);
        btnReset->setVisible(false);
        btnHome->setVisible(false);
        this->setDefaultCallback(nullptr);
	}

	this->setOpenAnimation([=](Node* sender){
		auto action = [=](Node* btn, Vec2 pos){
			btn->runAction(
				Spawn::createWithTwoActions(
				EaseExponentialOut::create(
				MoveTo::create(1.3f, pos)),
				FadeIn::create(1.f)));
		};
		int dpIndex = 0;
        for (auto dp : m_ChallengeList) {
            if(dp == nullptr) continue;
            
            action(dp, m_DPTargetPosArray[dpIndex++]);
        }

        action(btnReward, m_DPTargetPosArray[3]);
        btnReset->runAction(FadeIn::create(0.5f));
        btnHome->runAction(FadeIn::create(0.5f));
        challengesLabel->runAction(FadeIn::create(0.5f));
	});

	this->setCloseAnimation([=](Node* sender){

		auto action = [=](Node* btn, Vec2 pos){
			btn->runAction(
				Spawn::createWithTwoActions(
				EaseSineIn::create(
				MoveTo::create(0.35f, pos)),
				FadeTo::create(0.2f, 0)));
		};

		int dpIndex = 0;
        for (auto dp : m_ChallengeList){
            if(dp == nullptr) continue;
            
            action(dp, m_DPStartPosArray[dpIndex++]);
        }
        
		action(btnReward, m_DPStartPosArray[3]);
        btnReset->runAction(FadeTo::create(0.5f, 0));
        btnHome->runAction(FadeTo::create(0.5f, 0));
        challengesLabel->runAction(FadeTo::create(0.5f, 0));
    });

	return 	CPopup::show(parent, zOrder);
}

void CChallengePopup::initChallengeList()
{
    int posIndex = 0;
    auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_LIST);
    for (auto index : *list)
    {
        auto challengeData = CChallengeDataManager::Instance()->getChallengeByIndex(index);
        this->createChallengeDP(challengeData, posIndex++, false);
    }
}

void CChallengePopup::createChallengeDP(const sCHALLENGE_PARAM* data, int posIndex, bool isSkip)
{
    auto dp = CChallengePopupDP::create(data, posIndex)
    ->addSkipEventListner([=](CChallengePopupDP* sender, int posIdx){
        this->Skip(sender, posIdx);
    })
    ->setBackgroundVisible(false)
    ->setPopupPosition(m_DPTargetPosArray[posIndex])
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->show(this, ZORDER::POPUP);
    
    if(!isSkip){
        dp->setPosition(m_DPStartPosArray[posIndex]);
        dp->setOpenAnimation(nullptr);
        dp->setCloseAnimation(nullptr);
    }
    
    m_ChallengeList.at(posIndex) = dp;
}

void CChallengePopup::Skip(CChallengePopupDP *sender, int posIndex)
{
    auto scene = CGameScene::getGameScene();
    auto dp = sender;
    CPopup::create()
    ->setPositiveButton([=](Node* sender){
        auto newData = CChallengeDataManager::Instance()->SkipChallenge(dp->getChallengeParam()._index);
        dp->popupClose();
        this->createChallengeDP(newData, posIndex, true);
    }, "Yes")
    ->setNegativeButton([=](Node* sender){
    }, "No")
    ->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
    ->setMessage("Are you sure you want to skip this challenge?")
    ->setMessageFont(Color3B::BLACK, 40)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(scene->getContentSize() / 2)
    ->show(scene, ZORDER::POPUP);
}

void CChallengePopup::Reset(Node* sender){
    CGameScene::getGameScene()->GameStart();
    this->popupClose();
}

void CChallengePopup::GoHome(Node* sender){
    CGameScene::getGameScene()->OpenGameMenuLayer();
    this->popupClose();
}
