#include "TargetMark.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ItemManager.h"
#include "../../Particle/Particles.h"

using namespace cocos2d;

CTargetMark::CTargetMark(sBULLET_PARAM data, float angle)
: CBullet(data, angle)
, m_Bullet(nullptr)
, m_pParticle(nullptr)
, m_DeleteTime(0.f)
, m_isItemTime(false){}

CTargetMark::~CTargetMark(){}

CTargetMark* CTargetMark::create(sBULLET_PARAM data, float angle)
{
	CTargetMark* pRet = (CTargetMark*)new(std::nothrow)CTargetMark(data, angle);

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
    
    float distance = m_pPlanet->getPosition().distance(m_Bullet->getPosition());
    m_DeleteTime = distance / m_Bullet->getBulletSpeed();
    auto item = CItemManager::Instance()->getCurrentItem();
    m_isItemTime = (eITEM_FLAG_coin & item || eITEM_FLAG_star & item);
    
    if(m_isItemTime)
    {
        this->setParticle();
    }

    auto line = Sprite::create("test.png");
    if (line != nullptr)
    {
        line->setAnchorPoint(Vec2(0.025f, 0.5f));
        line->setOpacity(255 * 0.4f);
        this->addChild(line);
    }
    
    return this;
}

CTargetMark* CTargetMark::setBullet(CBullet *bullet)
{
    m_Bullet = bullet;
    
    return this;
}

bool CTargetMark::init()
{
    if (!CBullet::init()) return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    m_ScreenRect = Rect(-visibleSize.width, 0, visibleSize.width * 3, visibleSize.height);
    
    
    setPositionX((cos(CC_DEGREES_TO_RADIANS(m_fAngle)) * (m_pPlanet->getBRadius() + 20))
                 + m_pPlanet->getPosition().x);
    setPositionY((sin(CC_DEGREES_TO_RADIANS(m_fAngle)) * (m_pPlanet->getBRadius() + 20))
                 + m_pPlanet->getPosition().y);
    setRotation(-m_fAngle);
    
    return true;
}

void CTargetMark::setParticle()
{
    m_pParticle = CParticle_Line::create(MakeString("particle_star%d.png",
                                                    m_BulletParam._isAimingMissile + 1));
    if (m_pParticle != nullptr){
        m_pParticle->retain();
        m_pParticle->setAnchorPoint(Vec2::ZERO);
        m_pParticle->setPosition(Vec2(1300 * 0.32f, 0));
        this->addChild(m_pParticle, 10);
    }
}

void CTargetMark::Rotation(float dir, float delta)
{
	// aimingMissile일 경우 화면안에 들어왔을 때에만 회전한다.
	if (true == m_BulletParam._isAimingMissile && !m_isItemTime){
		if (m_Bullet->IsAlive() && !m_ScreenRect.containsPoint(m_Bullet->getPosition()))
		{
			return;
		}
	}

	CBullet::Rotation(dir, delta);
}

void CTargetMark::Execute(float delta)
{
    auto item = CItemManager::Instance()->getCurrentItem();
    if(eITEM_FLAG_coin & item || eITEM_FLAG_star & item)
    {
        if(m_pParticle == nullptr)
            this->setParticle();
    }
    
    m_fTime += delta;
    if (m_fTime > m_DeleteTime)
        ReturnToMemoryBlock();
}
