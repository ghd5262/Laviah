#include "PausePopup.h"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../../Scene/GameScene.h"

CPausePopup::CPausePopup()
{}

CPausePopup::~CPausePopup()
{}

CPausePopup* CPausePopup::create()
{
	CPausePopup *pRet = new(std::nothrow) CPausePopup();
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

bool CPausePopup::init()
{
	if (!CPopup::init()) return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto pauseBG = LayerColor::create(COLOR::WHITEGRAY_ALPHA, 1080.f, 570.f);
	if (pauseBG != nullptr){
		pauseBG->setIgnoreAnchorPointForPosition(false);
		pauseBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		pauseBG->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 1.25f));
		this->addChild(pauseBG);
	}

	std::string btnImageName[] = {
		"homeButton.png",
		"restartButton.png",
		"playButton.png",
		"playIcon.png",
		"exitButton.png"
	};

	Vec2 btnPosArray[] = {
		Vec2(pauseBG->getContentSize().width * -1.1f, pauseBG->getContentSize().height * 0.85f),
		Vec2(pauseBG->getContentSize().width * -1.1f, pauseBG->getContentSize().height * 0.7f),
		Vec2(pauseBG->getContentSize().width * -1.1f, pauseBG->getContentSize().height * 0.55f),
		Vec2(this->getContentSize().width * 0.92f, this->getContentSize().height * 0.05f),
		Vec2(this->getContentSize().width * 0.08f, this->getContentSize().height * 0.05f)
	};

	auto createButton = [=](const std::function<void(Node*)> &callback, std::string imageName, Vec2 pos)->CMyButton*{
		return CMyButton::create()
			->addEventListener(callback)
			->setButtonNormalImage(imageName)
			->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->setButtonPosition(pos);
	};
	
	auto btnHome = createButton([=](Node* sender){ this->GoHome(sender); }, btnImageName[0], btnPosArray[0])->show(pauseBG);
	auto btnReset = createButton([=](Node* sender){ this->Reset(sender); }, btnImageName[1], btnPosArray[1])->show(pauseBG);
//	auto btnPlay = createButton([=](Node* sender){ this->Play(sender); }, btnImageName[2], btnPosArray[2])->show(pauseBG);
	auto btnPlay = createButton([=](Node* sender){ this->Play(sender); }, btnImageName[3], btnPosArray[3])->show(this);
	btnPlay->setOpacity(0);
	auto btnExit = createButton([=](Node* sender){ this->GameExit(sender); }, btnImageName[4], btnPosArray[4])->show(this);
	btnExit->setOpacity(0);
	
	auto noticeLabel = Label::createWithTTF("", "fonts/malgunbd.ttf", 25);
	noticeLabel->setColor(Color3B::BLACK);
	noticeLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	noticeLabel->setPosition(Vec2(pauseBG->getContentSize().width * 0.5f, pauseBG->getContentSize().height * 0.5f));
	pauseBG->addChild(noticeLabel);

	auto btnUserCoin = CUserCoinButton::create();
	if (btnUserCoin != nullptr)
	{
		btnUserCoin->setPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.05f));
		btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		btnUserCoin->setCascadeOpacityEnabled(true);
		btnUserCoin->setOpacity(0);
		this->addChild(btnUserCoin);
	}

	this->setOpenAnimation([=](Node* sender){
		this->scheduleOnce([=](float delta){

			auto action = [=](Node* sender, float height){
				sender->runAction(
					EaseExponentialOut::create(
					MoveTo::create(1.0f,
					Vec2(pauseBG->getContentSize().width * 0.15f,
					pauseBG->getContentSize().height * height))));
			};

			action(btnHome, 0.85f);
			action(btnReset, 0.7f);
//			action(btnPlay, 0.55f);

		}, 0.1f, "PausePopupOpen");

		pauseBG->runAction(EaseExponentialOut::create(MoveTo::create(0.5f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.86f))));
		btnExit->runAction(FadeIn::create(0.5f));
		btnPlay->runAction(FadeIn::create(0.5f));
		btnUserCoin->runAction(FadeIn::create(0.5f));
	});

	this->setCloseAnimation([=](Node* sender){

		auto action = [=](Node* sender, float height){
			sender->runAction(
				EaseSineIn::create(
				MoveTo::create(0.4f, 
				Vec2(pauseBG->getContentSize().width * -1.1f,
				pauseBG->getContentSize().height * height))));
		};

		action(btnHome, 0.85f);
		action(btnReset, 0.7f);
//		action(btnPlay, 0.55f);

		this->scheduleOnce([=](float delta){

			btnExit->runAction(FadeTo::create(0.5f, 0));
			btnPlay->runAction(FadeTo::create(0.5f, 0));
			btnUserCoin->runAction(FadeTo::create(0.5f, 0));
			pauseBG->runAction(Sequence::create(
				EaseSineIn::create(
				MoveTo::create(0.3f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 1.25f))), NULL));
		}, 0.1f, "PausePopupClose");
	});

	return true;
}

void CPausePopup::Play(Node* sender){
	CGameScene::getGameScene()->GameResume();
	this->popupClose();
}

void CPausePopup::Reset(Node* sender){
	CGameScene::getGameScene()->GameStart();
    this->popupClose();
}

void CPausePopup::GoHome(Node* sender){
    CGameScene::getGameScene()->OpenGameMenuLayer();
	this->popupClose();
}

void CPausePopup::GameExit(Node* sender){
	CGameScene::getGameScene()->GameExit();
	this->popupClose();
}

void CPausePopup::Help(Node* sender)
{
	CGameScene::getGameScene()->GameHelp();
}

