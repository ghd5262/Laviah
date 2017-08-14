#include "GoalPopup.h"
#include "GoalPopupDP.h"
#include "RewardPopup.h"
#include "../MyButton.h"
#include "../../DataManager/UserDataManager.h"
#include "../../Scene/GameScene.h"
#include "../../SDKBOX/SDKBoxHeaders.h"
#include <array>

CGoalPopup* CGoalPopup::create()
{
	CGoalPopup *pRet = new(std::nothrow) CGoalPopup();
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

bool CGoalPopup::init()
{
	if (!CPopup::init()) return false;
	return true;
}

CPopup* CGoalPopup::show(Node* parent, int zOrder/* = 0*/)
{
    m_AchievementList.resize(ACHIEVEMENT_DEFINE::LIMIT_COUNT);
    std::fill(m_AchievementList.begin(), m_AchievementList.end(), nullptr);

	// To avoid duplicate check.
	GVALUE->Clear();

    auto popupSize = this->getContentSize();
    m_DPStartPosArray = {
        Vec2(popupSize.width * 0.5f, popupSize.height * 0.3f),
        Vec2(popupSize.width * 0.5f, popupSize.height * 0.1f),
        Vec2(popupSize.width * 0.5f, popupSize.height * 0.0f),
        Vec2(popupSize.width * 0.5f, popupSize.height * -0.5f)
    };
    
    m_DPTargetPosArray = {
        Vec2(popupSize.width * 0.5f, popupSize.height * 0.6f),
        Vec2(popupSize.width * 0.5f, popupSize.height * 0.5),
        Vec2(popupSize.width * 0.5f, popupSize.height * 0.4),
        Vec2(popupSize.width * 0.5f, popupSize.height * 0.2f)
    };
    
    this->initAchievementList();
    
	auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
	if (bg != nullptr){
		bg->setIgnoreAnchorPointForPosition(false);
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(popupSize / 2);
		this->addChild(bg);
	}

	auto achievementsLabel = Label::createWithSystemFont(TRANSLATE("GOAL_POPUP_TITLE"), FONT::MALGUNBD, 80);
	if (achievementsLabel != nullptr)
	{
		achievementsLabel->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.8f));
        achievementsLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		achievementsLabel->setOpacity(0);
		this->addChild(achievementsLabel);
	}

	auto createBtn = [=](const std::function<void(Node*)> &callback, std::string icon, Vec2 pos){
		auto btn = CMyButton::create()
			->addEventListener(callback)
            ->setButtonSingleUse(true)
			->setButtonNormalImage(icon)
			->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->setButtonPosition(pos)
			->show(this);
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
        
        std::vector<sREWARD_DATA> rewardList;
        rewardList.clear();
        for (auto node : m_AchievementList)
        {
            if(node == nullptr) continue;
            
            auto dp         = dynamic_cast<CGoalPopupDP*>(node);
            auto data       = dp->getAchievementParam();
            auto levelData  = CAchievementDataManager::Instance()->getCurLevelDataByIndex(data._index, false);
            auto value      = CAchievementDataManager::Instance()->getNormalAchievementRewardValue(data._index);
            rewardList.emplace_back(levelData._rewardKey, value);
        }
        
        CGameScene::getGameScene()->Reward([=](bool isPlay){
            // If there are more non-completed achievements than limit count.
            // Set achievements until non-exist.
            if (CAchievementDataManager::Instance()->NonCompleteAchievementExist() >= ACHIEVEMENT_DEFINE::LIMIT_COUNT){
                // Do open achievement popup again.
                CGameScene::getGameScene()->OpenGoalPopup();
                this->popupClose();
            }
            else{
                this->GoHome(nullptr);
            }
        }, rewardList);

        
        // TODO: If there are no more achievements. do not open achievement popup
        CAchievementDataManager::Instance()->getNewAchievements();
        
    }, "freeCoinIcon.png", m_DPStartPosArray[3]);
    btnReward->setVisible(false);

    
	this->setOpenAnimation([=](Node* sender){
		auto action = [=](Node* btn, Vec2 pos){
			btn->runAction(
				Spawn::createWithTwoActions(
				EaseExponentialOut::create(
				MoveTo::create(1.3f, pos)),
				FadeIn::create(1.f)));
		};
		int dpIndex = 0;
        for (auto dp : m_AchievementList) {
            if(dp == nullptr) continue;
            
            action(dp, m_DPTargetPosArray[dpIndex++]);
        }

        auto fadeIn = [=](Node* sender){
            sender->setOpacity(0);
            sender->runAction(FadeIn::create(0.5f));
        };
        
        fadeIn(btnReset);
        fadeIn(btnHome);
        fadeIn(achievementsLabel);
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
        for (auto dp : m_AchievementList){
            if(dp == nullptr) continue;
            
            action(dp, m_DPStartPosArray[dpIndex++]);
        }
        
		action(btnReward, m_DPStartPosArray[3]);
        btnReset->runAction(FadeTo::create(0.3f, 0));
        btnHome->runAction(FadeTo::create(0.3f, 0));
        achievementsLabel->runAction(FadeTo::create(0.3f, 0));
    });

    this->setDefaultCallback([=](Node* sender){
        this->GoHome(sender);
    });
    
    // Do below when achievement was completed all.
    if (CAchievementDataManager::Instance()->CheckCompleteAll()){
        CRewardPopup::createFlyAction(btnReward,
                                      Vec2(popupSize.width * 0.5f, popupSize.height * 0.25f),
                                      Vec2(popupSize.width * 0.5f, popupSize.height * 0.2f));
        btnReset->setVisible(false);
        btnHome->setVisible(false);
        this->setDefaultCallback([=](Node* sender){});
        this->setDefaultCallbackCleanUp(false);
    }
    
    return 	CPopup::show(parent, zOrder);
}

