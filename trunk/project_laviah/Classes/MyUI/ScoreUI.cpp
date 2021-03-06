#include "ScoreUI.h"
#include "../Common/StringUtility.h"
#include "../GameObject/ObjectManager.h"

namespace SCOREUI_DEFINE {
	static const float VISIBLE_LIMIT_TIME = 5.f;
};

using namespace cocos2d;

CScoreUI* CScoreUI::create(int& value)
{
	CScoreUI *pRet = new(std::nothrow) CScoreUI(value);
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

CScoreUI* CScoreUI::show(cocos2d::Node* parent, unsigned zOrder/* = 0*/)
{
    m_BG = LayerColor::create(m_BGColor, m_FontSize, m_FontSize * 2);
    m_BG->setIgnoreAnchorPointForPosition(false);
    m_BG->setAnchorPoint(m_ScoreAnchorPoint);
    this->addChild(m_BG);
    
    if(m_ScoreDistance == -1) m_ScoreDistance = m_FontSize;
	m_ScoreLabel = Label::createWithTTF("0", m_FontName, m_FontSize);
    m_ScoreLabel->setPosition(Vec2(m_FontSize + m_ScoreDistance, m_FontSize));
    m_ScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    m_ScoreLabel->setColor(m_FontColor);
	m_ScoreLabel->enableOutline(COLOR::BRIGHT_WHITEGRAY_ALPHA, m_OutlineSize);
	this->addChild(m_ScoreLabel);

	if (m_IconName != "")
	{
		m_Icon = Sprite::create(m_IconName);
		m_Icon->setAnchorPoint(m_IconAnchorPoint);
        m_Icon->setColor(m_FontColor);
		this->addChild(m_Icon);
	}

	this->scheduleUpdate();
    this->setAnchorPoint(m_ScoreAnchorPoint);
    this->setCascadeOpacityEnabled(true);
    this->setCascadeColorEnabled(true);
    
	parent->addChild(this, zOrder);
	return this;
}

CScoreUI* CScoreUI::setFont(std::string fontName,
                            size_t fontSize,
                            Color3B fontColor/* = WHITE*/,
                            int outlineSize/* = 3*/)
{
	m_FontName    = fontName;
	m_FontSize    = fontSize;
    m_FontColor   = fontColor;
    m_OutlineSize = outlineSize;
	return this;
}

CScoreUI* CScoreUI::setIcon(std::string iconName,
                            float posY/* = 0.5f*/,
                            cocos2d::Vec2 anchor/* = Vec2::ANCHOR_MIDDLE*/)
{
	m_IconName = iconName;
    m_IconPosY = posY;
    m_IconAnchorPoint = anchor;
	return this;
}

CScoreUI* CScoreUI::setScoreDistance(int distance)
{
    m_ScoreDistance = distance;
    return this;
}

CScoreUI* CScoreUI::setBGColor(cocos2d::Color4B color)
{
    m_BGColor = color;
    return this;
}

CScoreUI* CScoreUI::setScoreAnchorPoint(cocos2d::Vec2 anchorPoint)
{
	m_ScoreAnchorPoint = anchorPoint;
	return this;
}

void CScoreUI::update(float delta)
{
	//if (CObjectManager::Instance()->getIsGamePause()) return;
	//m_Time += delta;
	//this->setOpacityByTimer();

	if (m_OldValue == m_ValueRef) return;

	this->timerReset();
	this->setScoreString();
    this->setBGPosition();
	this->setIconPosition();
}

void CScoreUI::setOpacityByTimer()
{
	float time = (SCOREUI_DEFINE::VISIBLE_LIMIT_TIME - m_Time);
	time = std::max(0.f, time);

	float opacity = ((255.f * 0.7f) / (SCOREUI_DEFINE::VISIBLE_LIMIT_TIME - 2.f)) * time;
	opacity = std::min((255.f * 0.7f), opacity);
	this->setOpacity(opacity);
}

void CScoreUI::timerReset()
{
	m_OldValue = m_ValueRef;
	m_Time = 0.f;
}

void CScoreUI::setIconPosition()
{
    if (m_Icon == nullptr) return;
    
    m_Icon->setPosition(Vec2(m_FontSize, this->getContentSize().height * m_IconPosY));
}

void CScoreUI::setScoreString()
{
	auto valueStr = StringUtility::toCommaString(m_OldValue);
	m_ScoreLabel->setString(valueStr);
}

void CScoreUI::setBGPosition()
{
    this->setContentSize(Size((m_FontSize * 2.5f) + m_ScoreLabel->getContentSize().width,
                              m_FontSize * 2.f));
    m_BG->setContentSize(this->getContentSize());
    m_BG->setPosition(Vec2(this->getContentSize().width * m_ScoreAnchorPoint.x,
                           this->getContentSize().height * 0.5f));
}


