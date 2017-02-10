#include "CountDown.hpp"

using namespace cocos2d;

CCountDown* CCountDown::create()
{
    CCountDown *pRet = new(std::nothrow) CCountDown();
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

bool CCountDown::init()
{
    if(!Node::init()) return false;
    
    this->scheduleUpdate();
    
    return true;
}

void CCountDown::update(float delta)
{
    if(m_IsPause) return;
    
    m_Time += delta;
    
    if(m_Time <= m_Interval) return;

    this->updateContent();
    
    m_Time = 0.f;
}

CCountDown* CCountDown::show(cocos2d::Node* parent, int zOrder/* = 0*/)
{
    this->Reset();
    
    m_NumberLabel = Label::createWithSystemFont("", m_FontName, m_FontSize);
    m_NumberLabel->setTextColor(m_FontColor);
    m_NumberLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_NumberLabel->setPosition(this->getContentSize() / 2);
	m_NumberLabel->enableOutline(COLOR::BRIGHT_WHITEGRAY_ALPHA, 5);
    this->addChild(m_NumberLabel);
    
    parent->addChild(this, zOrder);
    this->setPosition(m_Position);
    this->setAnchorPoint(m_AnchorPoint);
    
    return this;
}

CCountDown* CCountDown::addLastEventListner(const LAST_CALLBACK &callback)
{
    m_EventList.emplace_back(callback);
    return this;
}

CCountDown* CCountDown::setLastContent(std::string content)
{
    m_LastContent = content;
    return this;
}

CCountDown* CCountDown::setInterval(float interval)
{
    m_Interval = interval;
    return this;
}

CCountDown* CCountDown::setMaxNumber(int number)
{
    m_MaxNumber = number;
    return this;
}

CCountDown* CCountDown::setMinNumber(int number)
{
    m_MinNumber = number;
    return this;
}

CCountDown* CCountDown::setCountUp(bool up)
{
    m_IsCountUP = up;
    return this;
}

CCountDown* CCountDown::setFont(cocos2d::Color4B fontColor, int fontSize)
{
    m_FontColor = fontColor;
    m_FontSize = fontSize;
    return this;
}

CCountDown* CCountDown::setFontName(std::string fontName)
{
    m_FontName = fontName;
    return this;
}

CCountDown* CCountDown::setCleanUpAtTheLast(bool clean)
{
    m_CleanUP = clean;
    return this;
}

CCountDown* CCountDown::setLabelPosition(Vec2 pos)
{
    m_Position = pos;
    return this;
}

CCountDown* CCountDown::setLabelAnchorPoint(Vec2 anchorPoint)
{
    m_AnchorPoint = anchorPoint;
    return this;
}

void CCountDown::Reset()
{
    if(m_MaxNumber >= 0 && m_MinNumber >= 0)
        m_CurrentNumber = m_MaxNumber - m_MinNumber;
    
    m_Time = m_Interval;
    this->Resume();
    this->setVisible(true);
}

void CCountDown::updateContent()
{
    auto currentNumber = m_CurrentNumber;
    if(m_IsCountUP)
        currentNumber = m_MaxNumber - m_CurrentNumber;
    
    if(m_CurrentNumber < 0)
    {
        this->retain();
        this->processEventListener();
        this->Pause();
        this->setVisible(false);
        if(m_CleanUP) this->removeFromParent();
        
        this->release();
        return;
    }
    
    auto content = StringUtils::format("%d", currentNumber);
    if(m_CurrentNumber == 0)
        content = m_LastContent;
    
    m_NumberLabel->setString(content);
    m_CurrentNumber--;
}

void CCountDown::processEventListener()
{
    if(m_IsPause) return;
    
    for(auto callback : m_EventList)
    {
        callback(this);
    }
}