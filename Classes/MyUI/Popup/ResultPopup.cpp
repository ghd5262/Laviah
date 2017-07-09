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
#include "../../DataManager/AchievementDataManager.hpp"
#include "../../DataManager/AchievementRewarder/AchievementRewarder.hpp"
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
    
    std::array<Vec2, 6> posDown  = {
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.4f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.3f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.2f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.1f),
        Vec2(layerSize.width * 0.5f, layerSize.height * 0.0f),
        Vec2(layerSize.width * 0.5f, layerSize.height * -.1f),
    };
    
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
        "achievementIcon.png",
        "",
        "bestScoreIcon.png"
    };
    
    std::array<std::string, 6> resultContent = {
		TRANSLATE("RESULT_SCORE"),
		TRANSLATE("RESULT_COMBO"),
        TRANSLATE("RESULT_COIN"),
        TRANSLATE("RESULT_GOAL"),
        "",
        TRANSLATE("RESULT_BEST_SCORE")
    };
    
    
    // 총 점수 = 달린 총거리 + 별 + 코인 + (보너스타임횟수 * 10000) + (외계주민 * 10000) + (도전과제 * 10000)
    
    auto createLayerBG = [=](Vec2 pos, std::string spriteName){
        auto layerBG = Sprite::create(spriteName);
        layerBG->setCascadeOpacityEnabled(true);
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
    

    auto createNormalLayer = [=](std::string iconImg, std::string content, int score, Vec2 layerPos, int fontSize, bool addScore = true){
		if ((GLOBAL->TOTAL_SCORE + score < INT_MAX) && addScore)
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
    
    // combo reset
    CComboScore::Instance()->ComboScoreReset();
        
    // score layer array
    std::array<Node*, 6> scoreLayerArray;
    
	scoreLayerArray.at(0) = createNormalLayer(resultIcon[0],
                                              resultContent[0],
                                              GLOBAL->STAR_SCORE,
                                              posArray[0], 50);
    
	scoreLayerArray.at(1) = createNormalLayer(resultIcon[1],
                                              resultContent[1],
                                              GLOBAL->BEST_COMBO,
                                              posArray[1], 50);
    
    scoreLayerArray.at(2) = createMultipleLayer(resultIcon[2],
                                                resultContent[2],
                                                GLOBAL->COIN_SCORE,
                                                posArray[2], 50, 10);
    
	scoreLayerArray.at(3) = createMultipleLayer(resultIcon[3],
                                                resultContent[3],
                                                GLOBAL->NORMAL_ACHIEVEMENT_CLEAR_COUNT,
                                                posArray[3], 50, 100);
    
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
            CFacebookManager::Instance()->setSaveScoreListener([=](){
                auto oldRank = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::RANK);
                auto newRank = CFacebookManager::Instance()->getMyRank();
                if(oldRank != newRank){
                    CGameScene::getGameScene()->OpenRankUpPopup();
                    CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::RANK, newRank);
                }
            });
        }
    }
    
    auto totalScoreBG     = createLayerBG(posArray[4], "resultPopup_1.png");
    scoreLayerArray.at(4) = totalScoreBG;
    
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
    scoreLayerArray.at(5) = createNormalLayer(resultIcon[5],
                                              resultContent[5],
                                              bestScore,
                                              posArray[5], 50, false);
    
    // update coin
    CUserDataManager::Instance()->CoinUpdate(GLOBAL->COIN_SCORE);
    
    // get exp
    CUserDataManager::Instance()->ExpAdd(std::max(1, GLOBAL->TOTAL_SCORE / 100));
    
    // Check all of achievement.
    bool achievementAll    = CAchievementDataManager::Instance()->CheckCompleteAll();
    m_GoalPopupOpen = (GLOBAL->NORMAL_ACHIEVEMENT_CLEAR_COUNT || achievementAll);
    
    // create button lambda
    auto createButton = [=](const std::function<void(Node*)> &callback, std::string name, Vec2 pos, bool visible){
        auto button = CMyButton::create()
        ->addEventListener(callback)
        ->setButtonSingleUse(true)
        ->setButtonNormalImage(name)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(pos)
        ->show(this);
        button->setVisible(visible);
        return button;
    };
    
    std::array<Vec2, 7> btnPosArray = {
        Vec2(layerSize.width * 0.08f, layerSize.height * 0.05f),
        Vec2(layerSize.width * 0.92f, layerSize.height * 0.05f),
        Vec2(layerSize.width * 0.92f, layerSize.height * 0.05f),
        Vec2(layerSize.width * 0.25f, layerSize.height * 0.35f),
        Vec2(layerSize.width * 0.5f,  layerSize.height * 0.35f),
        Vec2(layerSize.width * 0.75f, layerSize.height * 0.35f),
        Vec2(layerSize.width * 0.1f,  layerSize.height * 0.2f ),
    };
    
    std::array<std::string, 7> btnIconArray = {
        "homeIcon.png",
        "resetIcon.png",
        "endIcon.png",
        "videoIcon.png",
        "characterIcon.png",
        "rewardIcon.png",
        "polaroidIcon.png"
    };
    
    std::array<std::function<void(Node*)>, 7> btnListenerArray = {
        [=](Node* sender) { this->GoHome(sender);           },
        [=](Node* sender) { this->Reset(sender);            },
        [=](Node* sender) { this->End(sender);              },
        [=](Node* sender) { this->GetCoinFromVideo(sender); },
        [=](Node* sender) { this->GetNewCharacter(sender);  },
        [=](Node* sender) { this->GetFreeReward(sender);    },
        [=](Node* sender) { this->Share(sender);            },
    };
    
    std::array<bool, 7> btnVisibleArray = {
        (!m_GoalPopupOpen),
        (!m_GoalPopupOpen),
        ( m_GoalPopupOpen),
        ( true ),
        ( CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::COIN) >= 1000 ),
        ( CFreeRewardManager::Instance()->getRewardAble() ),
        ( CObjectManager::Instance()->getPhotoShareAble() )
    };
    
    
    // create button array
    std::array<CMyButton*, 7> btnArray;
    
    // create buttons
    for(int i = 0; i < 7; i++)
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
    
    
//    // create captured picture
//    auto pictureBtn = CMyButton::create()
//    ->addEventListener([=](Node* sender){ this->Share(sender); })
//    ->setLayer(LayerColor::create(Color4B::WHITE, 100, 160))
//    ->setButtonPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 0.2f))
//    ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
//    ->show(this);
//    
//    auto picture    = CObjectManager::Instance()->Capture(90, 160, false);
//    picture->setPosition(Vec2(pictureBtn->getContentSize().width * 0.5f,
//                              pictureBtn->getContentSize().height * 0.47f));
//    picture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    pictureBtn->addChild(picture);
    
    
    this->setOpenAnimation([=](Node* sender){
        // clear all bullets
        CObjectManager::Instance()->ReturnToMemoryBlockAll();
        
        auto moveAction = MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f));
        auto easeAction = EaseExponentialInOut::create(moveAction);
        bg->runAction(easeAction);
        
        
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
//        action(pictureBtn);
    }, 1.2f);
    
	this->setCloseAnimation([=](Node* sender){

        for(auto btn : btnArray)
            btn->runAction(FadeTo::create(0.3f, 0));
        
		resultLabel->runAction(FadeTo::create(0.3f, 0));
		btnUserCoin->runAction(FadeTo::create(0.3f, 0));
//        pictureBtn->runAction(FadeTo::create(0.3f, 0));
        
        if( m_GoalPopupOpen ){
            auto action = [=](Node* sprite, Vec2 pos){
                auto move       = MoveTo::create(0.35f, pos);
                auto sine       = EaseSineIn::create(move);
                auto fadeout    = FadeTo::create(0.2f, 0);
                auto spawn      = Spawn::createWithTwoActions(sine, fadeout);
                
                sprite->runAction(spawn);
            };
            
            for(int index = 0; index < scoreLayerArray.size(); index++){
                action(scoreLayerArray.at(index), posDown.at(index));
            }
        }
        else {
            auto move = MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f,
                                                  layerSize.height * 1.5f));
            auto ease = EaseExponentialInOut::create(move);
            bg->runAction(ease);
        }
	});
   
    // if there are some reward from achievement. open reward popup after end of result popup.
    if ( m_GoalPopupOpen ) this->setDefaultCallback([=](Node* sender){ this->End(sender);    });
    else                   this->setDefaultCallback([=](Node* sender){ this->GoHome(sender); });
    
	return true;
}

