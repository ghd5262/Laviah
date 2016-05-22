#include "VideoPopup.h"
#include "../MyButton.h"
#include "../../Scene/GameScene.h"

const Color3B g_labelColor1(36, 36, 36);
const Color3B g_labelColor2(255, 255, 255 - 14);
const Color3B g_labelColor3(255, 255, 166);
const int g_coinToRevive = 1500;

CVideoPopup* CVideoPopup::create()
{
	CVideoPopup *pRet = new(std::nothrow) CVideoPopup();
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

bool CVideoPopup::initVariable()
{
	try{
		m_BG = Sprite::create("empty1920.png");
		m_BG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_BG->setPosition(Vec2::ZERO);
		m_Popup->addChild(m_BG);

		
		/* revive label*/
		auto reviveLabel = Label::createWithTTF("Revive", "fonts/malgunbd.ttf", 80);
		if (reviveLabel != nullptr)
		{
			reviveLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			reviveLabel->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.8f));
			reviveLabel->setColor(g_labelColor2);
			m_BG->addChild(reviveLabel);
			reviveLabel->setOpacity(0);
		}

		/* Video */
		auto watchVideoBG = CMyButton::create(
			"resultPopup_1.png",
			END, 
			std::bind(&CVideoPopup::Video, this),
			EFFECT_SIZEDOWN);

		if (watchVideoBG != nullptr){
			watchVideoBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			watchVideoBG->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.5f));
			m_BG->addChild(watchVideoBG);
			watchVideoBG->setOpacity(0);
			watchVideoBG->setCascadeOpacityEnabled(true);
			auto watchVideoIcon = Sprite::create("videoIcon.png");
			if (watchVideoIcon != nullptr)
			{
				watchVideoIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				watchVideoIcon->setPosition(Vec2(- watchVideoBG->getContentSize().width * 0.18f, 0));
				watchVideoBG->addChild(watchVideoIcon);
				watchVideoIcon->setColor(g_labelColor1);
			}

			auto watchVideoLabel = Label::createWithTTF("Watch a Video", "fonts/malgunbd.ttf", 50);
			if (watchVideoLabel != nullptr){
				watchVideoLabel->setColor(g_labelColor1);
				watchVideoLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
				watchVideoLabel->setPosition(Vec2(- watchVideoBG->getContentSize().width * 0.13f, 0));
				watchVideoBG->addChild(watchVideoLabel);
			}
		}


		/* Use Coin */
		auto useCoinBG = CMyButton::create(
			"resultPopup_1.png",
			END,
			std::bind(&CVideoPopup::UseCoin, this),
			EFFECT_SIZEDOWN);

		if (useCoinBG != nullptr){
			useCoinBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			useCoinBG->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.4f));
			m_BG->addChild(useCoinBG);
			useCoinBG->setOpacity(0);
			useCoinBG->setCascadeOpacityEnabled(true);
			auto useCoinIcon = Sprite::create("coin_5.png");
			if (useCoinIcon != nullptr)
			{
				useCoinIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				useCoinIcon->setPosition(Vec2(- useCoinBG->getContentSize().width * 0.18f, 0));
				useCoinBG->addChild(useCoinIcon);
				useCoinIcon->setColor(g_labelColor3);
			}

			auto useCoinLabel = Label::createWithTTF(StringUtils::format("Use %d Coin", g_coinToRevive).c_str(), "fonts/malgunbd.ttf", 50);
			if (useCoinLabel != nullptr){
				useCoinLabel->setColor(g_labelColor3);
				useCoinLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
				useCoinLabel->setPosition(Vec2(- useCoinBG->getContentSize().width * 0.13f, 0));
				useCoinBG->addChild(useCoinLabel);
			}
		}

		m_btnEnd = CMyButton::create("endIcon.png",
			END,
			std::bind(&CVideoPopup::End, this),
			EFFECT_ALPHA);

		if (m_btnEnd != nullptr)
		{
			m_btnEnd->setPosition(Vec2(m_BG->getContentSize().width * 0.92f,
				m_BG->getContentSize().height * 0.05f));
			m_btnEnd->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_BG->addChild(m_btnEnd);
			m_btnEnd->setCascadeOpacityEnabled(true);
			m_btnEnd->setOpacity(0);
		}

		m_Popup->setPopupOpenEffectFunc([=](CPopup* pausePopup){
			auto winSize = Director::getInstance()->getWinSize();

			watchVideoBG->runAction(
				Spawn::createWithTwoActions(
				EaseExponentialOut::create(
				MoveTo::create(1.3f, Vec2(m_BG->getContentSize().width * 0.5f,
				m_BG->getContentSize().height * 0.7f))),
				FadeIn::create(1.f)));
			useCoinBG->runAction(
				Spawn::createWithTwoActions(
				EaseExponentialOut::create(
				MoveTo::create(1.3f, Vec2(m_BG->getContentSize().width * 0.5f,
				m_BG->getContentSize().height * 0.65f))),
				FadeIn::create(1.f)));
			
			reviveLabel->runAction(FadeIn::create(0.5f));

			m_btnEnd->runAction(FadeIn::create(0.5f));
		});

		m_Popup->setPopupCloseEffectFunc([=](CPopup* pausePopup){
			watchVideoBG->runAction(
				Spawn::createWithTwoActions(
				EaseSineIn::create(
				MoveTo::create(0.35f, Vec2(m_BG->getContentSize().width * 0.5f,
				m_BG->getContentSize().height * 0.5f))),
				FadeTo::create(0.2f, 0)));
			useCoinBG->runAction(
				Spawn::createWithTwoActions(
				EaseSineIn::create(
				MoveTo::create(0.35f, Vec2(m_BG->getContentSize().width * 0.5f,
				m_BG->getContentSize().height * 0.4f))),
				FadeTo::create(0.2f, 0)));
			
			reviveLabel->runAction(FadeTo::create(0.5f, 0));

			m_btnEnd->runAction(FadeTo::create(0.5f, 0));
			
			m_Popup->scheduleOnce([this](float delta){
				CSpecificPopupBase::PopupRelease();
			}, 0.35f, "CVideoPopupClose");
		});
	}
	catch (...){
		throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

void CVideoPopup::End(){
	CCLOG("format popup End");
	CGameScene::getGameScene()->GameEnd();
	CSpecificPopupBase::PopupClose();
}

void CVideoPopup::Video(){
	CCLOG("format popup Video");
}

void CVideoPopup::UseCoin(){
	CCLOG("format popup UseCoin");
}