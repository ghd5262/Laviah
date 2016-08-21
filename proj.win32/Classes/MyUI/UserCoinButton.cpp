#include "UserCoinButton.h"
#include "ScoreUI.h"
#include "MyButton.h"
#include "Popup/EarnCoinPopup.h"
#include "../DataManager/UserDataManager.h"

CUserCoinButton* CUserCoinButton::create()
{
	CUserCoinButton *pRet = new(std::nothrow) CUserCoinButton();
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

bool CUserCoinButton::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CUserCoinButton::initVariable()
{
	try{
		m_UserCoin = CScoreUI::create("fonts/Number.ttf", 38, "run.png");
		if (m_UserCoin != nullptr){
			this->addChild(m_UserCoin);
			m_UserCoin->UpdateValue(CUserDataManager::Instance()->getUserData_Number("USER_COIN"));
			m_UserCoin->setVisible(false);
		}
		
		m_CoinLabel = Label::createWithTTF(m_UserCoin->getValueString(), "fonts/malgunbd.ttf", 50);
		m_CoinIcon = Sprite::create("coinIcon_2.png");
		//auto earnCoinBtn = CMyButton::create("coinPlusIcon.png", END, std::bind(&CUserCoinButton::EarnCoin, this), EFFECT_SIZEDOWN);
		/* coinLabelLayer size
		 * ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
		 * | coinIcon  | coinLabel |
		 * ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
		 * 두개 사이 간격은 coinIcon 크기*/
		m_Back = LayerColor::create(Color4B(0, 0, 0, 255 * 0.4f), m_CoinLabel->getContentSize().width + (m_CoinIcon->getContentSize().width * 2.5f) 
			/*+ earnCoinBtn->getContentSize().width*/, m_CoinLabel->getContentSize().height * 1.5f);

		if (m_Back != nullptr)
		{
			this->setContentSize(m_Back->getContentSize());
			m_Back->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_Back->setPosition(this->getContentSize() * 0.5f);
			m_Back->setIgnoreAnchorPointForPosition(false);
			this->addChild(m_Back);

			if (m_CoinLabel != nullptr){
				m_CoinLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
				m_CoinLabel->setPosition(Vec2(m_CoinIcon->getContentSize().width * 2.f, this->getContentSize().height * 0.5f));
				m_CoinLabel->setColor(g_labelColor3);
				m_Back->addChild(m_CoinLabel);
			}

			if (m_CoinIcon != nullptr){
				m_CoinIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				m_CoinIcon->setPosition(Vec2(m_CoinIcon->getContentSize().width, this->getContentSize().height * 0.5f));
				m_CoinIcon->setColor(g_labelColor3);
				m_Back->addChild(m_CoinIcon);
			}
			
//			if (earnCoinBtn != nullptr){
//				earnCoinBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//				earnCoinBtn->setPosition(Vec2(coinLabel->getContentSize().width +
//					(coinIcon->getContentSize().width * 3.f),
//					coinLabelLayer->getContentSize().height * 0.5f));
//				earnCoinBtn->setColor(g_labelColor3);
//				coinLabelLayer->addChild(earnCoinBtn);
//			}
		}
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CUserCoinButton::EarnCoin()
{
	CCLOG("EarnCoin");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto popup = CPopup::createWithSpecificFormat(CEarnCoinPopup::create(), POPUPEFFECT_none);
	popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
		origin.x + visibleSize.height * 0.5f));
	popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	Director::getInstance()->getRunningScene()->addChild(popup, 102);

}

void CUserCoinButton::UpdateUI()
{
	m_CoinLabel->setString(m_UserCoin->getValueString());
	m_Back->setContentSize(Size(m_CoinLabel->getContentSize().width + (m_CoinIcon->getContentSize().width * 2.5f)
		/*+ earnCoinBtn->getContentSize().width*/, m_CoinLabel->getContentSize().height * 1.5f));
	m_CoinLabel->setPosition(Vec2(m_CoinIcon->getContentSize().width * 2.f, this->getContentSize().height * 0.5f));
	m_CoinIcon->setPosition(Vec2(m_CoinIcon->getContentSize().width, this->getContentSize().height * 0.5f));
}
