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
    int index = 0;
    auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_LIST);
    if(!list.size()) {
        CChallengeDataManager::Instance()->getNewChallenges();
        return;
    }
    
    // If current challenge is differs from previous challenge.
    {
        
        index = list.at(0);
        if(index != m_CurrentChallengeIndex) {
            // reset ui
            m_CurrentChallengeIndex = index;
            this->Reset();
        }
    }

    // calculate percent
    {
        auto value = GLOBAL->getVariable(m_ChallengeData->_materialKey);
        if(value != m_CurrentValue) {
            m_CurrentValue = value;
            m_ProgressBar->runAction(ProgressTo::create(0.5f, this->getPercent(value, m_GoalValue)));
        }
    }
    
    // If challenge complete set complete flag true
    // TODO: CheckChallengeComplete -> CheckNormalChallengeComplete (noraml should be check with saved data.)
    if(CChallengeDataManager::Instance()->CheckChallengeComplete(index))
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
        auto layer = LayerColor::create(m_BarBGColor, 1080, 50);
        this->setContentSize(layer->getContentSize());
        layer->setIgnoreAnchorPointForPosition(false);
        layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        layer->setPosition(getContentSize() / 2);
        this->addChild(layer);
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
        this->addChild(m_ProgressBar);
    }
    
    // challenge name label
    {
        m_TitleLabel = Label::createWithTTF("", FONT::MALGUNBD, 40,
                                            Size(getContentSize().width * 0.9f,
                                                 getContentSize().height),
                                            TextHAlignment::CENTER,
                                            TextVAlignment::CENTER);
        m_TitleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_TitleLabel->setPosition(getContentSize() / 2);
        m_TitleLabel->setColor(COLOR::DARKGRAY);
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
    auto list = CUserDataManager::Instance()->getUserData_List(USERDATA_KEY::CHALLENGE_CUR_LIST);
    if(!list.size()) {
        CChallengeDataManager::Instance()->getNewChallenges();
        return;
    }
    
    m_ChallengeData = CChallengeDataManager::Instance()->getChallengeByIndex(list.at(0));
    m_GoalValue = m_ChallengeData->_materialValue;
    m_TitleLabel->setString(m_ChallengeData->_contents);
    m_ProgressBar->setColor(m_BarColor);
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