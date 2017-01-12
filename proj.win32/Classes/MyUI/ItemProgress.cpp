#include "ItemProgress.hpp"
#include "../GameObject/ObjectManager.h"

using namespace cocos2d;

CItemProgress* CItemProgress::create()
{
    CItemProgress *pRet = new(std::nothrow) CItemProgress();
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

bool CItemProgress::init()
{
    if(!Node::init()) return false;
    
    this->scheduleUpdate();
    
    return true;
}

void CItemProgress::update(float delta)
{
    if(m_IsPause || CObjectManager::Instance()->getIsGamePause()) return;
    
    m_Time += delta;

    // if percentage is zero do last event callback
    if(m_Time <= m_LimitTime)
        m_ProgressBar->setPercentage(this->getPercent(m_LimitTime - m_Time, m_LimitTime));
    else{
        this->retain();
        this->processEventListner();
        m_IsPause = true;
        this->setVisible(false);
        if(m_CleanUP) this->removeFromParent();
        
        this->release();
        return;
    }
}

CItemProgress* CItemProgress::show(cocos2d::Node* parent, int zOrder/* = 0*/)
{
    // bar bg
    auto barBG = Sprite::create("itemProgressBar.png");
    this->setContentSize(barBG->getContentSize());
    barBG->setColor(COLOR::DARKGRAY);
    barBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    barBG->setPosition(Vec2(this->getContentSize() / 2));
    this->addChild(barBG);
    
    // bar
    auto bar = Sprite::create("itemProgressBar.png");
    bar->setColor(m_BarColor);
    m_ProgressBar = ProgressTimer::create(bar);
    m_ProgressBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_ProgressBar->setPosition(this->getContentSize() / 2);
    m_ProgressBar->setMidpoint(Vec2(m_IsDirectionRight, 0));
    m_ProgressBar->setType(ProgressTimer::Type::BAR);
    m_ProgressBar->setBarChangeRate(Vec2(1, 0));
    this->addChild(m_ProgressBar);
    
    auto icon = Sprite::create(m_IconName);
    icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    icon->setPosition(Vec2(-10, this->getContentSize().height * 0.5f));
    this->addChild(icon);
    
    parent->addChild(this, zOrder);
    this->setPosition(m_Position);
    this->setAnchorPoint(m_AnchorPoint);
    
    this->Reset();

    return this;
}

CItemProgress* CItemProgress::addLastEventListner(const LAST_CALLBACK &callback)
{
    m_EventList.emplace_back(callback);
    return this;
}

CItemProgress* CItemProgress::setIcon(std::string icon)
{
    m_IconName = icon;
    return this;
}

CItemProgress* CItemProgress::setBarColor(cocos2d::Color3B color)
{
    m_BarColor = color;
    return this;
}

CItemProgress* CItemProgress::setDirection(bool isRight)
{
    m_IsDirectionRight = isRight;
    return this;
}

CItemProgress* CItemProgress::setCleanUpAtTheLast(bool clean)
{
    m_CleanUP = clean;
    return this;
}

CItemProgress* CItemProgress::setBarPosition(Vec2 pos)
{
    m_Position = pos;
    return this;
}

CItemProgress* CItemProgress::setBarAnchorPoint(Vec2 anchorPoint)
{
    m_AnchorPoint = anchorPoint;
    return this;
}

void CItemProgress::Reset()
{
    m_Time = 0.f;
}

void CItemProgress::processEventListner()
{
    if(m_IsPause) return;
    
    for(auto callback : m_EventList)
    {
        callback(this);
    }
}

float CItemProgress::getPercent(float value, float max)
{
    if(value != 0 && max != 0)
        return (value / max) * 100.f;
    return 0.f;
}