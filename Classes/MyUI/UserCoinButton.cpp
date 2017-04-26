#include "UserCoinButton.h"
#include "ScoreUI.h"
#include "MyButton.h"
#include "Popup/EarnCoinPopup.h"
#include "../DataManager/UserDataManager.h"
#include "../Common/StringUtility.h"

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
	if (!Node::init()) return false;

	m_UserData = CUserDataManager::Instance();

	m_Back = LayerColor::create(COLOR::BRIGHTGRAY_ALPHA, 0, 0);
	if (m_Back != nullptr)
	{
		m_Back->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_Back->setIgnoreAnchorPointForPosition(false);
		this->addChild(m_Back);
	}

	m_CoinLabel = Label::createWithTTF("", FONT::MALGUNBD, 50);
	if (m_CoinLabel != nullptr){
		m_CoinLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		m_CoinLabel->setColor(COLOR::GOLD);
		this->addChild(m_CoinLabel);
	}

	m_CoinIcon = Sprite::create("coinIcon_s.png");
	if (m_CoinIcon != nullptr){
		m_CoinIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_CoinIcon->setColor(COLOR::GOLD);
		this->addChild(m_CoinIcon);
	}

	this->setCascadeOpacityEnabled(true);
	this->scheduleUpdate();
	this->update(0.f);	

	return true;
}

void CUserCoinButton::update(float delta)
{
	Size iconSize = m_CoinIcon->getContentSize();
	
	m_CoinLabel->setString(StringUtility::toCommaString(m_UserData->getUserData_Number(USERDATA_KEY::COIN)));
	m_Back->setContentSize(Size(m_CoinLabel->getContentSize().width + (iconSize.width * 2.5f), iconSize.height * 2.0f));
	this->setContentSize(m_Back->getContentSize());
	m_Back->setPosition(this->getContentSize() / 2);
	m_CoinIcon->setPosition(Vec2(iconSize.width, this->getContentSize().height * 0.5f));
	m_CoinLabel->setPosition(Vec2(this->getContentSize().width - iconSize.width * 0.5f, this->getContentSize().height * 0.5f));
}
