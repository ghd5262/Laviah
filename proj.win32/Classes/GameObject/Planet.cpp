#include "Planet.h"
#include "Player.h"
#include "ObjectManager.h"
#include "BulletCreator.h"

using namespace cocos2d;
using namespace PLANET_DEFINE;

CPlanet* CPlanet::create(std::string textureName)
{
	CPlanet *pRet = new(std::nothrow) CPlanet(textureName);
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

CPlanet::CPlanet(std::string textureName)
: m_TextureName(textureName)
, m_Angle(0.f)
, m_fRotateSpeed(BULLETCREATOR::ROTATION_SPEED)
, m_fElapsed(0.0f)
{
    this->setBoundingRadius(PLANET_DEFINE::BOUNDING_RADIUS);
}

bool CPlanet::init()
{
	//this->DrawDebugBinding();   //for debug
    if (!Node::init()) return false;
    
    m_pTexture = Sprite::createWithSpriteFrameName(m_TextureName);
    if (m_pTexture != nullptr){
		this->setContentSize(m_pTexture->getContentSize());
		m_pTexture->setPosition(this->getContentSize() / 2);
        m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
        m_pTexture->setCascadeColorEnabled(true);
        addChild(m_pTexture);
    }
    
	this->setScale(1.5f);

    return true;
}

void CPlanet::Clear()
{
    this->setRotation(0);
}

float CPlanet::noise(int x, int y) {
	int n = x + y * 57;
	n = (n << 13) ^ n;
	return (1.0 - ((n * ((n * n * 15731) + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

void CPlanet::CrushShake(float interval, float duration, float speed, float magnitude) {

	//experiment more with these four values to get a rough or smooth effect!
	m_fElapsed = 0.0f;
	
	this->schedule([=](float dt) {
		float randomStart = random(-1000.0f, 1000.0f);
		m_fElapsed += dt;

		float percentComplete = m_fElapsed / duration;

		// We want to reduce the shake from full power to 0 starting half way through
		float damper = 1.0f - clampf(2.0f * percentComplete - 1.0f, 0.0f, 1.0f);

		// Calculate the noise parameter starting randomly and going as fast as speed allows
		float alpha = randomStart + speed * percentComplete;

		// map noise to [-1, 1]
		float x = noise(alpha, 0.0f) * 2.0f - 1.0f;
		float y = noise(0.0f, alpha) * 2.0f - 1.0f;

		x *= magnitude * damper;
		y *= magnitude * damper;
		setPosition(m_OriginPos.x + x, m_OriginPos.y + y);
		CObjectManager::Instance()->getPlayer()->setPosition(
			CObjectManager::Instance()->getPlayer()->getOriginPos().x + x, 
			CObjectManager::Instance()->getPlayer()->getOriginPos().y + y);
		if (m_fElapsed >= duration)
		{
			m_fElapsed = 0;
			this->unschedule("Shake");

			setPosition(m_OriginPos);
			CObjectManager::Instance()->getPlayer()->setPosition(
				CObjectManager::Instance()->getPlayer()->getOriginPos() );
		}

	}, interval, CC_REPEAT_FOREVER, 0.f, "Shake");
}

void CPlanet::Execute(float delta)
{
}

// Dir -1 == Left, 1 == Right
void CPlanet::Rotation(float dir, float delta)
{
	m_Angle = this->getRotation() + (dir * m_fRotateSpeed * delta);
//	m_Angle = static_cast<int>(m_Angle) % 360;
	this->setRotation(m_Angle);
}

void CPlanet::setPlanetTexture(std::string textureName)
{
    if(m_pTexture != nullptr){
        this->m_pTexture->setSpriteFrame(textureName);
    }
}