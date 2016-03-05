#include "Enemy.h"
#include "ObjectManager.h"

CEnemy::CEnemy(float boundingRadius, float interval)
	: CMover(boundingRadius)
	, m_fInterval(interval)
	, m_fTime(0.f)
{
}

CEnemy::~CEnemy()
{
}