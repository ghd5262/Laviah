#include "GoalPopupDP.h"
#include "../MyButton.h"
#include "../Popup.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"

USING_NS_CC;
using namespace cocos2d::ui;

CGoalPopupDP* CGoalPopupDP::create(const ACHIEVEMENT achievement, int posIndex)
{
    CGoalPopupDP *pRet = new(std::nothrow) CGoalPopupDP(achievement, posIndex);
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

bool CGoalPopupDP::init()
{
    if (!CPopup::init()) return false;

	float delayTime = m_PosIndex * 0.4f;
	this->setCascadeColorEnabled(true);
	this->setOpacity(0);
	this->setContentSize(Size(1080, 270));
    
	auto popupSize = this->getContentSize();
    auto index = m_Achievement._index;
    auto levelData = CAchievementDataManager::Instance()->getCurLevelDataByIndex(index, false);
    auto value = GLOBAL->getVariable(levelData._materialList.at(0)._materialKey);
    auto mtrlValue = levelData._contentsValue;
//    
//	auto value = 0;
//	for (auto key : m_Achievement._materialKeyList)
//		value += GLOBAL->getVariable(key);
//
//	auto mtrlValue = 0;
//	for (auto value : m_Achievement._materialValueList)
//		mtrlValue += value;
	
	bool complete = CAchievementDataManager::Instance()->CheckAchievementComplete(index, false);
	if (complete) value = mtrlValue;

    auto contents = CAchievementDataManager::Instance()->getAchievementContentsByIndex(index, false);
	if (value > 0) contents += StringUtils::format(" (%d%%)", int(getPercent(value, mtrlValue)));
    
	auto label = Label::createWithSystemFont(contents, FONT::MALGUNBD, 45, 
									  Size(popupSize.width * 0.8f, popupSize.height),
                                      TextHAlignment::CENTER,
                                      TextVAlignment::CENTER);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.5f));
	this->addChild(label);
        
    if (!complete &&
        CAchievementDataManager::Instance()->NonCompleteAchievementExist()){
        
        auto skipBtn = CMyButton::create()
        ->addEventListener([=](Node* sender){
            if(m_SkipCallback){
                this->retain();
                m_SkipCallback(this, m_PosIndex);
                this->release();
            }
        })
        ->setButtonNormalImage("skipIcon.png")
        ->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->setButtonPosition(Vec2(popupSize.width * 0.92f, popupSize.height * 0.5f))
		->show(this);

    }
	if (complete)
	{
		label->setOpacity(255 * 0.4f);
		auto completeIcon = Sprite::create("completeIcon.png");
		completeIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		completeIcon->setPosition(Vec2(popupSize.width * 0.92f, popupSize.height * 0.5f));
        completeIcon->setOpacity(255 * 0.4f);
		this->addChild(completeIcon);


		/*auto completeIcon = ProgressTimer::create(completeImg);
		if (completeIcon != nullptr){
			completeIcon->setType(ProgressTimer::Type::BAR);
			completeIcon->setMidpoint(Vec2(0, 0));
			completeIcon->setBarChangeRate(Vec2(1, 0));
			completeIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			completeIcon->setPosition(Vec2(popupSize.width * 0.92f, popupSize.height * 0.5f));
			completeIcon->setColor(COLOR::DARKGRAY);
			this->addChild(completeIcon);
			auto delayAction = DelayTime::create(delayTime + 0.5f);
			auto progressAction = ProgressFromTo::create(0.3f, 0, 100);
			auto sequence = Sequence::createWithTwoActions(delayAction, progressAction);
			completeIcon->runAction(sequence);
			completeIcon->addChild(completeImg);
		}*/
	}
    
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

		auto delayAction  = DelayTime::create(delayTime + 0.3f);
		auto fadeInAction = FadeIn::create(0.5f);
		auto sequence     = Sequence::createWithTwoActions(delayAction, fadeInAction);
		this->runAction(sequence);
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
		this->runAction(fadeOutAction);
    });

    return true;
}

CGoalPopupDP* CGoalPopupDP::addSkipEventListner(const SKIP_CALLBACK &callback)
{
    m_SkipCallback = callback;
    
    return this;
}

float CGoalPopupDP::getPercent(float value, float max)
{
    if(value != 0 && max != 0)
        return std::min(100.f, (value / max) * 100.f);
    return 0.f;
}
