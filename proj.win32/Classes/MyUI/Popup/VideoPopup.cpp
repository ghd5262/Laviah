#include "VideoPopup.h"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"
#include "../../SDKUtil/SDKUtil.h"

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
        m_BG = LayerColor::create(Color4B(255, 255, 255, 0), 1080.f, 1920.f);
        if(m_BG != nullptr){
            m_BG->ignoreAnchorPointForPosition(false);
            m_BG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_BG->setPosition(Vec2::ZERO);
            m_Popup->addChild(m_BG);
        }

        CGameScene::getGameScene()->CountDown(10, "0", [this](){
            CGameScene::getGameScene()->GameEnd();
            CSpecificPopupBase::PopupClose();
        });
		
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
		m_btnVideo = CMyButton::create(
			"resultPopup_1.png",
			END, 
			std::bind(&CVideoPopup::Video, this),
			EFFECT_SIZEDOWN);

		if (m_btnVideo != nullptr){
			m_btnVideo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_btnVideo->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.3f));
			m_BG->addChild(m_btnVideo);
			m_btnVideo->setOpacity(0);
			m_btnVideo->setCascadeOpacityEnabled(true);
			auto watchVideoIcon = Sprite::create("videoIcon.png");
			if (watchVideoIcon != nullptr)
			{
				watchVideoIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				watchVideoIcon->setPosition(Vec2(- m_btnVideo->getContentSize().width * 0.18f, 0));
				m_btnVideo->addChild(watchVideoIcon);
				watchVideoIcon->setColor(g_labelColor1);
			}

			auto watchVideoLabel = Label::createWithTTF("Watch a Video", "fonts/malgunbd.ttf", 50);
			if (watchVideoLabel != nullptr){
				watchVideoLabel->setColor(g_labelColor1);
				watchVideoLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
				watchVideoLabel->setPosition(Vec2(- m_btnVideo->getContentSize().width * 0.13f, 0));
				m_btnVideo->addChild(watchVideoLabel);
			}
		}


		/* Use Coin */
		m_btnUseCoin = CMyButton::create(
			"resultPopup_1.png",
			END,
			std::bind(&CVideoPopup::UseCoin, this),
			EFFECT_SIZEDOWN);

		if (m_btnUseCoin != nullptr){
			m_btnUseCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_btnUseCoin->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.2f));
			m_BG->addChild(m_btnUseCoin);
			m_btnUseCoin->setOpacity(0);
			m_btnUseCoin->setCascadeOpacityEnabled(true);

			auto useCoinIcon = Sprite::create("coin_5.png");
			if (useCoinIcon != nullptr)
			{
				useCoinIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				useCoinIcon->setPosition(Vec2(- m_btnUseCoin->getContentSize().width * 0.18f, 0));
				m_btnUseCoin->addChild(useCoinIcon);
				useCoinIcon->setColor(g_labelColor3);
			}

			auto useCoinLabel = Label::createWithTTF(StringUtils::format("Use %d Coin", g_coinToRevive).c_str(), "fonts/malgunbd.ttf", 50);
			if (useCoinLabel != nullptr){
				useCoinLabel->setColor(g_labelColor3);
				useCoinLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
				useCoinLabel->setPosition(Vec2(- m_btnUseCoin->getContentSize().width * 0.13f, 0));
				m_btnUseCoin->addChild(useCoinLabel);
			}

			// 돈이 모자르면 버튼 불가처리
			if (CUserDataManager::Instance()->getUserData_Number("USER_COIN") < g_coinToRevive)
				m_btnUseCoin->setBtnUnable(true);
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

		m_btnUserCoin = CUserCoinButton::create();
		if (m_btnUserCoin != nullptr)
		{
			m_btnUserCoin->setPosition(Vec2(m_BG->getContentSize().width * 0.5f,
				m_BG->getContentSize().height * 0.05f));
			m_btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_BG->addChild(m_btnUserCoin);
			m_btnUserCoin->setCascadeOpacityEnabled(true);
			m_btnUserCoin->setOpacity(0);
		}


		m_Popup->setPopupOpenEffectFunc([=](CPopup* pausePopup){
			auto winSize = Director::getInstance()->getWinSize();

			m_btnVideo->runAction(
				Spawn::createWithTwoActions(
				EaseExponentialOut::create(
				MoveTo::create(1.3f, Vec2(m_BG->getContentSize().width * 0.5f,
				m_BG->getContentSize().height * 0.6f))),
				FadeIn::create(1.f)));
			m_btnUseCoin->runAction(
				Spawn::createWithTwoActions(
				EaseExponentialOut::create(
				MoveTo::create(1.3f, Vec2(m_BG->getContentSize().width * 0.5f,
				m_BG->getContentSize().height * 0.55f))),
				FadeIn::create(1.f)));
			
			reviveLabel->runAction(FadeIn::create(0.5f));

			m_btnEnd->runAction(FadeIn::create(0.5f));
			m_btnUserCoin->runAction(FadeIn::create(0.5f));
		});

		m_Popup->setPopupCloseEffectFunc([=](CPopup* pausePopup){
			m_btnVideo->runAction(
				Spawn::createWithTwoActions(
				EaseSineIn::create(
				MoveTo::create(0.35f, Vec2(m_BG->getContentSize().width * 0.5f,
				m_BG->getContentSize().height * 0.3f))),
				FadeTo::create(0.2f, 0)));
			m_btnUseCoin->runAction(
				Spawn::createWithTwoActions(
				EaseSineIn::create(
				MoveTo::create(0.35f, Vec2(m_BG->getContentSize().width * 0.5f,
				m_BG->getContentSize().height * 0.2f))),
				FadeTo::create(0.2f, 0)));
			
			reviveLabel->runAction(FadeTo::create(0.5f, 0));

			m_btnEnd->runAction(FadeTo::create(0.5f, 0));
			m_btnUserCoin->runAction(FadeTo::create(0.5f, 0));
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
    CGameScene::getGameScene()->CountDownCancel();
	CGameScene::getGameScene()->GameEnd();
	CSpecificPopupBase::PopupClose();
}

void CVideoPopup::Video(){
	CCLOG("format popup Video");
	CSDKUtil::Instance()->ShowRewardUnityAds(std::bind(&CVideoPopup::Resume, this));
}

void CVideoPopup::UseCoin(){
	CCLOG("format popup UseCoin");
	if (CUserDataManager::Instance()->CoinUpdate(-g_coinToRevive)){
		Resume();
	}
}

void CVideoPopup::Resume()
{
	CGameScene::getGameScene()->CountDownCancel();
	CGameScene::getGameScene()->GameStart();
	CSpecificPopupBase::PopupClose();
}