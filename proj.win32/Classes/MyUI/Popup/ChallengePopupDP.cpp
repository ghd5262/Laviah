#include "ChallengePopupDP.h"
#include "../MyButton.h"
#include "../Popup.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"

USING_NS_CC;

CChallengePopupDP* CChallengePopupDP::create(const sCHALLENGE_PARAM challenge)
{
    CChallengePopupDP *pRet = new(std::nothrow) CChallengePopupDP(challenge);
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

bool CChallengePopupDP::init()
{
    if (!CPopup::init()) return false;

    auto bg = Sprite::create("resultPopup_2.png");
    this->setContentSize(bg->getContentSize());
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setPosition(Vec2(this->getContentSize() / 2));
	bg->setOpacity(0);
	bg->setCascadeOpacityEnabled(true);
    this->addChild(bg);
    


    auto label = Label::createWithTTF(m_Challenge._contents, FONT::MALGUNBD, 45);
	label->setColor(COLOR::DARKGRAY);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.5f));
    bg->addChild(label);
    
    //CMyButton::create()
    //->addEventListener([=](Node* sender){
    //    if(m_SkipCallback){
    //        this->retain();
    //        m_SkipCallback(this);
    //        this->release();
    //    }
    //})
    //->setButtonNormalImage("resetIcon.png")
    //->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT)
    //->setButtonPosition(Vec2(label->getContentSize().width + 100.f, bg->getContentSize().height * 0.5f))
    //->show(bg);
    
	auto delayTime = CUserDataManager::getUserDataSequenceFromList(USERDATA_KEY::CHALLENGE_CUR_LIST, m_Challenge._index) * 0.5f;
    this->setOpenAnimation([=](Node* sender){
        /*auto originPos = getPosition();
        Size ScreenSize = Director::getInstance()->getVisibleSize();
        this->setPosition(Vec2(-ScreenSize.width + originPos.x, originPos.y));
		auto delayAction = DelayTime::create(delayTime);
        auto moveAction = MoveTo::create(1.f, originPos);
        auto fadeInAction = FadeIn::create(1.f);
        auto exponentialAction = EaseExponentialOut::create(moveAction);
        auto twoAction = Spawn::createWithTwoActions(exponentialAction, fadeInAction);*/

		auto delayAction = DelayTime::create(delayTime + 0.5f);
		auto fadeInAction = FadeIn::create(0.5f);

		auto sequence = Sequence::createWithTwoActions(delayAction, fadeInAction);

		bg->runAction(sequence);
    });
    
    this->setCloseAnimation([=](Node* sender){
       /* auto originPos = getPosition();
        Size ScreenSize = Director::getInstance()->getVisibleSize();
        auto targetPos = Vec2(-ScreenSize.width + originPos.x, originPos.y);
		auto delayAction = DelayTime::create(delayTime);
        auto moveAction = MoveTo::create(0.35f, targetPos);
        auto fadeOutAction = FadeTo::create(0.2f, 0);
        auto easeAction = EaseSineIn::create(moveAction);
        auto twoAction = Spawn::createWithTwoActions(easeAction, fadeOutAction);*/

		auto delayAction = DelayTime::create(delayTime);
		auto fadeOutAction = FadeTo::create(0.5f, 0);

		auto sequence = Sequence::createWithTwoActions(delayAction, fadeOutAction);

		bg->runAction(fadeOutAction);
    });

    return true;
}

CPopup* CChallengePopupDP::addSkipEventListner(const SKIP_CALLBACK &callback)
{
    m_SkipCallback = callback;
    
    return this;
}
//
//void CChallengePopupDP::Skip()
//{
//    auto scene = CGameScene::getGameScene();
//    
//    CPopup::create()
//    ->setPositiveButton([=](Node* sender){
//        CCLOG("Skip");
//    }, "Yes")
//    ->setNegativeButton([=](Node* sender){
//        CCLOG("Cancel");
//    }, "No")
//    ->setDefaultAnimation(ePOPUP_ANIMATION::OPEN_CENTER, ePOPUP_ANIMATION::CLOSE_CENTER)
//    ->setBackgroundColor(COLOR::TRANSPARENT_ALPHA)
//    ->setMessage("Are you sure you want to exit StarStarStar?")
//    ->setMessageFont(Color3B::BLACK, 40)
//    ->setPopupAnchorPoint(Vec2::ANCHOR_MIDDLE)
//    ->setPopupPosition(scene->getContentSize() / 2)
//    ->show(scene, ZORDER::POPUP);
//}