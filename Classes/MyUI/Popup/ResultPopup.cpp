#include "ResultPopup.h"
#include "RewardPopup.h"
#include "../MyButton.h"
#include "../ScoreUI.h"
#include "../UserCoinButton.h"
#include "../ComboScore.h"
#include "../../SDKUtil/SDKUtil.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/FreeRewardManager.hpp"
#include "../../DataManager/ChallengeDataManager.hpp"
#include "../../DataManager/ChallengeRewarder/ChallengeRewarder.hpp"
#include "../../Common/StringUtility.h"
#include "../../Scene/GameScene.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Player.h"
#include "../../Facebook/FacebookManager.hpp"
#include <array>

CResultPopup* CResultPopup::create()
{
	CResultPopup *pRet = new(std::nothrow) CResultPopup();
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

bool CResultPopup::init()
{
	if (!CPopup::init()) return false;

	auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
	if (bg != nullptr){
		bg->setIgnoreAnchorPointForPosition(false);
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(Vec2(this->getContentSize().width * 0.5f,
                             this->getContentSize().height * 1.5f));
		this->addChild(bg);
	}
    auto layerSize = bg->getContentSize();
    
    std::array<Vec2, 6> posArray = {
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.7f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.65f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.6f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.55f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.45f),
    };
    
    std::array<std::string, 6> resultIcon = {
		"starIcon.png",
		"comboIcon.png",
        "coinIcon.png",
        "challengeIcon_2.png",
        "",
        "bestScoreIcon.png"
    };
    
    std::array<std::string, 6> resultContent = {
		TRANSLATE("RESULT_SCORE"),
		TRANSLATE("RESULT_COMBO"),
        TRANSLATE("RESULT_COIN"),
        TRANSLATE("RESULT_CHALLENGE"),
        "",
        TRANSLATE("RESULT_BEST_SCORE")
    };
    
    
    // 총 점수 = 달린 총거리 + 별 + 코인 + (보너스타임횟수 * 10000) + (외계주민 * 10000) + (도전과제 * 10000)
    
    auto createLayerBG = [=](Vec2 pos, std::string spriteName){
        auto layerBG = Sprite::create(spriteName);
        layerBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        layerBG->setPosition(pos);
        bg->addChild(layerBG);
        
        return layerBG;
    };
    
    auto createIcon = [=](Node* parent, Vec2 pos, std::string iconImg){
        auto icon = Sprite::create(iconImg);
        icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        icon->setPosition(pos);
        icon->setScale(80 / icon->getContentSize().height);
        parent->addChild(icon);
        
        return icon;
    };
    
    auto createContent = [=](Node* parent, Vec2 pos, std::string content, int fontSize){
        auto contentLabel = Label::createWithSystemFont(content, FONT::MALGUNBD, fontSize);
        contentLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        contentLabel->setPosition(pos);
        parent->addChild(contentLabel);
        
        return contentLabel;
    };
    
    auto createScoreLabel = [=](Node* parent, Vec2 pos, int score, int fontSize){
        auto scoreLabel = Label::createWithTTF(StringUtility::toCommaString(score), FONT::MALGUNBD, fontSize);
        scoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        scoreLabel->setPosition(pos);
        parent->addChild(scoreLabel);
        
        return scoreLabel;
    };
    
    auto createMultipleLabel = [=](Node* parent, Vec2 pos, int multiple){
		auto multipleLabel = Label::createWithTTF(StringUtils::format("%d x ", multiple), FONT::MALGUNBD, 25);
        multipleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        multipleLabel->setPosition(pos);
        parent->addChild(multipleLabel);
    };
    

    auto createNormalLayer = [=](std::string iconImg, std::string content, int score, Vec2 layerPos, int fontSize){
		if (GLOBAL->TOTAL_SCORE + score < INT_MAX )
            GLOBAL->TOTAL_SCORE += score;

		auto layerBG = createLayerBG(layerPos, "resultPopup_2.png");
        createIcon(layerBG, Vec2(layerBG->getContentSize().width * 0.1f,
                                 layerBG->getContentSize().height * 0.5f), iconImg);
        
        createContent(layerBG, Vec2(layerBG->getContentSize().width * 0.15f,
                                    layerBG->getContentSize().height * 0.5f), content, fontSize);
        
        createScoreLabel(layerBG, Vec2(layerBG->getContentSize().width * 0.9f,
                                       layerBG->getContentSize().height * 0.5f), score, fontSize);
        
		return layerBG;
	};

	auto createMultipleLayer = [=](std::string iconImg, std::string content, int score, Vec2 layerPos, int fontSize, int multiple){
		if (GLOBAL->TOTAL_SCORE + (score * multiple) < INT_MAX)
			GLOBAL->TOTAL_SCORE += score * multiple;

        auto layerBG = createLayerBG(layerPos, "resultPopup_2.png");
        createIcon(layerBG, Vec2(layerBG->getContentSize().width * 0.1f,
                                 layerBG->getContentSize().height * 0.5f), iconImg);
        
        createContent(layerBG, Vec2(layerBG->getContentSize().width * 0.15f,
                                    layerBG->getContentSize().height * 0.5f), content, fontSize);
        
        auto scoreLabel = createScoreLabel(layerBG,
                                           Vec2(layerBG->getContentSize().width * 0.9f,
                                                layerBG->getContentSize().height * 0.5f), score, fontSize);
        
        createMultipleLabel(layerBG, Vec2(scoreLabel->getPosition().x - scoreLabel->getContentSize().width,
			layerBG->getContentSize().height * 0.4f), multiple);

		return layerBG;
	};
    
    CComboScore::Instance()->ComboScoreReset();
	bool challengeAll    = CChallengeDataManager::Instance()->CheckCompleteAll();
    bool rewardPopupOpen = (GLOBAL->NORMAL_CHALLENGE_CLEAR_COUNT || challengeAll);
    
	createNormalLayer(resultIcon[0],   resultContent[0], GLOBAL->STAR_SCORE,   posArray[0], 50);
	createNormalLayer(resultIcon[1],   resultContent[1], GLOBAL->BEST_COMBO,   posArray[1], 50);
    createMultipleLayer(resultIcon[2], resultContent[2], GLOBAL->COIN_SCORE,   posArray[2], 50, 10);
	createMultipleLayer(resultIcon[3], resultContent[3], GLOBAL->NORMAL_CHALLENGE_CLEAR_COUNT, posArray[3], 50, 100);
    
    // combo가 user best combo면 저장한다.
	auto bestCombo = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BEST_COMBO);
	if (GLOBAL->BEST_COMBO > bestCombo)
	{
		CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::BEST_COMBO, GLOBAL->BEST_COMBO);
	}

    // total score가 best score면 저장한다.
    auto bestScore = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BEST_SCORE);
    std::string totalContent = TRANSLATE("RESULT_TOTAL_SCORE");
    if (GLOBAL->TOTAL_SCORE > bestScore){
        totalContent = TRANSLATE("RESULT_BEST_SCORE");
        bestScore = GLOBAL->TOTAL_SCORE;
        CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::BEST_SCORE, GLOBAL->TOTAL_SCORE);
    }
    
    // save score to facebook
    if (CFacebookManager::IsScoresEnabled()){
        auto oldScore = CFacebookManager::Instance()->getMyFacebookData()->_score;
        if(oldScore < GLOBAL->TOTAL_SCORE){
            // save score to facebook data
            CFacebookManager::Instance()->SaveScore(GLOBAL->TOTAL_SCORE);
            CFacebookManager::Instance()->setSaveScoreListener([=](bool succeed){
                auto oldRank = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::RANK);
                auto newRank = CFacebookManager::Instance()->getMyRank();
                if(oldRank != newRank){
                    CGameScene::getGameScene()->OpenRankUpPopup();
                    CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::RANK, newRank);
                }
            });
        }
    }
    
    auto totalScoreBG = createLayerBG(posArray[4], "resultPopup_1.png");
    
    // create total score content label
    createContent(totalScoreBG, Vec2(totalScoreBG->getContentSize().width * 0.08f,
                                     totalScoreBG->getContentSize().height * 0.5f),
                  totalContent, 60)
    ->setColor(COLOR::BRIGHTGRAY);
    
    // create total score label;
    createScoreLabel(totalScoreBG, Vec2(totalScoreBG->getContentSize().width * 0.9f,
                                        totalScoreBG->getContentSize().height * 0.5f),
                     GLOBAL->TOTAL_SCORE, 60)
    ->setColor(COLOR::BRIGHTGRAY);
    
    // create my best score layer
    createNormalLayer(resultIcon[5], resultContent[5], bestScore,  posArray[5], 50);
    CUserDataManager::Instance()->CoinUpdate(GLOBAL->COIN_SCORE);
    
    // get exp
    CUserDataManager::Instance()->ExpAdd(std::max(1, GLOBAL->TOTAL_SCORE / 100));
    
    
    // create button lambda
    auto createButton = [=](const std::function<void(Node*)> &callback, std::string name, Vec2 pos, bool visible){
        auto button = CMyButton::create()
        ->addEventListener(callback)
        ->setButtonSingleUse(true)
        ->setButtonNormalImage(name)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(pos)
        ->show(this);
        button->setTouchEnable(false);
        button->setVisible(visible);
        return button;
    };
    
    std::array<Vec2, 6> btnPosArray = {
        Vec2(layerSize.width * 0.08f, layerSize.height * 0.05f),
        Vec2(layerSize.width * 0.92f, layerSize.height * 0.05f),
        Vec2(layerSize.width * 0.92f, layerSize.height * 0.05f),
        Vec2(layerSize.width * 0.25f, layerSize.height * 0.35f),
        Vec2(layerSize.width * 0.5f,  layerSize.height * 0.35f),
        Vec2(layerSize.width * 0.75f, layerSize.height * 0.35f),
    };
    
    std::array<std::string, 6> btnIconArray = {
        "homeIcon.png",
        "resetIcon.png",
        "endIcon.png",
        "videoIcon.png",
        "characterIcon.png",
        "rewardIcon.png",
    };
    
    std::array<std::function<void(Node*)>, 6> btnListenerArray = {
        [=](Node* sender) { this->GoHome(sender);           },
        [=](Node* sender) { this->Reset(sender);            },
        [=](Node* sender) { this->End(sender);              },
        [=](Node* sender) { this->GetCoinFromVideo(sender); },
        [=](Node* sender) { this->GetNewCharacter(sender);  },
        [=](Node* sender) { this->GetFreeReward(sender);    },
    };
    
    std::array<bool, 6> btnVisibleArray = {
        (!rewardPopupOpen),
        (!rewardPopupOpen),
        ( rewardPopupOpen),
        ( true ),
        ( CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::COIN) >= 1000 ),
        ( CFreeRewardManager::Instance()->getRewardAble() ),
    };
    
    
    // create button array
    std::array<CMyButton*, 6> btnArray;
    
    // create buttons
    for(int i = 0; i < 6; i++)
        btnArray[i] = createButton(btnListenerArray[i], btnIconArray[i], btnPosArray[i], btnVisibleArray[i]);
    
    
    // create result label
    auto resultLabel = Label::createWithSystemFont(TRANSLATE("RESULT_POPUP_TITLE"), FONT::MALGUNBD, 80);
    resultLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    resultLabel->setPosition(Vec2(layerSize.width * 0.5f,
                                  layerSize.height * 0.8f));
    this->addChild(resultLabel);
    
    
    // create user coin layer
    auto btnUserCoin = CUserCoinButton::create();
    btnUserCoin->setPosition(Vec2(layerSize.width * 0.5f,
                                  layerSize.height * 0.05f));
    btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(btnUserCoin);
    
    
    this->setOpenAnimation([=](Node* sender){
        // clear all bullets
        CObjectManager::Instance()->ReturnToMemoryBlockAll();
        
        auto moveAction = MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f));
        auto easeAction = EaseExponentialInOut::create(moveAction);
        auto callFunc   = CallFunc::create([=](){
            
            for(auto btn : btnArray)
                btn->setTouchEnable(true);
            
            // if there are some reward from challenge. open reward popup after end of result popup.
            if ( rewardPopupOpen )  this->changeDefaultCallback([=](Node* sender){ this->End(sender);    });
            else                    this->changeDefaultCallback([=](Node* sender){ this->GoHome(sender); });
    
            this->setDefaultCallbackCleanUp(true);
        });
        auto sequance   = Sequence::createWithTwoActions(easeAction, callFunc);
        bg->runAction(sequance);
        
        
        auto action = [=](Node* owner){
            auto delay = DelayTime::create(1.f);
            auto fade  = FadeIn::create(0.5f);
            auto sequence = Sequence::createWithTwoActions(delay, fade);
            owner->setOpacity(0);
            owner->runAction(sequence);
        };
        
        for(auto btn : btnArray)
            action(btn);
        
        action(resultLabel);
        action(btnUserCoin);
    });
    
	this->setCloseAnimation([=](Node* sender){

        for(auto btn : btnArray)
            btn->runAction(FadeTo::create(0.3f, 0));
        
		resultLabel->runAction(FadeTo::create(0.3f, 0));
		btnUserCoin->runAction(FadeTo::create(0.3f, 0));
        
        auto action = [=](Node* owner){
            auto move = MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f,
                                                  layerSize.height * 1.5f));
            auto ease = EaseExponentialInOut::create(move);
            owner->runAction(ease);
        };
        
        if( rewardPopupOpen )   bg->runAction(FadeTo::create(0.3f, 0));
        else                    action(bg);
	});
   
    this->setDefaultCallback([=](Node* sender){}, false);
    
	return true;
}

