#include "ResultPopup.h"
#include "../MyButton.h"
#include "../ScoreUI.h"
#include "../UserCoinButton.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../DataManager/ChallengeDataManager.hpp"
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

    auto layerSize = this->getContentSize();
    
	auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
	if (bg != nullptr){
		bg->setIgnoreAnchorPointForPosition(false);
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(Vec2(layerSize.width * 0.5f, layerSize.height * 1.5f));
		this->addChild(bg);
	}

    std::array<Vec2, 8> posArray = {
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.7f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.65f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.6f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.55f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.5f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.45f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.4f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.35f)
    };
    
    std::array<std::string, 8> resultIcon = {
		"starIcon.png",
//        "runIcon.png",
		"comboIcon.png",
        "coinIcon.png",
//        "bonustimeIcon.png",
        "challengeIcon_2.png",
        "",
        "bestScoreIcon.png"
    };
    
    std::array<std::string, 8> resultContent = {
		TRANSLATE("RESULT_SCORE"),
//        TRANSLATE("RESULT_RUN"),
		TRANSLATE("RESULT_COMBO"),
        TRANSLATE("RESULT_COIN"),
//        TRANSLATE("RESULT_BONUSTIME"),
        TRANSLATE("RESULT_CHALLENGE"),
        "",
        TRANSLATE("RESULT_BEST_SCORE")
    };
    
    
    // 총 점수 = 달린 총거리 + 별 + 코인 + (보너스타임횟수 * 10000) + (외계주민 * 10000) + (도전과제 * 10000)
    
    auto createLayerBG = [=](Vec2 pos){
        auto layerBG = Sprite::create("resultPopup_2.png");
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

		auto layerBG = createLayerBG(layerPos);
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

        auto layerBG = createLayerBG(layerPos);
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
    
	bool challengeAll = CChallengeDataManager::Instance()->CheckCompleteAll();

	createNormalLayer(resultIcon[0],    resultContent[0], GLOBAL->STAR_SCORE,   posArray[0], 50);
	createNormalLayer(resultIcon[1],    resultContent[1], GLOBAL->BEST_COMBO,   posArray[1], 50);
    createMultipleLayer(resultIcon[2],  resultContent[2], GLOBAL->COIN_SCORE,   posArray[2], 50, 10);
	createMultipleLayer(resultIcon[3],  resultContent[3], GLOBAL->NORMAL_CHALLENGE_CLEAR_COUNT, posArray[3], 50, 100);
    
    auto totalScoreBG = createLayerBG(posArray[4]);
    totalScoreBG->setTexture("resultPopup_1.png");
    
	auto bestScore = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BEST_SCORE);
    std::string totalContent = TRANSLATE("RESULT_TOTAL_SCORE");
    
    // combo가 user best combo면 저장한다.
	auto bestCombo = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BEST_COMBO);
	if (GLOBAL->BEST_COMBO > bestCombo)
	{
		CUserDataManager::Instance()->setUserData_Number(USERDATA_KEY::BEST_COMBO, GLOBAL->BEST_COMBO);
	}

    // total score가 best score면 저장한다.
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
    
    // create result label
    auto resultLabel = Label::createWithSystemFont(TRANSLATE("RESULT_POPUP_TITLE"), FONT::MALGUNBD, 80);
    if (resultLabel != nullptr)
    {
        resultLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        resultLabel->setPosition(Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.8f));
        this->addChild(resultLabel);
    }
    
    // create button lambda
    auto createButton = [=](const std::function<void(Node*)> &callback, std::string name, Vec2 pos){
        auto button = CMyButton::create()
        ->addEventListener(callback)
        ->setButtonSingleUse(true)
        ->setButtonNormalImage(name)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(pos)
        ->show(this);
        button->setTouchEnable(false);
        button->setVisible(false);
        return button;
    };
    
    auto btnHome    = createButton([=](Node* sender) { this->GoHome(sender); }, "homeIcon.png",
                                   Vec2(bg->getContentSize().width * 0.08f, bg->getContentSize().height * 0.05f));
    
    auto btnReset   = createButton([=](Node* sender) { this->Reset(sender);  }, "resetIcon.png",
                                   Vec2(bg->getContentSize().width * 0.92f, bg->getContentSize().height * 0.05f));
    
    auto btnEnd     = createButton([=](Node* sender) { this->End(sender);    }, "endIcon.png",
                                   Vec2(bg->getContentSize().width * 0.92f, bg->getContentSize().height * 0.05f));
    
    // create user coin layer
    auto btnUserCoin = CUserCoinButton::create();
    btnUserCoin->setPosition(Vec2(bg->getContentSize().width * 0.5f,
                                  bg->getContentSize().height * 0.05f));
    btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(btnUserCoin);
    
    
    bool rewardPopupOpen = (GLOBAL->NORMAL_CHALLENGE_CLEAR_COUNT || challengeAll);
    
    this->setOpenAnimation([=](Node* sender){
        // clear all bullets
        CObjectManager::Instance()->ReturnToMemoryBlockAll();
        
        auto moveAction = MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f, layerSize.height * 0.5f));
        auto easeAction = EaseExponentialInOut::create(moveAction);
        auto callFunc   = CallFunc::create([=](){
            
            // if there are some reward from challenge. open reward popup after end of result popup.
            if ( rewardPopupOpen )
            {
                this->changeDefaultCallback([=](Node* sender){ this->End(sender); });
                btnEnd->setVisible(true);
                btnEnd->setTouchEnable(true);
            }
            else{
                this->changeDefaultCallback([=](Node* sender){ this->GoHome(sender); });
                btnHome->setVisible(true);
                btnReset->setVisible(true);
                btnHome->setTouchEnable(true);
                btnReset->setTouchEnable(true);
            }
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
        
        action(resultLabel);
        action(btnHome);
        action(btnReset);
        action(btnEnd);
        action(btnUserCoin);
    });
    
	this->setCloseAnimation([=](Node* sender){

		resultLabel->runAction(FadeTo::create(0.3f, 0));
		btnHome->runAction(FadeTo::create(0.3f, 0));
		btnReset->runAction(FadeTo::create(0.3f, 0));
        btnEnd->runAction(FadeTo::create(0.3f, 0));
		btnUserCoin->runAction(FadeTo::create(0.3f, 0));
        
        if( rewardPopupOpen )
            bg->runAction(FadeTo::create(0.3f, 0));
        else
            bg->runAction(EaseExponentialInOut::create(MoveTo::create(1.2f, Vec2(layerSize.width * 0.5f,
                                                                                 layerSize.height * 1.5f))));
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