void CGoalPopup::initAchievementList()
{
    int posIndex = 0;
    //    auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::ACHIEVEMENT_CUR_NORMAL_LIST);
    auto pickedList = CAchievementDataManager::Instance()->getPickedAchievementList();
    for (auto achievement : pickedList)
    {
        auto index = achievement.second->_index;
        auto achievementData = CAchievementDataManager::Instance()->getNormalAchievementByIndex(index);
        this->createAchievementDP(achievementData, posIndex++, false);
    }
}

void CGoalPopup::createAchievementDP(const ACHIEVEMENT* data, int posIndex, bool isSkip)
{
    auto dp = CGoalPopupDP::create(*data, posIndex)
    ->addSkipEventListner([=](CGoalPopupDP* sender, int posIdx){
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
    
    m_AchievementList.at(posIndex) = dp;
}

void CGoalPopup::Skip(CGoalPopupDP *sender, int posIndex)
{
    auto dp = sender;
    CGameScene::getGameScene()->CreateAlertPopup()
    ->setPositiveButton([=](Node* sender){
        
        CUnityAdsManager::Instance()->ShowUnityAds([=](){
            auto achievementMng = CAchievementDataManager::Instance();
            auto index = dp->getAchievementParam()._index;
            auto data  = achievementMng->getNormalAchievementByIndex(index);
            
            achievementMng->setAchievementStateByIndex(index, ACHIEVEMENT_STATE::FINISHED, false);
            GVALUE->NORMAL_ACHIEVEMENT_CLEAR_COUNT += 1;
            dp->popupClose();
            this->createAchievementDP(data, posIndex, true);
        });
    }, TRANSLATE("BUTTON_YES"))
    ->setNegativeButton([=](Node* sender){
    }, TRANSLATE("BUTTON_NO"))
    ->setMessage(TRANSLATE("GOAL_SKIP_CHECK"))
    ->show(CGameScene::getPopupLayer(), ZORDER::POPUP);
}

void CGoalPopup::Reset(Node* sender){
    CGameScene::getGameScene()->GameStart();
    this->popupClose();
}

void CGoalPopup::GoHome(Node* sender){
    CGameScene::getGameScene()->OpenGameMenuLayer();
    this->popupClose();
}
