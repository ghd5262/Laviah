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
    m_ChallengeList.resize(CHALLENGE_DEFINE::LIMIT_COUNT);
    std::fill(m_ChallengeList.begin(), m_ChallengeList.end(), nullptr);

	// To avoid duplicate check.
	GLOBAL->Clear();

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

	auto challengesLabel = Label::createWithSystemFont(TRANSLATE("CHALLENGE_POPUP_TITLE"), FONT::MALGUNBD, 80);
	if (challengesLabel != nullptr)
	{
		challengesLabel->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.8f));
        challengesLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		challengesLabel->setOpacity(0);
		this->addChild(challengesLabel);
	}

	auto createBtn = [=](const std::function<void(Node*)> &callback, std::string icon, Vec2 pos){
		auto btn = CMyButton::create()
			->addEventListener(callback)
            ->setButtonSingleUse(true)
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
            
            // If there are more non-completed challenges than limit count.
            // Set challenges until non-exist.
			if (CChallengeDataManager::Instance()->NonCompleteChallengeExist() >= CHALLENGE_DEFINE::LIMIT_COUNT){
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
	}, 1.3f);

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

    this->setDefaultCallback([=](Node* sender){
        this->GoHome(sender);
    });
    
    // Do below when challenge was completed all.
    if (CChallengeDataManager::Instance()->CheckCompleteAll()){
        btnReward->setTouchEnable(true);
        btnReward->setColor(COLOR::GOLD);
        btnReset->setVisible(false);
        btnHome->setVisible(false);
        this->setDefaultCallback([=](Node* sender){});
        this->setDefaultCallbackCleanUp(false);
    }
    
    return 	CPopup::show(parent, zOrder);
}

void CChallengePopup::initChallengeList()
{
    int posIndex = 0;
    auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_NORMAL_LIST);
    for (auto index : list)
    {
        auto challengeData = CChallengeDataManager::Instance()->getNormalChallengeByIndex(index);
        this->createChallengeDP(challengeData, posIndex++, false);
    }
}

void CChallengePopup::createChallengeDP(const sCHALLENGE_PARAM* data, int posIndex, bool isSkip)
{
    auto dp = CChallengePopupDP::create(data, posIndex)
    ->addSkipEventListner([=](CChallengePopupDP* sender, int posIdx){
        this->Skip(sender, posIdx);
    })
    ->setDefaultCallbackEnable(false)
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
    auto dp = sender;
	CGameScene::getGameScene()->CreateAlertPopup()
		->setPositiveButton([=](Node* sender){
		auto newData = CChallengeDataManager::Instance()->SkipChallenge(dp->getChallengeParam()._index);
		dp->popupClose();
		this->createChallengeDP(newData, posIndex, true);
	}, TRANSLATE("BUTTON_YES"))
		->setNegativeButton([=](Node* sender){
	}, TRANSLATE("BUTTON_NO"))
		->setMessage(TRANSLATE("CHALLENGE_SKIP_CHECK"))
		->show(CGameScene::getGameScene(), ZORDER::POPUP);
}

void CChallengePopup::Reset(Node* sender){
    CGameScene::getGameScene()->GameStart();
    this->popupClose();
}

void CChallengePopup::GoHome(Node* sender){
    CGameScene::getGameScene()->OpenGameMenuLayer();
    this->popupClose();
}
