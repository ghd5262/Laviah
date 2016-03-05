#include "Bullet.h"
#include "ObjectManager.h"
#include "Planet.h"

CBullet* CBullet::create(
	std::string textureName,	//bullet 이미지
	float boundingRadius,		//bullet 충돌 범위
	float angle,				//bullet 초기 각도 
	float speed,				//bullet 초기 속도
	const CMover* target)		//bullet 타겟 위치
{
	CBullet* pRet = (CBullet*)new(std::nothrow) CBullet(
		textureName
		, boundingRadius
		, angle
		, speed
		, target);
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

CBullet::CBullet(std::string textureName, float boundingRadius, float angle, float speed, const CMover* target)
	: CMover(boundingRadius)
	, m_TextureName(textureName)
	, m_fAngle(angle)
	, m_fBulletSpeed(speed)
	, m_Target(target)
{
	setPositionX((cos(CC_DEGREES_TO_RADIANS(angle)) * 700.f) + target->getPosition().x);
	setPositionY((sin(CC_DEGREES_TO_RADIANS(angle)) * 700.f) + target->getPosition().y);
	setRotation(-angle);
}

bool CBullet::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CBullet::initVariable()
{
	try{
		m_pTexture = Sprite::create(m_TextureName);
		if (m_pTexture != nullptr){
			m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
			addChild(m_pTexture);
		}
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void* CBullet::operator new(size_t size, const std::nothrow_t)
{
	// ObjectManager에서 메모리를 할당 받는다.
	return CObjectManager::Instance()->BulletNew();
}

void CBullet::Execute(float delta)
{
	Vec2 dir = m_Target->getPosition() - getPosition();
	dir.normalize();
	dir *= (m_fBulletSpeed * delta);

	setPosition(getPosition() + dir);

	if (IsHit(const_cast<CMover*>(m_Target)))
	{
		CObjectManager::Instance()->ObjectDelete(this);
	}
	//m_fAngle += m_fAngleRate;
	//m_fBulletSpeed += m_fSpeedRate​​;
	//if (abs(getPositionX()) >= MaxX + Shape->Size || abs(getPositionY()) >= MaxY + Shape->Size) {
	//	setAlive(false);
	//}
}