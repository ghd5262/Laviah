#include "HelpPopup.h"
#include "../MyButton.h"
#include "../../Scene/GameScene.h"
#include "ui/UIScrollView.h"
#include "ui/UIImageView.h"
#include "ui/UIPageView.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CHelpPopup* CHelpPopup::create()
{
	CHelpPopup *pRet = new(std::nothrow) CHelpPopup();
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

bool CHelpPopup::init()
{
	if (!CPopup::init()) return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();

	size_t helpImgCount = 2;

	//auto bg = LayerColor::create(Color4B(255, 255, 255, 0), 1080.f, 1920.f);
	//if (bg != nullptr){
	//	bg->setIgnoreAnchorPointForPosition(false);
	//	bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//	bg->setPosition(this->getContentSize() / 2);
	//	this->addChild(bg);
	//}

	// PageView 생성
	auto pageview = PageView::create();
	pageview->setTouchEnabled(true);
	pageview->setContentSize(visibleSize);
	pageview->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 1.5f));
	pageview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	for (int i = 0; i < helpImgCount; i++) {
		// Layout 생성
		auto layout = Layout::create();
		layout->setSize(visibleSize);

		// ImageView를 생성하고 Layout에 add함
		auto imageView = ImageView::create(StringUtils::format("helpImg_%d.png", i + 1));
		imageView->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		layout->addChild(imageView);

		// Layout을 PageView에 add함
		pageview->addPage(layout);
	}

	this->addChild(pageview);


	auto btnEnd = CMyButton::create()
		->addEventListener([=](Node* sender){
		this->End(sender);
	})
		->setButtonNormalImage("endIcon.png")
		->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setButtonPosition(Vec2(this->getContentSize().width * 0.92f, this->getContentSize().height * 0.05f))
		->show(this, 10);

	btnEnd->setCascadeOpacityEnabled(true);
	btnEnd->setOpacity(0);

	this->setOpenAnimation([=](Node* sender){
		pageview->runAction(EaseExponentialOut::create(MoveTo::create(0.8f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f))));

		this->scheduleOnce([=](float delta){
			btnEnd->runAction(FadeIn::create(0.5f));
		}, 0.5f, "HelpPopupOpen");
	});

	this->setCloseAnimation([=](Node* sender){
		pageview->runAction(EaseSineIn::create(MoveTo::create(0.4f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 1.5f))));

		btnEnd->runAction(FadeTo::create(0.5f, 0));
	});

	return true;
}

void CHelpPopup::End(Node* sender){
	CCLOG("format popup End");
	this->popupClose();
}