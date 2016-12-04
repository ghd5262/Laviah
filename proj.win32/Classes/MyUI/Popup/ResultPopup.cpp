#include "ResultPopup.h"
#include "../MyButton.h"
#include "../ScoreUI.h"
#include "../UIManager.h"
#include "../UserCoinButton.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../Common/StringUtility.h"
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
	int &totalScore = GLOBAL::TOTALSCORE;

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
		scoreLabel->setPosition(Vec2(parent->getContentSize().width * 0.9f, parent->getContentSize().height * 0.5f));
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

	auto createResultLayerWithNomalValue = [=](std::string iconImg, std::string content, int score, Vec2 pos){
		GLOBAL::TOTALSCORE += score;
		auto layerBG = createResultLayer(iconImg, content, score, pos);
		auto scoreLabel = createScoreLabel(layerBG, score);
		return layerBG;
	};

	auto createResultLayerWithMultipleValue = [=](std::string iconImg, std::string content, int score, Vec2 pos){
		GLOBAL::TOTALSCORE += (score * 10000);
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
	
	std::array<Vec2, 8> resultStartPosArray = {
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.5f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.4f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.3f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.2f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.1f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.0f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * -0.1f),
		Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * -0.2f)
	};

	auto moveScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("RunScoreUI"));
	auto moveDistanceBG = createResultLayerWithNomalValue("runIcon.png", "Run", moveScore->getScoreValue(), resultStartPosArray[0]);

	auto starScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("StarScoreUI"));
	auto starScoreBG = createResultLayerWithNomalValue("starIcon.png", "Star", starScore->getScoreValue(), resultStartPosArray[1]);
	
	auto coinScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("CoinScoreUI"));
	auto coinScoreBG = createResultLayerWithNomalValue("coinIcon.png", "Coin", coinScore->getScoreValue(), resultStartPosArray[2]);
	CUserDataManager::Instance()->CoinUpdate(coinScore->getScoreValue());

	auto bonusTimeBG = createResultLayerWithMultipleValue("bonustimeIcon.png", "BonusTime", GLOBAL::BONUSTIME, resultStartPosArray[3]);
	auto alienBG = createResultLayerWithMultipleValue("alienIcon.png", "AlienGet", GLOBAL::ALIENGET, resultStartPosArray[4]);
	auto challengeBG = createResultLayerWithMultipleValue("challengeIcon.png", "ChallengeClear", GLOBAL::CHALLENGECLEAR, resultStartPosArray[5]);

	auto bestScore = CUserDataManager::Instance()->getUserData_Number("USER_BEST_TOTAL_SCORE");
	auto bestScoreBG = createResultLayer("bestScoreIcon.png", "Best Score", bestScore, resultStartPosArray[7]);
	auto scoreLabel = createScoreLabel(bestScoreBG, bestScore);


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

		Vec2 posArray[] = {
			Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.7f),
			Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.65f),
			Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.6f),
			Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.55f),
			Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.5f),
			Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.45f),
			Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.4f),
			Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.35f)
		};

		action(moveDistanceBG, posArray[0]);
		action(starScoreBG, posArray[1]);
		action(coinScoreBG, posArray[2]);
		action(bonusTimeBG, posArray[3]);
		action(alienBG, posArray[4]);
		action(challengeBG, posArray[5]);
		action(totalScoreBG, posArray[6]);
		action(bestScoreBG, posArray[7]);

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

		action(moveDistanceBG,	resultStartPosArray[0]);
		action(starScoreBG,		resultStartPosArray[1]);
		action(coinScoreBG,		resultStartPosArray[2]);
		action(bonusTimeBG,		resultStartPosArray[3]);
		action(alienBG,			resultStartPosArray[4]);
		action(challengeBG,		resultStartPosArray[5]);
		action(totalScoreBG,	resultStartPosArray[6]);
		action(bestScoreBG,		resultStartPosArray[7]);

		resultLabel->runAction(FadeTo::create(0.5f, 0));
		btnHome->runAction(FadeTo::create(0.5f, 0));
		btnReset->runAction(FadeTo::create(0.5f, 0));
		btnUserCoin->runAction(FadeTo::create(0.5f, 0));
	});

	return true;
}

void CResultPopup::Reset(Node* sender){
	CCLOG("format popup Replay");
	CGameScene::getGameScene()->ResetGameScene();
}

void CResultPopup::GoHome(Node* sender){
	CCLOG("format popup GoHome");
	CGameScene::getGameScene()->BackToMenuScene();
	this->popupClose();
}