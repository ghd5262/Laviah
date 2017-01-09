#include "EarnCoinPopup.h"
#include "../MyButton.h"

CEarnCoinPopup* CEarnCoinPopup::create()
{
	CEarnCoinPopup *pRet = new(std::nothrow) CEarnCoinPopup();
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

bool CEarnCoinPopup::init()
{
	if (!CPopup::init()) return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = LayerColor::create(COLOR::WHITEGRAY_ALPHA, 1080.f, 570.f);
	if (bg != nullptr){
		bg->setIgnoreAnchorPointForPosition(false);
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(this->getContentSize() / 2);
		this->addChild(bg);
	}

	auto mentLabel = Label::createWithTTF(MakeString("Would you like to watch a video to earn %d more?", 500).c_str(), FONT::MALGUNBD, 40);
	if (mentLabel != nullptr)
	{
		mentLabel->setColor(COLOR::DARKGRAY);
		mentLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		mentLabel->setPosition(Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.6f));
		bg->addChild(mentLabel);
	}

	CMyButton::create()
		->addEventListener([=](Node* sender){
		this->Video(sender);
	})
		->setLayer(LayerColor::create(COLOR::DARKGRAY_ALPHA, 430, 150))
		->setContents("Watch")
		->setFont(Color3B::WHITE, 40)
		->setButtonPosition(Vec2(bg->getContentSize().width * 0.275f, bg->getContentSize().height * 0.25f))
		->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->show(bg);


	CMyButton::create()
		->addEventListener([=](Node* sender){
		this->popupClose();
	})
		->setLayer(LayerColor::create(COLOR::BRIGHTRED_ALPHA, 430, 150))
		->setContents("No")
		->setFont(Color3B::WHITE, 40)
		->setButtonPosition(Vec2(bg->getContentSize().width * 0.725f, bg->getContentSize().height * 0.25f))
		->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->show(bg);
    
    this->setDefaultCallback([=](Node* sender){});

	return true;
}

void CEarnCoinPopup::Video(Node* sender){
	CCLOG("format popup Video");
    this->popupClose();
//    CSDKUtil::Instance()->ShowRewardUnityAds(std::bind(&CVideoPopup::Resume, this));
}