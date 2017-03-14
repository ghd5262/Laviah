#include "PausePopup.h"
#include "ChallengePopupDP.h"
#include "../MyButton.h"
#include "../UserCoinButton.h"
#include "../ChallengeProgressBar.hpp"
#include "../../Scene/GameScene.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Player.h"
#include "../../DataManager/UserDataManager.h"
#include <array>

CPausePopup::CPausePopup(){}

CPausePopup::~CPausePopup(){}

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
    m_ChallengeList.resize(CHALLENGE_DEFINE::LIMIT_COUNT);
    std::fill(m_ChallengeList.begin(), m_ChallengeList.end(), nullptr);
    
    
//    this->createRewardBox();
    
    auto pauseBG = LayerColor::create(COLOR::WHITEGRAY_ALPHA, 1080.f, 570.f);
	if (pauseBG != nullptr){
		pauseBG->setIgnoreAnchorPointForPosition(false);
		pauseBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		pauseBG->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 1.25f));
		this->addChild(pauseBG);
	}
    
    this->initChallengeList();

	std::string btnImageName[] = {
		"homeButton.png",
		"restartButton.png",
		"playButton.png",
		"playIcon.png",
		"exitButton.png"
	};

	std::array<Vec2, 5> btnStartPosArray = {
		Vec2(pauseBG->getContentSize().width * -1.1f, pauseBG->getContentSize().height * 0.85f),
		Vec2(pauseBG->getContentSize().width * -1.1f, pauseBG->getContentSize().height * 0.7f),
		Vec2(pauseBG->getContentSize().width * -1.1f, pauseBG->getContentSize().height * 0.55f),
		Vec2(this->getContentSize().width * 0.92f, this->getContentSize().height * 0.05f),
		Vec2(this->getContentSize().width * 0.08f, this->getContentSize().height * 0.05f)
	};

	std::array<Vec2, 3> btnOriginPosArray = {
		Vec2(pauseBG->getContentSize().width * 0.15f, pauseBG->getContentSize().height * 0.85f),
		Vec2(pauseBG->getContentSize().width * 0.15f, pauseBG->getContentSize().height * 0.7f),
		Vec2(pauseBG->getContentSize().width * 0.15f, pauseBG->getContentSize().height * 0.55f)
	};

	auto createButton = [=](const std::function<void(Node*)> &callback, std::string imageName, Vec2 pos)->CMyButton*{
		return CMyButton::create()
			->addEventListener(callback)
            ->setButtonSingleUse(true)
			->setButtonNormalImage(imageName)
			->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
			->setButtonPosition(pos);
	};
	
	auto btnHome = createButton([=](Node* sender){ this->GoHome(sender); }, btnImageName[0], btnStartPosArray[0])->show(pauseBG);
	auto btnReset = createButton([=](Node* sender){ this->Reset(sender); }, btnImageName[1], btnStartPosArray[1])->show(pauseBG);
