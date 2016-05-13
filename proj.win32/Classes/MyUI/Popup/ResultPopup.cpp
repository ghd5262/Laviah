#include "ResultPopup.h"
#include "../MyButton.h"
#include "../ScoreUI.h"
#include "../UIManager.h"
#include "../../Scene/GameScene.h"

CResultPopup* CResultPopup::create()
{
	CResultPopup *pRet = new(std::nothrow) CResultPopup();
	if (pRet)
	{
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool CResultPopup::initVariable()
{
	try{
		m_BG = Sprite::create("empty1280.png");
		m_BG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_BG->setPosition(Vec2::ZERO);
		m_Popup->addChild(m_BG);

		//        auto noticeLabel = Label::createWithTTF("format popup", "fonts/Marker Felt.ttf", 25);
		//        noticeLabel->setColor(Color3B::BLACK);
		//        noticeLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		//        noticeLabel->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.5f));
		//        m_BG->addChild(noticeLabel);

		//auto moveDistance = Sprite::create("")

		/* result label*/
		auto resultLabel = Label::create("Result", "fonts/malgunbd.ttf", 50);
		if (resultLabel != nullptr)
		{
			resultLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			resultLabel->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.8f));
			
			m_BG->addChild(resultLabel);
			resultLabel->setOpacity(0);
		}

		/* 이동거리 */
		auto moveDistanceBG = Sprite::create("resultPopup_1.png");
		if (moveDistanceBG != nullptr){
			moveDistanceBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			moveDistanceBG->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.5f));
			m_BG->addChild(moveDistanceBG);
			moveDistanceBG->setOpacity(0);
			moveDistanceBG->setCascadeOpacityEnabled(true);
			auto moveDistanceIcon = Sprite::create("runIcon.png");
			if (moveDistanceIcon != nullptr)
			{
				moveDistanceIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				moveDistanceIcon->setPosition(Vec2(moveDistanceBG->getContentSize().width * 0.1f, moveDistanceBG->getContentSize().height * 0.5f));
				moveDistanceBG->addChild(moveDistanceIcon);
			}

			auto moveDistanceLabel = Label::create("Run", "fonts/malgunbd.ttf", 30);
			if (moveDistanceLabel != nullptr){
				moveDistanceLabel->setColor(Color3B(0, 0, 14));
				moveDistanceLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
				moveDistanceLabel->setPosition(Vec2(moveDistanceBG->getContentSize().width * 0.15f, moveDistanceBG->getContentSize().height * 0.5f));
				moveDistanceBG->addChild(moveDistanceLabel);
			}

			auto runScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("RunScoreUI"));

			auto moveDistanceValueLabel = Label::create(StringUtils::format("%d m", runScore->getScoreValue()), "fonts/malgunbd.ttf", 30);
			if (moveDistanceValueLabel != nullptr){
				moveDistanceValueLabel->setColor(Color3B(0, 0, 14));
				moveDistanceValueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
				moveDistanceValueLabel->setPosition(Vec2(moveDistanceBG->getContentSize().width * 0.9f, moveDistanceBG->getContentSize().height * 0.5f));
				moveDistanceBG->addChild(moveDistanceValueLabel);
			}
		}


		/* 별 점수 */
		auto starScoreBG = Sprite::create("resultPopup_2.png");
		if (starScoreBG != nullptr){
			starScoreBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			starScoreBG->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.4f));
			m_BG->addChild(starScoreBG);
			starScoreBG->setOpacity(0);
			starScoreBG->setCascadeOpacityEnabled(true);
			auto starScoreIcon = Sprite::create("starIcon.png");
			if (starScoreIcon != nullptr)
			{
				starScoreIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				starScoreIcon->setPosition(Vec2(starScoreBG->getContentSize().width * 0.1f, starScoreBG->getContentSize().height * 0.5f));
				starScoreBG->addChild(starScoreIcon);
			}

			auto starScoreLabel = Label::create("Star", "fonts/malgunbd.ttf", 30);
			if (starScoreLabel != nullptr){
				starScoreLabel->setColor(Color3B(0, 0, 14));
				starScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
				starScoreLabel->setPosition(Vec2(starScoreBG->getContentSize().width * 0.15f, starScoreBG->getContentSize().height * 0.5f));
				starScoreBG->addChild(starScoreLabel);
			}

			auto starScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("StarScoreUI"));

			auto starScoreValueLabel = Label::create(StringUtils::format("%d", starScore->getScoreValue()), "fonts/malgunbd.ttf", 30);
			if (starScoreValueLabel != nullptr){
				starScoreValueLabel->setColor(Color3B(0, 0, 14));
				starScoreValueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
				starScoreValueLabel->setPosition(Vec2(starScoreBG->getContentSize().width * 0.9f, starScoreBG->getContentSize().height * 0.5f));
				starScoreBG->addChild(starScoreValueLabel);
			}
		}

		/* 코인 점수 */
		auto coinScoreBG = Sprite::create("resultPopup_1.png");
		if (coinScoreBG != nullptr){
			coinScoreBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			coinScoreBG->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.3f));
			m_BG->addChild(coinScoreBG);
			coinScoreBG->setOpacity(0);
			coinScoreBG->setCascadeOpacityEnabled(true);
			auto coinScoreIcon = Sprite::create("coinIcon.png");
			if (coinScoreIcon != nullptr)
			{
				coinScoreIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				coinScoreIcon->setPosition(Vec2(coinScoreBG->getContentSize().width * 0.1f, coinScoreBG->getContentSize().height * 0.5f));
				coinScoreBG->addChild(coinScoreIcon);
			}

			auto coinScoreLabel = Label::create("Coin", "fonts/malgunbd.ttf", 30);
			if (coinScoreLabel != nullptr){
				coinScoreLabel->setColor(Color3B(0, 0, 14));
				coinScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
				coinScoreLabel->setPosition(Vec2(coinScoreBG->getContentSize().width * 0.15f, coinScoreBG->getContentSize().height * 0.5f));
				coinScoreBG->addChild(coinScoreLabel);
			}

			auto coinScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("CoinScoreUI"));

			auto coinScoreValueLabel = Label::create(StringUtils::format("%d", coinScore->getScoreValue()), "fonts/malgunbd.ttf", 30);
			if (coinScoreValueLabel != nullptr){
				coinScoreValueLabel->setColor(Color3B(0, 0, 14));
				coinScoreValueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
				coinScoreValueLabel->setPosition(Vec2(coinScoreBG->getContentSize().width * 0.9f, coinScoreBG->getContentSize().height * 0.5f));
				coinScoreBG->addChild(coinScoreValueLabel);
			}
		}

		m_btnHome = CMyButton::create("homeIcon.png",
			END,
			std::bind(&CResultPopup::GoHome, this),
			EFFECT_ALPHA);

		if (m_btnHome != nullptr)
		{
			m_btnHome->setPosition(Vec2(m_BG->getContentSize().width * 0.08f,
				m_BG->getContentSize().height * 0.05f));
			m_btnHome->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_BG->addChild(m_btnHome);
			m_btnHome->setCascadeOpacityEnabled(true);
			m_btnHome->setOpacity(0);
		}

		m_btnReset = CMyButton::create("resetIcon.png",
			END,
			std::bind(&CResultPopup::Reset, this),
			EFFECT_ALPHA);

		if (m_btnReset != nullptr)
		{
			m_btnReset->setPosition(Vec2(m_BG->getContentSize().width * 0.92f,
				m_BG->getContentSize().height * 0.05f));
			m_btnReset->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_BG->addChild(m_btnReset);
			m_btnReset->setCascadeOpacityEnabled(true);
			m_btnReset->setOpacity(0);
		}

		m_Popup->setPopupOpenEffectFunc([=](CPopup* pausePopup){
			auto winSize = Director::getInstance()->getWinSize();

			moveDistanceBG->runAction(
				Spawn::createWithTwoActions(
				EaseExponentialOut::create(
				MoveTo::create(1.3f, Vec2(m_BG->getContentSize().width * 0.5f,
				m_BG->getContentSize().height * 0.7f))), 
				FadeIn::create(1.f)));
			starScoreBG->runAction(
				Spawn::createWithTwoActions(
				EaseExponentialOut::create(
				MoveTo::create(1.3f, Vec2(m_BG->getContentSize().width * 0.5f,
				m_BG->getContentSize().height * 0.65f))),
				FadeIn::create(1.f)));
			coinScoreBG->runAction(
				Spawn::createWithTwoActions(
				EaseExponentialOut::create(
				MoveTo::create(1.3f, Vec2(m_BG->getContentSize().width * 0.5f,
				m_BG->getContentSize().height * 0.6f))),
				FadeIn::create(1.f)));

			resultLabel->runAction(FadeIn::create(0.5f));

			m_btnHome->runAction(FadeIn::create(0.5f));

			m_btnReset->runAction(FadeIn::create(0.5f));
		});

		m_Popup->setPopupCloseEffectFunc([=](CPopup* pausePopup){
			moveDistanceBG->runAction(
				Spawn::createWithTwoActions(
				EaseSineIn::create(
				MoveTo::create(0.35f, Vec2(m_BG->getContentSize().width * 0.5f, 
				m_BG->getContentSize().height * 0.5f))),
				FadeTo::create(0.2f, 0)));
			starScoreBG->runAction(
				Spawn::createWithTwoActions(
				EaseSineIn::create(
				MoveTo::create(0.35f, Vec2(m_BG->getContentSize().width * 0.5f,
				m_BG->getContentSize().height * 0.4f))),
				FadeTo::create(0.2f, 0)));
			coinScoreBG->runAction(
				Spawn::createWithTwoActions(
				EaseSineIn::create(
				MoveTo::create(0.35f, Vec2(m_BG->getContentSize().width * 0.5f,
				m_BG->getContentSize().height * 0.3f))),
				FadeTo::create(0.2f, 0)));

			resultLabel->runAction(FadeTo::create(0.5f, 0));

			m_btnHome->runAction(FadeTo::create(0.5f, 0));

			m_btnReset->runAction(FadeTo::create(0.5f, 0));

			m_Popup->scheduleOnce([this](float delta){
				CSpecificPopupBase::PopupRelease();
			}, 0.35f, "PausePopupClose");
		});
	}
	catch (...){
		throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

void CResultPopup::Reset(){
	CCLOG("format popup Replay");
	CGameScene::getGameScene()->GameStart();
	CSpecificPopupBase::PopupClose();
}

void CResultPopup::GoHome(){
	CCLOG("format popup GoHome");
	CGameScene::getGameScene()->GameStart();
	CSpecificPopupBase::PopupClose();
}