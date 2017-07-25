#include "Planet.h"
#include "Player.h"
#include "ObjectManager.h"
#include "BulletCreator.h"
#include "../Scene/GameScene.h"
#include "../DataManager/PlanetDataManager.hpp"

using namespace cocos2d;
using namespace PLANET_DEFINE;

CPlanet* CPlanet::create()
{
	CPlanet *pRet = new(std::nothrow) CPlanet();
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

CPlanet::CPlanet()
: m_Angle(0.f)
{}

bool CPlanet::init()
{
	//this->DrawDebugBinding();   //for debug
    if (!Node::init()) return false;
    
	auto data = CObjectManager::Instance()->getPlanetParam();
	m_Texture = Sprite::createWithSpriteFrameName(data->_planetTexture);
    if (m_Texture != nullptr){
		this->setContentSize(m_Texture->getContentSize());
		m_Texture->setPosition(this->getContentSize() / 2);
        m_Texture->setAnchorPoint(Vec2(0.5f, 0.5f));
        m_Texture->setCascadeColorEnabled(true);
        addChild(m_Texture);
    }
    
    this->setBoundingRadius(PLANET_DEFINE::BOUNDING_RADIUS);

    return true;
}

void CPlanet::Clear()
{
//    this->setRotation(0);
}

void CPlanet::Crushed()
{
    CObjectManager::Shake(0.01f, 0.2f, 0.1f, 3.0f, m_Texture, this->getContentSize() / 2);
    CObjectManager::Instance()->getPlayer()->Crushed();
}

void CPlanet::Execute(float delta){}

// Dir -1 == Left, 1 == Right
void CPlanet::Rotation(float speed)
{
    m_Angle = this->getRotation() + speed;
    
//	m_Angle = static_cast<int>(m_Angle) % 360;
	this->setRotation(m_Angle);
}

void CPlanet::setPlanetTexture(std::string textureName)
{
    if(m_Texture != nullptr){
        this->m_Texture->setSpriteFrame(textureName);
    }
}

void CPlanet::setPlanetParam(const PLANET* param)
{
    m_PlanetParam = param;
    this->setPlanetTexture(m_PlanetParam->_planetTexture);
}

void CPlanet::StartRotation()
{
    if(this->getActionByTag(100)) return;
    
    auto rotation = RotateBy::create(120.f, 360);
    auto repeat   = RepeatForever::create(rotation);
    repeat->setTag(100);
    this->runAction(repeat);
}

void CPlanet::StopRotation()
{
    this->stopActionByTag(100);
//    this->setRotation(0);
}