//	auto btnPlay = createButton([=](Node* sender){ this->Play(sender); }, btnImageName[2], btnStartPosArray[2])->show(pauseBG);
	auto btnPlay = createButton([=](Node* sender){ this->Play(sender); }, btnImageName[3], btnStartPosArray[3])->show(this);
	btnPlay->setOpacity(0);
	auto btnExit = createButton([=](Node* sender){ this->GameExit(sender); }, btnImageName[4], btnStartPosArray[4])->show(this);
	btnExit->setOpacity(0);

	auto btnUserCoin = CUserCoinButton::create();
	if (btnUserCoin != nullptr)
	{
		btnUserCoin->setPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.05f));
		btnUserCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		btnUserCoin->setCascadeOpacityEnabled(true);
		btnUserCoin->setOpacity(0);
		this->addChild(btnUserCoin);
	}
    
    CChallengeProgressBar::create()
    ->setBarBGColor(COLOR::TRANSPARENT_ALPHA)
    ->setBarColor(COLOR::GOLD)
    ->setBarAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM)
    ->setBarPosition(Vec2(pauseBG->getContentSize().width * 0.5f, 0))
    ->setLabelVisible(false)
    ->show(pauseBG);

    this->setOpenAnimation([=](Node* sender){

		auto action = [=](Node* sender, Vec2 pos){
			auto delayAction = DelayTime::create(0.1f);
			auto moveAction = MoveTo::create(1.f, pos);
			auto exponentialAction = EaseExponentialOut::create(moveAction);
			auto sequence = Sequence::createWithTwoActions(delayAction, exponentialAction);
			sender->runAction(sequence);
		};

		action(btnHome, btnOriginPosArray[0]);
		action(btnReset, btnOriginPosArray[1]);
		//			action(btnPlay, 0.55f);

		pauseBG->runAction(EaseExponentialOut::create(MoveTo::create(0.5f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.86f))));
		btnExit->runAction(FadeIn::create(0.5f));
		btnPlay->runAction(FadeIn::create(0.5f));
		btnUserCoin->runAction(FadeIn::create(0.5f));
	});

	this->setCloseAnimation([=](Node* sender){

		auto action = [=](Node* sender, Vec2 pos){
			auto moveAction = MoveTo::create(0.4f, pos);
			auto easeAction = EaseSineIn::create(moveAction);
			sender->runAction(easeAction);
		};

		action(btnHome, btnStartPosArray[0]);
		action(btnReset, btnStartPosArray[1]);
//		action(btnPlay, 0.55f);

		this->scheduleOnce([=](float delta){

			btnExit->runAction(FadeTo::create(0.5f, 0));
			btnPlay->runAction(FadeTo::create(0.5f, 0));
			btnUserCoin->runAction(FadeTo::create(0.5f, 0));
			pauseBG->runAction(Sequence::create(
				EaseSineIn::create(
				MoveTo::create(0.3f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 1.25f))), NULL));
		}, 0.1f, "PausePopupClose");

        for (auto dp : m_ChallengeList){
            if(dp == nullptr) continue;
            
            dp->popupClose();
        }
	});

    this->setDefaultCallback([=](Node* sender){
        this->Play(sender);
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
	CGameScene::getGameScene()->GameExit(true);
	this->popupClose();
}

void CPausePopup::Help(Node* sender)
{
	CGameScene::getGameScene()->GameHelp();
}

void CPausePopup::Skip(CChallengePopupDP *sender, int posIndex)
{
    auto dp = sender;

	CGameScene::getGameScene()->CreateAlertPopup()
    ->setPositiveButton([=](Node* sender){
        auto newData = CChallengeDataManager::Instance()->SkipChallenge(dp->getChallengeParam()._index);
        dp->popupClose();
        this->createChallengeDP(newData, posIndex);
    }, TRANSLATE("BUTTON_YES"))
    ->setNegativeButton([=](Node* sender){
    }, TRANSLATE("BUTTON_NO"))
    ->setMessage(TRANSLATE("CHALLENGE_SKIP_CHECK"))
	->show(CGameScene::getGameScene(), ZORDER::POPUP);
}

void CPausePopup::initChallengeList()
{
    int posIndex = 0;
    auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_NORMAL_LIST);
    for (auto index : list)
    {
        auto challengeData = CChallengeDataManager::Instance()->getNormalChallengeByIndex(index);
        this->createChallengeDP(challengeData, posIndex++);
    }
}

void CPausePopup::createChallengeDP(const sCHALLENGE_PARAM* data, int posIndex)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 posArray[] = {
        Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f),
        Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.425f),
        Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.35f)
    };
    
    auto dp = CChallengePopupDP::create(data, posIndex)
    ->addSkipEventListner([=](CChallengePopupDP* sender, int posIdx){
        this->Skip(sender, posIdx);
    })
    ->setDefaultCallbackEnable(false)
    ->setBackgroundVisible(false)
    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
    ->setPopupPosition(posArray[posIndex])
    ->show(this, ZORDER::POPUP);
    m_ChallengeList.at(posIndex) = dp;
}

void CPausePopup::createRewardBox()
{
#if CC_USE_PHYSICS

    auto popupSize = getContentSize();
    auto createWall = [=](Vec2 point1, Vec2 point2){
        auto node = Node::create();
        node->addComponent(PhysicsBody::createEdgeSegment(point1, point2));
        this->addChild(node);
    };
    
    createWall(Vec2(popupSize.width * 0.05f,
                    popupSize.height),
               Vec2(popupSize.width * 0.05f,
                    popupSize.height * 0.2f));
    
    createWall(Vec2(popupSize.width * 0.05f,
                    popupSize.height * 0.2f),
               Vec2(popupSize.width * 0.95f,
                    popupSize.height * 0.2f));
    
    createWall(Vec2(popupSize.width * 0.95f,
                    popupSize.height),
               Vec2(popupSize.width * 0.95f,
                    popupSize.height * 0.2f));

    
    auto box = Sprite::create("rewardIconBig.png");
    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    box->setPosition(Vec2(popupSize.width * 0.5f,
                          popupSize.height * 1.5f));
    box->addComponent(PhysicsBody::createBox(box->getContentSize()));
    box->setRotation(random<float>(0, 20) + 350.f);
    box->getPhysicsBody()->setMass(30000);
    box->getPhysicsBody()->setVelocity(Vec2(0, -98 * 20));
    this->addChild(box);
#endif
    
}