void CResultPopup::Reset(Node* sender){
	CGameScene::getGameScene()->GameStart();
    this->exit();
}

void CResultPopup::GoHome(Node* sender){
	CGameScene::getGameScene()->OpenGameMenuLayer();
    this->exit();
}

void CResultPopup::End(Node* sender){
    CGameScene::getGameScene()->OpenGoalPopup();
    this->exit();
}

void CResultPopup::GetCoinFromVideo(cocos2d::Node* sender)
{
    CSDKUtil::Instance()->ShowRewardUnityAds([=](){
        this->createRewardPopup(ACHIEVEMENT_REWARD_KEY::REWARD_COIN_RANDOM, 50);
    });
}

void CResultPopup::GetNewCharacter(cocos2d::Node* sender)
{
    CGameScene::getGameScene()->Reward([=](bool isPlay){
        if(isPlay) this->popupClose();
    }, {}, -1500);
}

void CResultPopup::GetFreeReward(cocos2d::Node* sender)
{
    this->createRewardPopup(ACHIEVEMENT_REWARD_KEY::REWARD_COIN_RANDOM, 150);
    CFreeRewardManager::Instance()->setRewardAble(false);
}

void CResultPopup::createRewardPopup(std::string key, int value)
{
    CGameScene::getGameScene()->Reward(nullptr, {
        sREWARD_DATA(key, value)
    });
}

void CResultPopup::Share(cocos2d::Node* sender)
{
    CGameScene::getGameScene()->OpenSharePopup();
}

void CResultPopup::exit()
{
    if(m_GoalPopupOpen) this->popupClose();
    else                this->popupClose(1.3f);
}
