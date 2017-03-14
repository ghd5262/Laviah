#include "ChallengeProgressBar.hpp"
#include "../DataManager/UserDataManager.h"

using namespace cocos2d;

CChallengeProgressBar* CChallengeProgressBar::create()
{
    CChallengeProgressBar *pRet = new(std::nothrow) CChallengeProgressBar();
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

bool CChallengeProgressBar::init()
{
    if(!Node::init()) return false;
    
    this->scheduleUpdate();
    
    return true;
}

void CChallengeProgressBar::update(float delta)
{
	// If current challenge is differs from previous challenge.
    auto data = CChallengeDataManager::Instance()->getNonCompleteChallengeFromCurrentList();
    if (!data && !m_ChallengeData)  return;
    if (m_ChallengeData != data  ){
        this->Reset();
        return;
    }

    // calculate percent
    auto value = GLOBAL->getVariable(m_ChallengeData->_materialKeyList.at(0));
    {
        if(value != m_CurrentValue) {
            m_CurrentValue = value;
            m_ProgressBar->runAction(ProgressTo::create(0.5f, this->getPercent(value, m_GoalValue)));
        }
    }
    
    // If challenge complete set complete flag true
    if(this->getPercent(value, m_GoalValue) >= 100.f)
    {
        if(m_Complete) return;
        
        m_Complete = true;
        // set the ui completion ui.
        this->setCompletionUI();
        
        // do last event callback.
        this->retain();
        this->processEventListener();
        this->release();
        return;
    }
}

CChallengeProgressBar* CChallengeProgressBar::show(cocos2d::Node* parent, int zOrder/* = 0*/)
{
    // bar bg
    {
        m_BarBG = LayerColor::create(m_BarBGColor, 1080, 50);
        this->setContentSize(m_BarBG->getContentSize());
        m_BarBG->setIgnoreAnchorPointForPosition(false);
        m_BarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_BarBG->setPosition(getContentSize() / 2);
        this->addChild(m_BarBG);
    }
    
    // bar
    {
        auto bar = Sprite::create("challengeProgress.png");
        bar->setColor(m_BarColor);
        m_ProgressBar = ProgressTimer::create(bar);
        m_ProgressBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_ProgressBar->setPosition(this->getContentSize() / 2);
        m_ProgressBar->setMidpoint(Vec2(0, 0));
        m_ProgressBar->setType(ProgressTimer::Type::BAR);
        m_ProgressBar->setBarChangeRate(Vec2(1, 0));
		m_ProgressBar->setOpacity(255 * 0.8f);
        this->addChild(m_ProgressBar);
    }
    
    // challenge name label
    {
        m_TitleLabel = Label::createWithSystemFont("", FONT::MALGUNBD, 35,
                                            Size(getContentSize().width * 0.9f,
                                                 0),
                                            TextHAlignment::CENTER,
                                            TextVAlignment::TOP);
        m_TitleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_TitleLabel->setPosition(getContentSize() / 2);
        m_TitleLabel->setColor(COLOR::DARKGRAY);
        m_TitleLabel->setVisible(m_LabelVisible);
        this->addChild(m_TitleLabel);
    }
    
    parent->addChild(this, zOrder);
    this->setPosition(m_Position);
    this->setAnchorPoint(m_AnchorPoint);
    
    this->Reset();
    
    return this;
}

CChallengeProgressBar* CChallengeProgressBar::addLastEventListner(const LAST_CALLBACK &callback)
{
    m_EventList.emplace_back(callback);
    return this;
}

CChallengeProgressBar* CChallengeProgressBar::setLabelVisible(bool visible)
{
    m_LabelVisible = visible;
    return this;
}

CChallengeProgressBar* CChallengeProgressBar::setBarBGColor(cocos2d::Color4B color)
{
    m_BarBGColor = color;
    return this;
}

CChallengeProgressBar* CChallengeProgressBar::setBarColor(cocos2d::Color3B color)
{
    m_BarColor = color;
    return this;
}

CChallengeProgressBar* CChallengeProgressBar::setBarPosition(Vec2 pos)
{
    m_Position = pos;
    return this;
}

CChallengeProgressBar* CChallengeProgressBar::setBarAnchorPoint(Vec2 anchorPoint)
{
    m_AnchorPoint = anchorPoint;
    return this;
}

void CChallengeProgressBar::Reset()
{
    //이부분은 여기서 할게 아니라 게임을 처음에 키면 동작해야할듯하다.
//    auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_LIST);
//    if(!list.size()) {
//        CChallengeDataManager::Instance()->getNewChallenges();
//        return;
//    }
    
    
    m_BarBG->updateDisplayedOpacity(0);
    m_TitleLabel->setString("");
    m_ProgressBar->runAction(ProgressTo::create(0.5f, 0));
    m_ChallengeData = CChallengeDataManager::Instance()->getNonCompleteChallengeFromCurrentList();
    if(!m_ChallengeData) return;
    
    m_CurrentChallengeIndex = m_ChallengeData->_index;
	m_GoalValue = m_ChallengeData->_materialValueList.at(0);
    auto content = StringUtils::format(TRANSLATE(m_ChallengeData->_contents).c_str(), m_GoalValue);
	m_TitleLabel->setString(content);
    m_ProgressBar->setColor(m_BarColor);
    m_BarBG->updateDisplayedOpacity(m_BarBGColor.a);
    m_Complete = false;
}

void CChallengeProgressBar::setCompletionUI()
{
    m_ProgressBar->setColor(COLOR::GOLD);
}

void CChallengeProgressBar::processEventListener()
{
    if(m_Complete) return;
    
    for(auto callback : m_EventList)
    {
        callback(this);
    }
}

float CChallengeProgressBar::getPercent(float value, float max)
{
    if(value != 0 && max != 0)
        if(value >= max) return 100.f;
        return (value / max) * 100.f;
    return 0.f;
}