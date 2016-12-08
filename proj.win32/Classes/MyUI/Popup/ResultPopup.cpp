#include "ResultPopup.h"
#include "../MyButton.h"
#include "../ScoreUI.h"
#include "../UserCoinButton.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../Common/StringUtility.h"
#include "../../Scene/GameScene.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Player.h"
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

	// 총 점수 = 달린 총거리 + 별 + 코인 + (보너스타임횟수 * 10000) + (외계주민 * 10000) + (도전과제 * 10000)
	/* result label*/
	auto resultLabel = Label::createWithTTF("Result", "fonts/malgunbd.ttf", 80);
	if (resultLabel != nullptr)
	{
		resultLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		resultLabel->setPosition(Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.8f));
		resultLabel->setColor(COLOR::BRIGHTGRAY);
		bg->addChild(resultLabel);
		resultLabel->setOpacity(0);
	}

	auto createScoreLabel = [=](Node* parent, int score){
		auto scoreLabel = Label::createWithTTF(StringUtility::toCommaString(score), "fonts/malgunbd.ttf", 50);
		scoreLabel->setColor(COLOR::DARKGRAY);
		scoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		scoreLabel->setPosition(Vec2(parent->getContentSize().width * 0.9f,
                                     parent->getContentSize().height * 0.5f));
		parent->addChild(scoreLabel);

		return scoreLabel;
	};

	auto createResultLayer = [=](std::string iconImg, std::string content, int score, Vec2 pos){
		auto layerBG = Sprite::create("resultPopup_2.png");
		layerBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		layerBG->setPosition(pos);
		layerBG->setOpacity(0);
		layerBG->setCascadeOpacityEnabled(true);
		bg->addChild(layerBG);

		auto icon = Sprite::create(iconImg);
		icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		icon->setPosition(Vec2(layerBG->getContentSize().width * 0.1f, layerBG->getContentSize().height * 0.5f));
		icon->setColor(COLOR::DARKGRAY);
		layerBG->addChild(icon);

		auto contentLabel = Label::createWithTTF(content, "fonts/malgunbd.ttf", 50);
		contentLabel->setColor(COLOR::DARKGRAY);
		contentLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		contentLabel->setPosition(Vec2(layerBG->getContentSize().width * 0.15f, layerBG->getContentSize().height * 0.5f));
		layerBG->addChild(contentLabel);

		return layerBG;
	};

    auto createNormalLayer = [=](std::string iconImg, std::string content, std::string key, Vec2 pos){
        auto score = CGameScene::getGameScene()->getGlobalValue(key);
        CGameScene::getGameScene()->addGlobalValue(GLOBAL::TOTALSCORE, score);

		auto layerBG = createResultLayer(iconImg, content, score, pos);
		createScoreLabel(layerBG, score);
		return layerBG;
	};

	auto createMultipleLayer = [=](std::string iconImg, std::string content, std::string key, Vec2 pos){
        auto score = CGameScene::getGameScene()->getGlobalValue(key);
        CGameScene::getGameScene()->addGlobalValue(GLOBAL::TOTALSCORE, score * 10000);
        
        auto layerBG = createResultLayer(iconImg, content, score, pos);
		auto scoreLabel = createScoreLabel(layerBG, score);
		auto multipleScoreLabel = Label::createWithTTF(StringUtils::format("%d x ", 10000), "fonts/malgunbd.ttf", 25);
		if (multipleScoreLabel != nullptr){
			multipleScoreLabel->setColor(COLOR::DARKGRAY);
			multipleScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
			multipleScoreLabel->setPosition(
				Vec2(scoreLabel->getPosition().x - scoreLabel->getContentSize().width,
				layerBG->getContentSize().height * 0.4f));
			layerBG->addChild(multipleScoreLabel);
		}

		return layerBG;
	};
	
	std::array<Vec2, 8> startPosArray = {
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.5f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.4f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.3f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.2f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.1f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.0f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * -0.1f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * -0.2f)
	};
    
    std::array<Vec2, 8> targetPosArray = {
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.7f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.65f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.6f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.55f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.5f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.45f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.4f),
        Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.35f)
    };
    
    auto moveDistanceBG = createNormalLayer("runIcon.png", "Run", GLOBAL::RUNSCORE,  startPosArray[0]);
	auto starScoreBG = createNormalLayer("starIcon.png", "Star", GLOBAL::STARSCORE, startPosArray[1]);
	auto coinScoreBG = createNormalLayer("coinIcon.png", "Coin", GLOBAL::COINSCORE, startPosArray[2]);
	auto bonusTimeBG = createMultipleLayer("bonustimeIcon.png", "BonusTime", GLOBAL::BONUSTIME, startPosArray[3]);
	auto alienBG     = createMultipleLayer("alienIcon.png", "AlienGet", GLOBAL::ALIENGET, startPosArray[4]);
	auto challengeBG = createMultipleLayer("challengeIcon.png", "ChallengeClear", GLOBAL::CHALLENGECLEAR, startPosArray[5]);
	auto bestScore = CUserDataManager::Instance()->getUserData_Number("USER_BEST_TOTAL_SCORE");
	auto bestScoreBG = createResultLayer("bestScoreIcon.png", "Best Score", bestScore, startPosArray[7]);
    createScoreLabel(bestScoreBG, bestScore);
    
    auto coinScore = CGameScene::getGameScene()->getGlobalValue(GLOBAL::COINSCORE);
    auto totalScore = CGameScene::getGameScene()->getGlobalValue(GLOBAL::TOTALSCORE);
    CUserDataManager::Instance()->CoinUpdate(coinScore);
    
	/* total score */
	auto totalScoreBG = Sprite::create("resultPopup_1.png");
	if (totalScoreBG != nullptr){
		totalScoreBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		totalScoreBG->setPosition(Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * -0.1f));
		bg->addChild(totalScoreBG);
		totalScoreBG->setOpacity(0);
		totalScoreBG->setCascadeOpacityEnabled(true);

		std::string bestScoreStr = "Total Score";

		// total score가 best score면 저장한다.
		if (totalScore > CUserDataManager::Instance()->getUserData_Number("USER_BEST_TOTAL_SCORE")){
			bestScoreStr = "Best Score";
			CUserDataManager::Instance()->setUserData_Number("USER_BEST_TOTAL_SCORE", totalScore);
		}

		auto totalScoreLabel = Label::createWithTTF(bestScoreStr.c_str(), "fonts/malgunbd.ttf", 60);
		if (totalScoreLabel != nullptr){
			totalScoreLabel->setColor(COLOR::BRIGHTGRAY);
			totalScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			totalScoreLabel->setPosition(Vec2(totalScoreBG->getContentSize().width * 0.08f, totalScoreBG->getContentSize().height * 0.5f));
			totalScoreBG->addChild(totalScoreLabel);
		}

		auto totalScoreValueLabel = Label::createWithTTF(StringUtility::toCommaString(totalScore), "fonts/malgunbd.ttf", 60);
		if (totalScoreValueLabel != nullptr){
			totalScoreValueLabel->setColor(COLOR::BRIGHTGRAY);
			totalScoreValueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
			totalScoreValueLabel->setPosition(Vec2(totalScoreBG->getContentSize().width * 0.9f, totalScoreBG->getContentSize().height * 0.5f));
			totalScoreBG->addChild(totalScoreValueLabel);
		}
	}

	auto createButton = [=](const std::function<void(Node*)> &callback, std::string name, Vec2 pos){
		auto button = CMyButton::create()
			->addEventListener(callback)
			->setButtonNormalImage(name)
			->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->setButtonPosition(pos)
			->show(bg);
		button->setOpacity(0);

		return button;
	};

	auto btnHome = createButton([=](Node* sender)
	{
		this->GoHome(sender);
	}, "homeIcon.png",
		Vec2(bg->getContentSize().width * 0.08f, bg->getContentSize().height * 0.05f));

	auto btnReset = createButton([=](Node* sender)
	{
		this->Reset(sender);
	}, "resetIcon.png",
		Vec2(bg->getContentSize().width * 0.92f, bg->getContentSize().height * 0.05f));

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

	this->setOpenAnimation([=](Node* sender){
		auto winSize = Director::getInstance()->getWinSize();

		auto action = [=](Sprite* sprite, Vec2 pos){
			sprite->runAction(
				Spawn::createWithTwoActions(
				EaseExponentialOut::create(
				MoveTo::create(1.3f, pos)),
				FadeIn::create(1.f)));
		};

		action(moveDistanceBG,  targetPosArray[0]);
		action(starScoreBG,     targetPosArray[1]);
		action(coinScoreBG,     targetPosArray[2]);
		action(bonusTimeBG,     targetPosArray[3]);
		action(alienBG,         targetPosArray[4]);
		action(challengeBG,     targetPosArray[5]);
		action(totalScoreBG,    targetPosArray[6]);
		action(bestScoreBG,     targetPosArray[7]);

		resultLabel->runAction(FadeIn::create(0.5f));
		btnHome->runAction(FadeIn::create(0.5f));
		btnReset->runAction(FadeIn::create(0.5f));
		btnUserCoin->runAction(FadeIn::create(0.5f));
	});

	this->setCloseAnimation([=](Node* sender){

		auto action = [=](Sprite* sprite, Vec2 pos){
			sprite->runAction(
				Spawn::createWithTwoActions(
				EaseSineIn::create(
				MoveTo::create(0.35f, pos)),
				FadeTo::create(0.2f, 0)));
		};

		action(moveDistanceBG,	startPosArray[0]);
		action(starScoreBG,		startPosArray[1]);
		action(coinScoreBG,		startPosArray[2]);
		action(bonusTimeBG,		startPosArray[3]);
		action(alienBG,			startPosArray[4]);
		action(challengeBG,		startPosArray[5]);
		action(totalScoreBG,	startPosArray[6]);
		action(bestScoreBG,		startPosArray[7]);

		resultLabel->runAction(FadeTo::create(0.5f, 0));
		btnHome->runAction(FadeTo::create(0.5f, 0));
		btnReset->runAction(FadeTo::create(0.5f, 0));
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