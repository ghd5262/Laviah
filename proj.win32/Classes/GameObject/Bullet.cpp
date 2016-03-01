#include "Bullet.h"
#include "ObjectManager.h"

CBullet::CBullet(std::string textureName, float boundingRadius, float angle, float speed, Vec2 targetVec)
	: CMover(CObjectManager::Instance()->getM_BulletList(), textureName, boundingRadius)
	, m_fAngle(angle)
	, m_fBulletSpeed(speed)
	, m_bulletTargetVec(targetVec)
{
	m_pPlanet = CObjectManager::Instance()->getM_Planet();
	setPositionX((cos(CC_DEGREES_TO_RADIANS(angle)) * 700.f) + m_pPlanet->getPosition().x);
	setPositionY((sin(CC_DEGREES_TO_RADIANS(angle)) * 700.f) + m_pPlanet->getPosition().y);
	setRotation(-angle);
}

CBullet::~CBullet()
{
}

void* CBullet::operator new (size_t n)
{
	return const_cast<CTaskList*>(CObjectManager::Instance()->getM_BulletList())->New(n);
}

void CBullet::operator delete (void* p)
{
	const_cast<CTaskList*>(CObjectManager::Instance()->getM_BulletList())->Delete(p);
}

void CBullet::Execute(float delta)
{
	Vec2 dir = m_bulletTargetVec - getPosition();
	dir.normalize();
	dir *= (m_fBulletSpeed * delta);

	setPosition(getPosition() + dir);

	if (IsHit(const_cast<CPlanet*>(m_pPlanet)))
	{
		this->setAlive(false);
	}
	//m_fAngle += m_fAngleRate;
	//m_fBulletSpeed += m_fSpeedRate​​;
	//if (abs(getPositionX()) >= MaxX + Shape->Size || abs(getPositionY()) >= MaxY + Shape->Size) {
	//	setAlive(false);
	//}
}