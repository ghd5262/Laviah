#include "GoogleCloudTestPopupDP.h"
#include "GoogleCloudDataInfoPopup.h"
#include "../MyButton.h"
#include "../../DataManager/UserDataManager.h"

CGoogleCloudTestPopupDP* CGoogleCloudTestPopupDP::create(int keyIdx, const std::function<void(cocos2d::Ref*)> &func)
{
	CGoogleCloudTestPopupDP *pRet = new(std::nothrow) CGoogleCloudTestPopupDP(keyIdx, func);
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
        auto keyList = CUserDataManager::Instance()->getKeyList();
        
		auto dpBack = LayerColor::create(Color4B(0, 0, 0, 0), 1080.f, 200.f);
		if (dpBack != nullptr){
			dpBack->ignoreAnchorPointForPosition(false);
			dpBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			dpBack->setPosition(Vec2::ZERO);
			this->addChild(dpBack);
		}

		auto dpItemBack = LayerColor::create(Color4B(0, 0, 0, 255 * 0.4f), 805.f, 200.f);
		if (dpItemBack != nullptr){
			dpItemBack->ignoreAnchorPointForPosition(false);
			dpItemBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			dpItemBack->setPosition(Vec2(0 + dpItemBack->getContentSize().width * 0.5f, 0));
			dpBack->addChild(dpItemBack);
		}

		auto dpBuyBtn = CMyButton::createWithLayerColor(Size(260, 200), Color4B(0, 0, 0, 255 * 0.8f)
			,"Info" , 40, g_labelColor2, END, std::bind(&CGoogleCloudTestPopupDP::Buy, this), EFFECT_SIZEDOWN);
		if (dpBuyBtn != nullptr)
		{
			dpBuyBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			dpBuyBtn->setPosition(Vec2(dpBack->getContentSize().width - (dpBuyBtn->getContentSize().width * 0.5f), 0));
			dpBack->addChild(dpBuyBtn);
		}

		auto googleCloudKey = Label::createWithTTF(keyList.at(m_UserKeyIdx), "fonts/malgunbd.ttf", 40);
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

void CGoogleCloudTestPopupDP::Buy()
{
	m_SelectFunc(this);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto popup = CPopup::createWithSpecificFormat(CGoogleCloudDataInfoPopup::create(), POPUPEFFECT_none);
	popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
		origin.x + visibleSize.height * 0.5f));
	popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	Director::getInstance()->getRunningScene()->addChild(popup, 102);
}

void CGoogleCloudTestPopupDP::DeSelect()
{
	
}
