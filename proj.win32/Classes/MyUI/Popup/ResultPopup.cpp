#include "ResultPopup.h"
#include "../MyButton.h"
#include "../ScoreUI.h"
#include "../UIManager.h"

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

		/* 이동거리 */
		auto moveDistanceBG = Sprite::create("resultPopup_1.png");
		if (moveDistanceBG != nullptr){
			moveDistanceBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			moveDistanceBG->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * -0.1f));
			m_BG->addChild(moveDistanceBG);

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
			starScoreBG->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * -0.2f));
			m_BG->addChild(starScoreBG);

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
			coinScoreBG->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * -0.3f));
			m_BG->addChild(coinScoreBG);

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

			m_btnHome = CMyButton::create("homeButton.png",
			END,
			std::bind(&CResultPopup::GoHome, this),
			EFFECT_ALPHA);

		if (m_btnHome != nullptr)
		{
			m_btnHome->setPosition(Vec2(m_BG->getContentSize().width * -1.1f,
				m_BG->getContentSize().height * 0.85f));
			m_btnHome->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_BG->addChild(m_btnHome);
		}

		m_btnReset = CMyButton::create("restartButton.png",
			END,
			std::bind(&CResultPopup::Reset, this),
			EFFECT_ALPHA);

		if (m_btnReset != nullptr)
		{
			m_btnReset->setPosition(Vec2(m_BG->getContentSize().width * -1.1f,
				m_BG->getContentSize().height * 0.7f));
			m_btnReset->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_BG->addChild(m_btnReset);
		}

		m_btnPlay = CMyButton::create("playButton.png",
			END,
			std::bind(&CResultPopup::Play, this),
			EFFECT_ALPHA);

		if (m_btnPlay != nullptr)
		{
			m_btnPlay->setPosition(Vec2(m_BG->getContentSize().width * -1.1f,
				m_BG->getContentSize().height * 0.55f));
			m_btnPlay->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_BG->addChild(m_btnPlay);
		}

		m_Popup->setPopupOpenEffectFunc([=](CPopup* pausePopup){
			auto winSize = Director::getInstance()->getWinSize();

			moveDistanceBG->runAction(EaseExponentialOut::create(MoveTo::create(0.5f, Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.7f))));
			starScoreBG->runAction(EaseExponentialOut::create(MoveTo::create(0.5f, Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.65f))));
			coinScoreBG->runAction(EaseExponentialOut::create(MoveTo::create(0.5f, Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.6f))));


			m_btnHome->runAction(MoveTo::create(0.8f, Vec2(m_BG->getContentSize().width * 0.15f,
				m_BG->getContentSize().height * 0.85f)));

			m_btnReset->runAction(MoveTo::create(0.8f, Vec2(m_BG->getContentSize().width * 0.15f,
				m_BG->getContentSize().height * 0.7f)));

			m_btnPlay->runAction(MoveTo::create(0.8f, Vec2(m_BG->getContentSize().width * 0.15f,
				m_BG->getContentSize().height * 0.55f)));

			//pausePopup->runAction(MoveTo::create(0.5f, Vec2(winSize.width * 0.5f, winSize.height * 0.86f)));
		});

		m_Popup->setPopupCloseEffectFunc([=](CPopup* pausePopup){

			moveDistanceBG->runAction(EaseSineIn::create(MoveTo::create(0.25f, Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * -0.1f))));
			starScoreBG->runAction(EaseSineIn::create(MoveTo::create(0.25f, Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * -0.2f))));
			coinScoreBG->runAction(EaseSineIn::create(MoveTo::create(0.25f, Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * -0.3f))));


			m_btnHome->runAction(MoveTo::create(0.8f, Vec2(m_BG->getContentSize().width * -1.1f,
				m_BG->getContentSize().height * 0.85f)));

			m_btnReset->runAction(MoveTo::create(0.8f, Vec2(m_BG->getContentSize().width * -1.1f,
				m_BG->getContentSize().height * 0.7f)));

			m_btnPlay->runAction(MoveTo::create(0.8f, Vec2(m_BG->getContentSize().width * -1.1f,
				m_BG->getContentSize().height * 0.55f)));

			m_Popup->scheduleOnce([this](float delta){
				CSpecificPopupBase::PopupRelease();
			}, 0.7f, "PausePopupClose");
		});
	}
	catch (...){
		throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

void CResultPopup::Play(){
	CCLOG("format popup Play");

	CSpecificPopupBase::PopupClose();
}

void CResultPopup::Reset(){
	CCLOG("format popup Replay");
}

void CResultPopup::GoHome(){
	CCLOG("format popup GoHome");
	CSpecificPopupBase::PopupClose();
}