#include "StageProgressBar.hpp"
#include "../DataManager/UserDataManager.h"
#include "../GameObject/ObjectManager.h"
#include "../DataManager/StageDataManager.hpp"

using namespace cocos2d;

CStageProgressBar* CStageProgressBar::create()
{
    CStageProgressBar *pRet = new(std::nothrow) CStageProgressBar();
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

bool CStageProgressBar::init()
{
    if(!Node::init()) return false;
    
    this->schedule([=](float delta){
        this->Update(delta);
    }, 0.5f, "Update");
    
    return true;
}

void CStageProgressBar::Update(float delta)
{
    // calculate percent
    auto value = (int)CObjectManager::Instance()->getLevelTimer();
    {
        if(value != m_CurrentValue) {
            m_CurrentValue = value;
            auto percent = GVALUE->getPercent(value, m_GoalValue);
            m_ProgressBar->runAction(ProgressTo::create(0.5f, percent));
            
            if(m_TitleLabel)
            {
                auto content = StringUtils::format("%d%%", percent);
                m_TitleLabel->setString(content);
            }
        }
    }
    
    // If achievement complete set complete flag true
    if(GVALUE->getPercent(value, m_GoalValue) >= 100.f)
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

CStageProgressBar* CStageProgressBar::show(cocos2d::Node* parent, int zOrder/* = 0*/)
{
    // bar
    {
        auto bar = Sprite::create(m_ProgressBarName);
        bar->setColor(m_BarColor);
        m_ProgressBar = ProgressTimer::create(bar);
        m_ProgressBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_ProgressBar->setPosition(bar->getContentSize() / 2);
        m_ProgressBar->setMidpoint(Vec2(0, 0));
        m_ProgressBar->setType(ProgressTimer::Type::BAR);
        m_ProgressBar->setBarChangeRate(Vec2(1, 0));
        m_ProgressBar->setOpacity(255 * 0.8f);
        this->addChild(m_ProgressBar, 1);
    
        m_BarBG = LayerColor::create(m_BarBGColor, bar->getContentSize().width, bar->getContentSize().height);
        this->setContentSize( bar->getContentSize());
        m_BarBG->setIgnoreAnchorPointForPosition(false);
        m_BarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_BarBG->setPosition(getContentSize() / 2);
        this->addChild(m_BarBG);
        
    }
    
    // achievement name label
    {
        m_TitleLabel = Label::createWithSystemFont("", FONT::MALGUNBD, 35,
                                                   Size(getContentSize().width * 0.9f, 0),
                                                   TextHAlignment::CENTER,
                                                   TextVAlignment::TOP);
        m_TitleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_TitleLabel->setPosition(this->getContentSize() / 2);
        m_TitleLabel->setColor(COLOR::DARKGRAY);
        m_TitleLabel->setCascadeOpacityEnabled(true);
        m_TitleLabel->setVisible(m_LabelVisible);
        this->addChild(m_TitleLabel, 1);
    }
    
    parent->addChild(this, zOrder);
    this->setPosition(m_Position);
    this->setAnchorPoint(m_AnchorPoint);
    
    this->Reset();
    
    return this;
}

CStageProgressBar* CStageProgressBar::addLastEventListner(const LAST_CALLBACK &callback)
{
    m_EventList.emplace_back(callback);
    return this;
}

CStageProgressBar* CStageProgressBar::setBarBGColor(cocos2d::Color4B color)
{
    m_BarBGColor = color;
    return this;
}

CStageProgressBar* CStageProgressBar::setBarColor(cocos2d::Color3B color)
{
    m_BarColor = color;
    return this;
}

CStageProgressBar* CStageProgressBar::setBarPosition(Vec2 pos)
{
    m_Position = pos;
    return this;
}

CStageProgressBar* CStageProgressBar::setProgressBar(std::string name)
{
    m_ProgressBarName = name;
    return this;
}

CStageProgressBar* CStageProgressBar::setLabelVisible(bool visible)
{
    m_LabelVisible = visible;
    return this;
}

CStageProgressBar* CStageProgressBar::setBarAnchorPoint(Vec2 anchorPoint)
{
    m_AnchorPoint = anchorPoint;
    return this;
}

void CStageProgressBar::Reset()
{
    if(m_SavePointList.size()){
        for(auto savePoint : m_SavePointList){
            savePoint->removeFromParent();
        }
        m_SavePointList.clear();
    }
    
    m_BarBG->updateDisplayedOpacity(0);
    m_TitleLabel->setString("0%");
    m_ProgressBar->runAction(ProgressTo::create(0.5f, 0));
    
    m_StageData     = CStageDataManager::Instance()->getStageByIndex(GVALUE->CURRENT_PLANET);
    m_GoalValue     = CStageDataManager::Instance()->getStageFinishTime(GVALUE->CURRENT_PLANET);
    m_CurrentValue  = 0;
    m_Complete      = false;
    
    for(auto savePoint : m_StageData->_savePointList){
        auto stageData = CStageDataManager::Instance()->getStageDataByIndex(GVALUE->CURRENT_PLANET,
                                                                            savePoint-1);
        auto value = stageData._changeTime;
        auto percent = GVALUE->getPercent(value, m_GoalValue);
        auto layer = LayerColor::create(COLOR::BRIGHT_WHITEGRAY_ALPHA, 10, this->getContentSize().height);
        layer->setIgnoreAnchorPointForPosition(false);
        layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        layer->setPosition(Vec2(this->getContentSize().width * (percent / 100.f),
                                 this->getContentSize().height * 0.5f));
        this->addChild(layer);
        m_SavePointList.emplace_back(layer);
    }
    
    m_ProgressBar->setColor(m_BarColor);
    m_BarBG->updateDisplayedOpacity(m_BarBGColor.a);
}

void CStageProgressBar::setCompletionUI()
{
    m_ProgressBar->setColor(COLOR::GOLD);
}

void CStageProgressBar::processEventListener()
{
    if(m_Complete) return;
    
    for(auto callback : m_EventList)
    {
        callback(this);
    }
}
