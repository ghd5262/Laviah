#include "GoogleCloudTestPopupDP.h"
#include "../MyButton.h"

CGoogleCloudTestPopupDP* CGoogleCloudTestPopupDP::create(int workshopItemIdx, const std::function<void(cocos2d::Ref*)> &func)
{
	CGoogleCloudTestPopupDP *pRet = new(std::nothrow) CGoogleCloudTestPopupDP(workshopItemIdx, func);
	if (pRet && pRet->init())
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

bool CGoogleCloudTestPopupDP::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CGoogleCloudTestPopupDP::initVariable()
{
	try{
		//auto workshopItemInfo = CWorkshopItemDataManager::Instance()->getWorkshopItemInfoByIndex(m_WorkshopItemIdx);
		int currentLevel = 3;
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

		auto googleCloudKey = Label::createWithTTF("Key Name", "fonts/malgunbd.ttf", 40);
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
	auto popup = CPopup::createWithSpecificFormat(CTextFieldPopup::create(), POPUPEFFECT_none);
	popup->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
		origin.x + visibleSize.height * 0.5f));
	popup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	Director::getInstance()->getRunningScene()->addChild(popup, 102);
}

void CGoogleCloudTestPopupDP::DeSelect()
{
	
}


CTextFieldPopup* CTextFieldPopup::create()
{
	CTextFieldPopup *pRet = new(std::nothrow) CTextFieldPopup();
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

bool CTextFieldPopup::initVariable()
{
	try{
		// Register Touch Event
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(CTextFieldPopup::onTouchBegan, this);
		listener->onTouchEnded = CC_CALLBACK_2(CTextFieldPopup::onTouchEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		auto dpBack = LayerColor::create(Color4B(0, 0, 0, 255 * 0.8f), 900.f, 1500.f);
		if (dpBack != nullptr){
			dpBack->ignoreAnchorPointForPosition(false);
			dpBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			dpBack->setPosition(Vec2::ZERO);
			m_Popup->addChild(dpBack);
		}

		auto btnEnd = CMyButton::create("endIcon.png",
			END,
			std::bind(&CTextFieldPopup::End, this),
			EFFECT_ALPHA);

		if (btnEnd != nullptr)
		{
			btnEnd->setPosition(Vec2(dpBack->getContentSize().width - btnEnd->getContentSize().width * 0.5f,
				dpBack->getContentSize().height - btnEnd->getContentSize().height * 0.5f ));
			btnEnd->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			dpBack->addChild(btnEnd);
			btnEnd->setCascadeOpacityEnabled(true);
		}

		_trackNode = TextFieldTTF::textFieldWithPlaceHolder("<click here for input>", "fonts/malgun.ttf", 50);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)    
		// on android, TextFieldTTF cannot auto adjust its position when soft-keyboard pop up
		// so we had to set a higher position to make it visable
		_trackNode->setPosition(Vec2(dpBack->getContentSize().width / 2, dpBack->getContentSize().height / 2 + 50));
#else
		_trackNode->setPosition(Vec2(dpBack->getContentSize().width / 2, dpBack->getContentSize().height  + 50));
#endif
		dpBack->addChild(_trackNode);


		m_Popup->setPopupOpenEffectFunc([this](CPopup* pausePopup){
		});

		m_Popup->setPopupCloseEffectFunc([this](CPopup* pausePopup){
			m_Popup->scheduleOnce([this](float delta){
					CSpecificPopupBase::PopupRelease();
			}, 0.1f, "CTextFieldPopupClose");
		});
	}
	catch (...){
		throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

Rect CTextFieldPopup::getRect(Node * node)
{
	Rect rc;
	rc.origin = node->getPosition();
	rc.size = node->getContentSize();
	rc.origin.x -= rc.size.width / 2;
	rc.origin.y -= rc.size.height / 2;
	return rc;
}

void CTextFieldPopup::keyboardWillShow(IMEKeyboardNotificationInfo& info)
{
	CCLOG("TextInputTest:keyboardWillShowAt(origin:%f,%f, size:%f,%f)",
		info.end.origin.x, info.end.origin.y, info.end.size.width, info.end.size.height);

	if (!_trackNode)
	{
		return;
	}

	auto rectTracked = getRect(_trackNode);
	CCLOG("TextInputTest:trackingNodeAt(origin:%f,%f, size:%f,%f)",
		rectTracked.origin.x, rectTracked.origin.y, rectTracked.size.width, rectTracked.size.height);

	// if the keyboard area doesn't intersect with the tracking node area, nothing need to do.
	if (!rectTracked.intersectsRect(info.end))
	{
		return;
	}

	// assume keyboard at the bottom of screen, calculate the vertical adjustment.
	float adjustVert = info.end.getMaxY() - rectTracked.getMinY();
	CCLOG("TextInputTest:needAdjustVerticalPosition(%f)", adjustVert);

	// move all the children node of CTextFieldPopup
	auto& children = getChildren();
	Node * node = 0;
	ssize_t count = children.size();
	Vec2 pos;
	for (int i = 0; i < count; ++i)
	{
		node = children.at(i);
		pos = node->getPosition();
		pos.y += adjustVert;
		node->setPosition(pos);
	}
}

// Layer function

bool CTextFieldPopup::onTouchBegan(Touch  *touch, Event  *event)
{
	CCLOG("++++++++++++++++++++++++++++++++++++++++++++");
	_beginPos = touch->getLocation();
	return true;
}

void CTextFieldPopup::onTouchEnded(Touch  *touch, Event  *event)
{
	if (!_trackNode)
	{
		return;
	}

	auto endPos = touch->getLocation();

	float delta = 5.0f;
	if (::abs(endPos.x - _beginPos.x) > delta
		|| ::abs(endPos.y - _beginPos.y) > delta)
	{
		// not click
		_beginPos.x = _beginPos.y = -1;
		return;
	}

	// decide the trackNode is clicked.
	Rect rect;
	auto point = convertTouchToNodeSpaceAR(touch);
	CCLOG("CTextFieldPopup:clickedAt(%f,%f)", point.x, point.y);

	rect = getRect(_trackNode);
	CCLOG("CTextFieldPopup:TrackNode at(origin:%f,%f, size:%f,%f)",
		rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);

	this->onClickTrackNode(rect.containsPoint(point));
	CCLOG("----------------------------------");
}

void CTextFieldPopup::onClickTrackNode(bool bClicked)
{
	auto pTextField = _trackNode;
	if (bClicked)
	{
		// TextFieldTTFTest be clicked
		CCLOG("TextFieldTTFDefaultTest:TextFieldTTF attachWithIME");
		pTextField->attachWithIME();
	}
	else
	{
		// TextFieldTTFTest not be clicked
		CCLOG("TextFieldTTFDefaultTest:TextFieldTTF detachWithIME");
		pTextField->detachWithIME();
	}
}

void CTextFieldPopup::End(){
	CCLOG("format popup End");
	CSpecificPopupBase::PopupClose();
}