void CResultPopup::Reset(Node* sender){
	CGameScene::getGameScene()->GameStart();
    this->popupClose(1.3f);
}

void CResultPopup::GoHome(Node* sender){
	CGameScene::getGameScene()->OpenGameMenuLayer();
	this->popupClose(1.3f);
}

void CResultPopup::End(Node* sender){
    CGameScene::getGameScene()->ShowChallenge();
    this->popupClose(1.3f);
}

void CResultPopup::GetCoinFromVideo(cocos2d::Node* sender)
{
    CSDKUtil::Instance()->ShowRewardUnityAds([=](){
        this->createRewardPopup(CHALLENGE_REWARD_KEY::REWARD_COIN_RANDOM, 50);
    });
}

void CResultPopup::GetNewCharacter(cocos2d::Node* sender)
{
    this->createRewardPopup(CHALLENGE_REWARD_KEY::REWARD_CHARACTER_RANDOM, 0);
}

void CResultPopup::GetFreeReward(cocos2d::Node* sender)
{
    this->createRewardPopup(CHALLENGE_REWARD_KEY::REWARD_COIN_RANDOM, 150);
    CFreeRewardManager::Instance()->setRewardAble(false);
}

void CResultPopup::createRewardPopup(std::string key, int value)
{
    auto popup = CGameScene::getGameScene()->Reward();
    auto rewardPopup = dynamic_cast<CRewardPopup*>(popup);
    rewardPopup->AddRewardToList(key, value);
}