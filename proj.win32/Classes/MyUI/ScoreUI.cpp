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
	m_ScoreLabel = Label::createWithTTF("0", m_FontName, m_FontSize);
	m_ScoreLabel->setAnchorPoint(m_ScoreAnchorPoint);
	m_ScoreLabel->enableOutline(COLOR::BRIGHT_WHITEGRAY_ALPHA, 3);
	this->addChild(m_ScoreLabel);

	if (m_IconName != "")
	{
		m_Icon = Sprite::create(m_IconName);
		m_Icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		this->addChild(m_Icon);
	}

	this->scheduleUpdate();
	this->setCascadeOpacityEnabled(true);
	this->setOpacityByTimer();
	this->setIconPosition();
	parent->addChild(this, zOrder);
	return this;
}

CScoreUI* CScoreUI::setFont(std::string fontName, size_t fontSize)
{
	m_FontName = fontName;
	m_FontSize = fontSize;
	return this;
}

CScoreUI* CScoreUI::setIcon(std::string iconName)
{
	m_IconName = iconName;
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
	m_Time += delta;
	this->setOpacityByTimer();

	if (m_OldValue == m_ValueRef) return;

	this->timerReset();
	this->setScoreString();
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

	m_Icon->setPosition(Vec2((m_ScoreLabel->getContentSize().width * -m_ScoreLabel->getAnchorPoint().x) - m_FontSize, 0));
}

void CScoreUI::setScoreString()
{
	auto valueStr = StringUtility::toCommaString(m_OldValue);
	m_ScoreLabel->setString(valueStr);
}
