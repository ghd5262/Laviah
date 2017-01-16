#include "ItemRange.h"
#include "../Particle/Particles.h"

using namespace cocos2d;

CItemRange* CItemRange::create(std::string textureName, float delayTime/* = 0*/)
{
    CItemRange *pRet = new(std::nothrow) CItemRange(textureName, delayTime);
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

CItemRange::CItemRange(std::string textureName, float delayTime)
: m_TextureName(textureName)
, m_OriginBoundingRadius(0.f)
, m_IsItemTimeUP(true)
, m_DelayTime(delayTime)
{}

bool CItemRange::init()
{
    if (!Node::init()) return false;
    
    this->setCascadeOpacityEnabled(true);
    
    auto texture = Sprite::create(m_TextureName);
    this->setContentSize(texture->getContentSize());
    texture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    texture->setPosition(this->getContentSize() / 2);
    addChild(texture);
    
    this->setScale(0.f);
    this->setVisible(false);
    
    return true;
}

void CItemRange::Execute(float delta)
{
    if (m_IsItemTimeUP) return;
	
    setBoundingRadius((m_OriginBoundingRadius * this->getScale()) * 0.5f);
}

void CItemRange::ItemGet()
{
    //		CAudioManager::Instance()->PlayEffectSound("sounds/barrier.mp3", false);
    this->setVisible(true);
    m_IsItemTimeUP = false;
    
    auto action = cocos2d::Sequence::create(cocos2d::ScaleTo::create(BARRIER_TIMER, BARRIER_SIZE),
                                            cocos2d::CallFunc::create([=](){
        this->itemTimeUP();
    }), nullptr);
    this->runAction(action);
}

void CItemRange::itemTimeUP()
{
    this->setScale(0.f);
    this->setVisible(false);
    setBoundingRadius(0);
    m_IsItemTimeUP = true;
}
