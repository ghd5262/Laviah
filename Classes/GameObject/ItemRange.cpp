#include "ItemRange.h"
#include "ObjectManager.h"
#include "../Scene/GameScene.h"

using namespace cocos2d;
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
, m_TargetDuration(0.f){}

CItemRange* CItemRange::show(cocos2d::Node* parent, int zOrder/* = 0*/)
{
    this->scheduleUpdate();
	this->setCascadeOpacityEnabled(true);

	m_Texture = Sprite::create(m_TextureName);
	this->setContentSize(m_Texture->getContentSize());
	m_Texture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_Texture->setPosition(this->getContentSize() / 2);
	addChild(m_Texture);

    this->Clear();
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
    this->setBoundingRadius((getContentSize().width * this->getScale()) / 2);
}

void CItemRange::ItemGet(float time)
{
	this->Clear();
    this->setVisible(true);

    auto scaleUp  = ScaleTo::create(0.2f, 1600.f / getContentSize().width);
    auto delay    = DelayTime::create(time - 0.4f);
    auto fadeOut  = FadeTo::create(0.2f, 0);
    auto callFunc = CallFunc::create([=](){
        this->setScale(0.f);
        this->setVisible(false);
    });
    this->runAction(Sequence::create(scaleUp, delay, fadeOut, callFunc, NULL));
}

void CItemRange::Clear()
{
	this->setBoundingRadius(0);
	this->setVisible(false);
	this->setScale(0.f);
	this->stopAllActions();
    this->setOpacity(255);
	m_StayTime = 0.f;
}
