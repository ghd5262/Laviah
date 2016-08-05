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
		auto userCoin = CScoreUI::create("fonts/Number.ttf", 38, "run.png");
		if (userCoin != nullptr){
			this->addChild(userCoin);
			userCoin->UpdateValue(CUserDataManager::Instance()->getUserData_Number("USER_COIN"));
			userCoin->setVisible(false);
		}
		
		auto coinLabel = Label::createWithTTF(userCoin->getValueString(), "fonts/malgunbd.ttf", 50);
		auto coinIcon = Sprite::create("coinIcon_2.png");
		auto earnCoinBtn = CMyButton::create("coinPlusIcon.png", END, std::bind(&CUserCoinButton::EarnCoin, this), EFFECT_SIZEDOWN);
		/* coinLabelLayer size
		 * ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
		 * | coinIcon  | coinLabel |
		 * ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
		 * 두개 사이 간격은 coinIcon 크기*/
		auto coinLabelLayer = LayerColor::create(Color4B(0, 0, 0, 255 * 0.4f), coinLabel->getContentSize().width +
			(coinIcon->getContentSize().width * 3.f) + earnCoinBtn->getContentSize().width, coinLabel->getContentSize().height * 1.5f);

		if (coinLabelLayer != nullptr)
		{
			coinLabelLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			coinLabelLayer->ignoreAnchorPointForPosition(false);
			this->addChild(coinLabelLayer);

			if (coinLabel != nullptr){
				coinLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
				coinLabel->setPosition(Vec2((coinIcon->getContentSize().width * 2), coinLabelLayer->getContentSize().height * 0.5f));
				coinLabel->setColor(g_labelColor3);
				coinLabelLayer->addChild(coinLabel);
			}

			if (coinIcon != nullptr){
				coinIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				coinIcon->setPosition(Vec2(coinIcon->getContentSize().width, coinLabelLayer->getContentSize().height * 0.5f));
				coinIcon->setColor(g_labelColor3);
				coinLabelLayer->addChild(coinIcon);
			}
			
			if (earnCoinBtn != nullptr){
				earnCoinBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				earnCoinBtn->setPosition(Vec2(coinLabel->getContentSize().width +
					(coinIcon->getContentSize().width * 3.f),
					coinLabelLayer->getContentSize().height * 0.5f));
				earnCoinBtn->setColor(g_labelColor3);
				coinLabelLayer->addChild(earnCoinBtn);
			}
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
