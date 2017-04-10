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

	auto bg = LayerColor::create(COLOR::TRANSPARENT_ALPHA, 1080.f, 1920.f);
	if (bg != nullptr){
		bg->setIgnoreAnchorPointForPosition(false);
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(this->getContentSize() / 2);
		this->addChild(bg);
	}

    std::array<Vec2, 8> startPos = {
//        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.5f),
//        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.4f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.3f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.2f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.1f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.0f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * -0.1f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * -0.2f)
    };
    
    std::array<Vec2, 8> targetPos = {
//        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.7f),
//        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.65f),
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
        layerBG->setOpacity(0);
        layerBG->setCascadeOpacityEnabled(true);
        bg->addChild(layerBG);
        
        return layerBG;
    };
    
    auto createIcon = [=](Node* parent, Vec2 pos, std::string iconImg){
        auto icon = Sprite::create(iconImg);
        icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        icon->setPosition(pos);
        icon->setColor(COLOR::DARKGRAY);
        parent->addChild(icon);
        
        return icon;
    };
    
    auto createContent = [=](Node* parent, Vec2 pos, std::string content, int fontSize){
        auto contentLabel = Label::createWithSystemFont(content, FONT::MALGUNBD, fontSize);
        contentLabel->setColor(COLOR::DARKGRAY);
        contentLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        contentLabel->setPosition(pos);
        parent->addChild(contentLabel);
        
        return contentLabel;
    };
    
    auto createScoreLabel = [=](Node* parent, Vec2 pos, int score, int fontSize){
        auto scoreLabel = Label::createWithTTF(StringUtility::toCommaString(score), FONT::MALGUNBD, fontSize);
        scoreLabel->setColor(COLOR::DARKGRAY);
        scoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        scoreLabel->setPosition(pos);
        parent->addChild(scoreLabel);
        
        return scoreLabel;
    };
    
    auto createMultipleLabel = [=](Node* parent, Vec2 pos, int multiple){
		auto multipleLabel = Label::createWithTTF(StringUtils::format("%d x ", multiple), FONT::MALGUNBD, 25);
        multipleLabel->setColor(COLOR::DARKGRAY);
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
	
    auto createButton = [=](const std::function<void(Node*)> &callback, std::string name, Vec2 pos){
        auto button = CMyButton::create()
        ->addEventListener(callback)
        ->setButtonSingleUse(true)
        ->setButtonNormalImage(name)
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
        ->setButtonPosition(pos)
        ->show(bg);
        button->setOpacity(0);
        
        return button;
    };
    
	bool challengeAll = CChallengeDataManager::Instance()->CheckCompleteAll();

	auto starScoreBG	= createNormalLayer(resultIcon[0], resultContent[0], GLOBAL->STAR_SCORE, startPos[0], 50);
//	auto moveDistanceBG = createNormalLayer(resultIcon[1], resultContent[1], GLOBAL->RUN_SCORE, startPos[1], 50);
	auto comboBG		= createNormalLayer(resultIcon[1], resultContent[1], GLOBAL->BEST_COMBO, startPos[1], 50);
    auto coinScoreBG    = createMultipleLayer(resultIcon[2], resultContent[2], GLOBAL->COIN_SCORE,  startPos[2], 50, 10);
//    auto bonusTimeBG	= createMultipleLayer(resultIcon[4], resultContent[4], GLOBAL->BONUSTIME, startPos[4], 50, 10000);
	auto challengeBG	= createMultipleLayer(resultIcon[3], resultContent[3], GLOBAL->NORMAL_CHALLENGE_CLEAR_COUNT, startPos[3], 50, 100);
    
    auto totalScoreBG = createLayerBG(startPos[4]);
    totalScoreBG->setTexture("resultPopup_1.png");
    
	auto bestScore = CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BEST_SCORE);
    std::string totalContent = TRANSLATE("RESULT_TOTAL_SCORE");
    
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
    
    // get exp
    {
        CUserDataManager::Instance()->ExpAdd(std::max(1, GLOBAL->TOTAL_SCORE / 100));
//        MessageBox(StringUtils::format("Get exp : %d, total : exp %d, level : %d",
//                                       std::max(1, GLOBAL->TOTAL_SCORE / 100),
//                                       CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::EXP),
//                                       CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::LEVEL)).c_str(), "NOTICE");
    }
    
    auto totalLabel = createContent(totalScoreBG,
                                    Vec2(totalScoreBG->getContentSize().width * 0.08f,
                                         totalScoreBG->getContentSize().height * 0.5f),
                                    totalContent,
                                    60);
    totalLabel->setColor(COLOR::BRIGHTGRAY);
    
    auto totalScoreLabel = createScoreLabel(totalScoreBG,
                                            Vec2(totalScoreBG->getContentSize().width * 0.9f,
                                                 totalScoreBG->getContentSize().height * 0.5f),
                                            GLOBAL->TOTAL_SCORE,
                                            60);
    totalScoreLabel->setColor(COLOR::BRIGHTGRAY);
    
    
    auto bestScoreBG = createNormalLayer(resultIcon[5], resultContent[5], bestScore,  startPos[5], 50);
    CUserDataManager::Instance()->CoinUpdate(GLOBAL->COIN_SCORE);
    
    
    
    /* result label*/
    auto resultLabel = Label::createWithSystemFont(TRANSLATE("RESULT_POPUP_TITLE"), FONT::MALGUNBD, 80);
    if (resultLabel != nullptr)
    {
        resultLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        resultLabel->setPosition(Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.8f));
        resultLabel->setColor(COLOR::DARKGRAY);
        bg->addChild(resultLabel);
        resultLabel->setOpacity(0);
    }
    
    auto btnHome = createButton([=](Node* sender) { this->GoHome(sender); },
                                "homeIcon.png",
                                Vec2(bg->getContentSize().width * 0.08f,
                                     bg->getContentSize().height * 0.05f));

    auto btnReset = createButton([=](Node* sender) { this->Reset(sender); },
                                 "resetIcon.png",
                                 Vec2(bg->getContentSize().width * 0.92f,
                                      bg->getContentSize().height * 0.05f));
    
    auto btnEnd = createButton([=](Node* sender) { this->End(sender); },
                               "endIcon.png",
                               Vec2(bg->getContentSize().width * 0.92f,
                                    bg->getContentSize().height * 0.05f));
    
	auto btnUserCoin = CUserCoinButton::create();
	if (btnUserCoin != nullptr)
	{
		btnUserCoin->setPosition(Vec2(bg->getContentSize().width * 0.5f,
			bg->getContentSize().height * 0.05f));
		btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->addChild(btnUserCoin);
		btnUserCoin->setCascadeOpacityEnabled(true);
		btnUserCoin->setOpacity(0);
	}

	if (GLOBAL->NORMAL_CHALLENGE_CLEAR_COUNT || challengeAll)
    {
        btnHome->setVisible(false);
        btnReset->setVisible(false);
        btnEnd->setVisible(true);
		this->setDefaultCallback([=](Node* sender){
			this->End(sender);
		});
    }
    else{
        btnHome->setVisible(true);
        btnReset->setVisible(true);
        btnEnd->setVisible(false);
		this->setDefaultCallback([=](Node* sender){
			this->GoHome(sender);
		});
    }
        
	this->setOpenAnimation([=](Node* sender){
		auto winSize = Director::getInstance()->getWinSize();

        auto action = [=](Sprite* sprite, Vec2 pos){
            auto move        = MoveTo::create(1.3f, pos);
            auto exponential = EaseExponentialOut::create(move);
            auto fadein      = FadeIn::create(1.f);
            auto spawn       = Spawn::createWithTwoActions(exponential, fadein);
            
            sprite->runAction(spawn);
        };
		action(starScoreBG,		targetPos[0]);
//		action(moveDistanceBG,  targetPos[1]);
		action(comboBG,			targetPos[1]);
        action(coinScoreBG,     targetPos[2]);
//        action(bonusTimeBG,		targetPos[4]);
		action(challengeBG,     targetPos[3]);
		action(totalScoreBG,    targetPos[4]);
		action(bestScoreBG,     targetPos[5]);

		resultLabel->runAction(FadeIn::create(0.5f));
		btnHome->runAction(FadeIn::create(0.5f));
		btnReset->runAction(FadeIn::create(0.5f));
        btnEnd->runAction(FadeIn::create(0.5f));
		btnUserCoin->runAction(FadeIn::create(0.5f));
	});

	this->setCloseAnimation([=](Node* sender){

        auto action = [=](Sprite* sprite, Vec2 pos){
            auto move       = MoveTo::create(0.35f, pos);
            auto sine       = EaseSineIn::create(move);
            auto fadeout    = FadeTo::create(0.2f, 0);
            auto spawn      = Spawn::createWithTwoActions(sine, fadeout);
            
            sprite->runAction(spawn);
        };

		action(starScoreBG,		startPos[0]);
//		action(moveDistanceBG,	startPos[1]);
		action(comboBG,			startPos[1]);
        action(coinScoreBG,		startPos[2]);
//        action(bonusTimeBG,		startPos[4]);
		action(challengeBG,		startPos[3]);
		action(totalScoreBG,	startPos[4]);
		action(bestScoreBG,		startPos[5]);

		resultLabel->runAction(FadeTo::create(0.5f, 0));
		btnHome->runAction(FadeTo::create(0.5f, 0));
		btnReset->runAction(FadeTo::create(0.5f, 0));
        btnEnd->runAction(FadeTo::create(0.5f, 0));
		btnUserCoin->runAction(FadeTo::create(0.5f, 0));
	});
   
	return true;
}

void CResultPopup::Reset(Node* sender){
	CGameScene::getGameScene()->GameStart();
    this->popupClose();
}

void CResultPopup::GoHome(Node* sender){
	CGameScene::getGameScene()->OpenGameMenuLayer();
	this->popupClose();
}

void CResultPopup::End(Node* sender){
    CGameScene::getGameScene()->ShowChallenge();
    this->popupClose();
}