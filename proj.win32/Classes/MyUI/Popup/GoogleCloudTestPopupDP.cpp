#include "GoogleCloudTestPopupDP.h"
#include "GoogleCloudDataInfoPopup.h"
#include "../MyButton.h"


CGoogleCloudTestPopupDP* CGoogleCloudTestPopupDP::create(std::string key)
{
	CGoogleCloudTestPopupDP *pRet = new(std::nothrow) CGoogleCloudTestPopupDP(key);
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

bool CGoogleCloudTestPopupDP::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CGoogleCloudTestPopupDP::initVariable()
{
    try{
		m_DPBack = LayerColor::create(Color4B(0, 0, 0, 0), 1080.f, 200.f);
		if (m_DPBack != nullptr){
            this->setContentSize(m_DPBack->getContentSize());
            
			m_DPBack->setIgnoreAnchorPointForPosition(false);
			m_DPBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
			m_DPBack->setPosition(m_DPBack->getContentSize() * 0.5f);
			this->addChild(m_DPBack);
		}

		auto dpItemBack = LayerColor::create(Color4B(0, 0, 0, 255 * 0.4f), 805.f, 200.f);
		if (dpItemBack != nullptr){
			dpItemBack->setIgnoreAnchorPointForPosition(false);
			dpItemBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			dpItemBack->setPosition(Vec2(0 + dpItemBack->getContentSize().width * 0.5f, 0));
			m_DPBack->addChild(dpItemBack);
		}

		auto dpBuyBtn = CMyButton::createWithLayerColor(Size(260, 200), Color4B(0, 0, 0, 255 * 0.8f)
			, "Info", 40, g_labelColor2, END, [=](Node* sender){this->Buy(sender); }, EFFECT_SIZEDOWN);
		if (dpBuyBtn != nullptr)
		{
			dpBuyBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			dpBuyBtn->setPosition(Vec2(m_DPBack->getContentSize().width - (dpBuyBtn->getContentSize().width * 0.5f), 0));
			m_DPBack->addChild(dpBuyBtn);
		}

		auto googleCloudKey = Label::createWithTTF(m_UserKey, "fonts/malgunbd.ttf", 40);
		if (googleCloudKey != nullptr)
		{
			googleCloudKey->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			googleCloudKey->setPosition(Vec2(dpItemBack->getContentSize().width * 0.25f, dpItemBack->getContentSize().height * 0.8f));
			googleCloudKey->setColor(g_labelColor2);
			dpItemBack->addChild(googleCloudKey);
		}

		auto googleCloudValue = Label::createWithTTF("Key Value", "fonts/malgun.ttf", 35);
		if (googleCloudValue != nullptr)
		{
			googleCloudValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			googleCloudValue->setPosition(Vec2(dpItemBack->getContentSize().width * 0.25f, dpItemBack->getContentSize().height * 0.5f));
			googleCloudValue->setColor(g_labelColor2);
			dpItemBack->addChild(googleCloudValue);
		}
	}
	catch (...){
		throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

void CGoogleCloudTestPopupDP::Buy(Node* sender)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto popup = CPopup::createWithSpecificFormat(CGoogleCloudDataInfoPopup::create(m_UserKey), POPUPEFFECT_none);
	popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
		origin.x + visibleSize.height * 0.5f));
	popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	Director::getInstance()->getRunningScene()->addChild(popup, 102);
}

void CGoogleCloudTestPopupDP::DeSelect(Node* sender)
{
	
}
