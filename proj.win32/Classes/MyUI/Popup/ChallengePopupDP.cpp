#include "ChallengePopupDP.h"
#include "../MyButton.h"
#include "../Popup.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"

USING_NS_CC;

CChallengePopupDP* CChallengePopupDP::create(const sCHALLENGE_PARAM challenge, int posIndex)
{
    CChallengePopupDP *pRet = new(std::nothrow) CChallengePopupDP(challenge, posIndex);
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

    auto value = GLOBAL->getVariable(m_Challenge._materialKey);
    auto mtrlValue = m_Challenge._materialValue;
	bool complete = CChallengeDataManager::Instance()->CheckChallengeComplete(m_Challenge._index);
	if (complete) value = mtrlValue;

    auto contents = m_Challenge._contents;
	if (value > 0) contents += StringUtils::format("(%d%%)", int(getPercent(value, mtrlValue)));
    
    auto label = Label::createWithTTF(contents, FONT::MALGUNBD, 45,
                                      Size(this->getContentSize().width * 0.8f,
                                           this->getContentSize().height * 3.f),
                                      TextHAlignment::CENTER,
                                      TextVAlignment::CENTER);
	label->setColor(COLOR::DARKGRAY);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.5f));
    bg->addChild(label);
    
	if (complete) label->setTextColor(COLOR::DARKGRAY_ALPHA);
    
    if (!complete &&
        CChallengeDataManager::Instance()->NonCompleteChallengeExist(m_Challenge._level, false)){
        
        CMyButton::create()
        ->addEventListener([=](Node* sender){
            if(m_SkipCallback){
                this->retain();
                m_SkipCallback(this, m_PosIndex);
                this->release();
            }
        })
        ->setButtonNormalImage("resetIcon.png")
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT)
        ->setButtonPosition(Vec2(bg->getContentSize().width - 10, bg->getContentSize().height * 0.5f))
        ->show(bg);
    }
    
    float delayTime = m_PosIndex * 0.4f;
    this->setOpenAnimation([=](Node* sender){
        /*auto originPos = getPosition();
        Size ScreenSize = Director::getInstance()->getVisibleSize();
        this->setPosition(Vec2(-ScreenSize.width + originPos.x, originPos.y));
		auto delayAction = DelayTime::create(delayTime);
        auto moveAction = MoveTo::create(1.f, originPos);
        auto fadeInAction = FadeIn::create(1.f);
        auto exponentialAction = EaseExponentialOut::create(moveAction);
        auto twoAction = Spawn::createWithTwoActions(exponentialAction, fadeInAction);
		auto sequence = Sequence::createWithTwoActions(delayAction, twoAction);
		bg->runAction(sequence);*/

		auto delayAction = DelayTime::create(delayTime + 0.3f);
		auto fadeInAction = FadeIn::create(0.5f);
		auto sequence = Sequence::createWithTwoActions(delayAction, fadeInAction);
		bg->runAction(sequence);
    });
    
    this->setCloseAnimation([=](Node* sender){
        /*auto originPos = getPosition();
        Size ScreenSize = Director::getInstance()->getVisibleSize();
        auto targetPos = Vec2(-ScreenSize.width + originPos.x, originPos.y);
		auto delayAction = DelayTime::create(delayTime);
        auto moveAction = MoveTo::create(0.35f, targetPos);
        auto fadeOutAction = FadeTo::create(0.2f, 0);
        auto easeAction = EaseSineIn::create(moveAction);
        auto twoAction = Spawn::createWithTwoActions(easeAction, fadeOutAction);
		auto sequence = Sequence::createWithTwoActions(delayAction, twoAction);
		bg->runAction(sequence);*/

		auto fadeOutAction = FadeTo::create(0.5f, 0);
		bg->runAction(fadeOutAction);
    });

    return true;
}

CPopup* CChallengePopupDP::addSkipEventListner(const SKIP_CALLBACK &callback)
{
    m_SkipCallback = callback;
    
    return this;
}

float CChallengePopupDP::getPercent(float value, float max)
{
    if(value != 0 && max != 0)
        return (value / max) * 100.f;
    return 0.f;
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