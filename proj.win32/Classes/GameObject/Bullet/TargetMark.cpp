#include "TargetMark.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../Particle/Particles.h"

using namespace cocos2d;

CTargetMark::CTargetMark()
: m_Bullet(nullptr)
, m_Particle(nullptr)
, m_DeleteTime(0.f){}

CTargetMark::~CTargetMark(){}

CTargetMark* CTargetMark::create()
{
	CTargetMark* pRet = (CTargetMark*)new(std::nothrow)CTargetMark();

	if (pRet && pRet->init())
	{
#if(!USE_MEMORY_POOLING)
        pRet->autorelease();
#endif
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

CTargetMark* CTargetMark::build()
{
    if(m_Bullet == nullptr) return nullptr;
    
	this->setCascadeOpacityEnabled(true);
    float distance = m_Planet->getPosition().distance(m_Bullet->getPosition());
    m_DeleteTime = distance / m_Bullet->getSpeed();
    
    // sprite init
    auto sprite = Sprite::createWithSpriteFrameName("test.png");
    this->setContentSize(sprite->getContentSize());
    sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT); // 왼쪽 끝이 라인의 시작점임
    sprite->setPosition(this->getContentSize() / 2);
    this->addChild(sprite);

    // position init
    auto pos = CBullet::getCirclePosition(getAngle(),
                                          getDistance(),
                                          m_Planet->getPosition());
    this->setPosition(pos);
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    // rotation init
    this->setRotation(-getAngle());

    this->setParticle();
    
    return this;
}

CTargetMark* CTargetMark::setBullet(CBullet *bullet)
{
    m_Bullet = bullet;
    return this;
}

void CTargetMark::setParticle()
{
	m_Particle = CParticle_Line::create("particle_star1.png");
    if (m_Particle != nullptr){
		m_Particle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_Particle->setPosVar(Vec2(this->getContentSize().width * 0.7f, 20));
        m_Particle->setPosition(Vec2(this->getContentSize().width, this->getContentSize().height /2));
        m_Particle->setVisible(false);
        this->addChild(m_Particle, 10);
    }
}

void CTargetMark::setOpacityByTimer()
{
    float time = (m_DeleteTime - m_Time);
    time = std::max(0.f, time);
    float opacity = ((255.f * 0.7f) / (m_DeleteTime - 0.5f)) * time;
    opacity = std::min((255.f * 0.7f), opacity);
    this->setOpacity(opacity);
}

void CTargetMark::Execute(float delta)
{
    m_Time += delta;

    this->setOpacityByTimer();
    
    // set particle when it needs.
    auto item = CItemManager::Instance()->getCurrentItem();
    m_Particle->setVisible(eITEM_FLAG_coin & item || eITEM_FLAG_star & item);
    
	if (m_Time > m_DeleteTime) this->ReturnToMemoryBlock();
}
