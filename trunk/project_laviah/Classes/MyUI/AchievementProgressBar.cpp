#include "AchievementProgressBar.hpp"
#include "../DataManager/UserDataManager.h"
#include "../GameObject/ObjectManager.h"

using namespace cocos2d;

CAchievementProgressBar* CAchievementProgressBar::create()
{
    CAchievementProgressBar *pRet = new(std::nothrow) CAchievementProgressBar();
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

bool CAchievementProgressBar::init()
{
    if(!Node::init()) return false;
    
    this->schedule([=](float delta){
        this->Update(delta);
    }, 0.5f, "Update");
    
    return true;
}

void CAchievementProgressBar::Update(float delta)
{
    if(CObjectManager::Instance()->getIsGamePause()) return;
    if (!m_AchievementData) return;

    // calculate percent
    auto value = CAchievementDataManager::Instance()->getNormalAchievementCurrentValue(m_AchievementData->_index);
    {
        if(value != m_CurrentValue) {
            m_CurrentValue = value;
            m_ProgressBar->runAction(ProgressTo::create(0.5f, this->getPercent(value, m_GoalValue)));
        }
    }
    
    // If achievement complete set complete flag true
    if(this->getPercent(value, m_GoalValue) >= 100.f)
    {
        if(m_Complete) return;
        
        m_Complete = true;
        // set the ui completion ui.
        this->setCompletionUI();
        
        // do last event callback.
        this->retain();
        this->processEventListener();
        this->Reset();
        this->release();
        return;
    }
}

CAchievementProgressBar* CAchievementProgressBar::show(cocos2d::Node* parent, int zOrder/* = 0*/)
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
        auto bar = Sprite::create("achievementProgress.png");
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
    
    // achievement name label
    {
        m_TitleLabel = Label::createWithSystemFont("", FONT::MALGUNBD, 35,
                                            Size(getContentSize().width * 0.9f, 0),
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

CAchievementProgressBar* CAchievementProgressBar::addLastEventListner(const LAST_CALLBACK &callback)
{
    m_EventList.emplace_back(callback);
    return this;
}

CAchievementProgressBar* CAchievementProgressBar::setLabelVisible(bool visible)
{
    m_LabelVisible = visible;
    return this;
}

CAchievementProgressBar* CAchievementProgressBar::setBarBGColor(cocos2d::Color4B color)
{
    m_BarBGColor = color;
    return this;
}

CAchievementProgressBar* CAchievementProgressBar::setBarColor(cocos2d::Color3B color)
{
    m_BarColor = color;
    return this;
}

CAchievementProgressBar* CAchievementProgressBar::setBarPosition(Vec2 pos)
{
    m_Position = pos;
    return this;
}

CAchievementProgressBar* CAchievementProgressBar::setBarAnchorPoint(Vec2 anchorPoint)
{
    m_AnchorPoint = anchorPoint;
    return this;
}

void CAchievementProgressBar::Reset()
{
    auto achievementMng = CAchievementDataManager::Instance();
    m_BarBG->updateDisplayedOpacity(0);
    m_TitleLabel->setString("");
    m_ProgressBar->runAction(ProgressTo::create(0.5f, 0));
    m_AchievementData = achievementMng->getFirstFromNonCompleted();
    if(!m_AchievementData) return;
    
    m_CurrentAchievementIndex = m_AchievementData->_index;
    m_GoalValue  = achievementMng->getNormalAchievementMaterialValue(m_CurrentAchievementIndex);
    auto content = achievementMng->getAchievementContentsByIndex(m_CurrentAchievementIndex, false);
	m_TitleLabel->setString(content);
    m_ProgressBar->setColor(m_BarColor);
    m_BarBG->updateDisplayedOpacity(m_BarBGColor.a);
    m_Complete = false;
}

void CAchievementProgressBar::setCompletionUI()
{
    m_ProgressBar->setColor(COLOR::GOLD);
}

void CAchievementProgressBar::processEventListener()
{
    if(m_Complete) return;
    
    for(auto callback : m_EventList)
    {
        callback(this);
    }
}

float CAchievementProgressBar::getPercent(float value, float max)
{
    if(value != 0 && max != 0)
        if(value >= max) return 100.f;
        return (value / max) * 100.f;
    return 0.f;
}
