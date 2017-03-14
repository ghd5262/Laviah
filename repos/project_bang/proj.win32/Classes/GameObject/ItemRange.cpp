#include "ItemRange.h"
#include "ObjectManager.h"
#include "../Scene/GameScene.h"

using namespace cocos2d;
using namespace ITEMRANGE;

CItemRange* CItemRange::create()
{
    CItemRange *pRet = new(std::nothrow) CItemRange();
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

CItemRange::CItemRange()
	: m_Texture(nullptr)
	, m_TextureName("")
	, m_StayTime(0.f)
	, m_StayLimitTime(0.f)
	, m_TargetDuration(0.f)
	, m_IsStayStatus(false){}

CItemRange* CItemRange::show(cocos2d::Node* parent, int zOrder/* = 0*/)
{
    this->scheduleUpdate();
	this->setCascadeOpacityEnabled(true);
	this->setVisible(false);
	this->setBoundingRadius(1);

	m_Texture = Sprite::create(m_TextureName);
	this->setContentSize(m_Texture->getContentSize());
	m_Texture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_Texture->setPosition(this->getContentSize() / 2);
	m_Texture->setScale(0.f);
	addChild(m_Texture);

	parent->addChild(this, zOrder);
	return this;
}

CItemRange* CItemRange::setTextureName(std::string name)
{
	m_TextureName = name;
	return this;
}

void CItemRange::update(float delta)
{
    if(CObjectManager::Instance()->getIsGamePause()) return;

	this->setBoundingRadius((getContentSize().width * m_Texture->getScale()) / 2);
	if (!m_IsStayStatus) return;

	m_StayTime += delta;
	if (m_StayTime >= m_StayLimitTime)
	{
		m_IsStayStatus = false;
		this->stayTimeUP();
	}
}

void CItemRange::ItemGet(float time)
{
    //		CAudioManager::Instance()->PlayEffectSound("sounds/barrier.mp3", false);
	this->Clear();
    this->setVisible(true);
	m_StayLimitTime = time;
	if (m_StayLimitTime <= 0.f)
	{
		m_TargetDuration = TARGET_DURATION + STAY_DURATION;
		m_StayTime = m_StayLimitTime;
		m_IsStayStatus = true;
	}
	else
	{
		m_TargetDuration = TARGET_DURATION;
		float scale = (STAY_DISTANCE * 2) / getContentSize().width;
		auto scaleAction = ScaleTo::create(STAY_DURATION, scale);
		auto callFunc = CallFunc::create([=](){	m_IsStayStatus = true; });
		auto sequence = Sequence::createWithTwoActions(scaleAction, callFunc);
		m_Texture->runAction(sequence);
	}
}

void CItemRange::Clear()
{
	this->setBoundingRadius(1.f);
	this->setVisible(false);
	m_Texture->setScale(0.f);
	m_Texture->stopAllActions();
	m_IsStayStatus = false;
	m_StayTime = 0.f;
}

void CItemRange::stayTimeUP()
{
	float scale = (TARGET_DISTANCE * 2) / getContentSize().width;
	auto scaleAction = ScaleTo::create(m_TargetDuration, scale);
	auto callFunc = CallFunc::create([=](){	this->Clear(); });
	auto sequence = Sequence::createWithTwoActions(scaleAction, callFunc);
	m_Texture->runAction(sequence);
}