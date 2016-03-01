#include "Enemy.h"
#include "ObjectManager.h"

CEnemy::CEnemy(std::string textureName, float boundingRadius, float interval, Vec2 targetVec)
	: CMover(CObjectManager::Instance()->getM_EnemyList(), textureName, boundingRadius)
	, m_fInterval(interval)
	, m_fTime(0.f)
	, m_TargetVec(targetVec)
{
}

CEnemy::~CEnemy()
{
}

void* CEnemy::operator new (size_t n)
{
	return const_cast<CTaskList*>(CObjectManager::Instance()->getM_EnemyList())->New(n);
}

void CEnemy::operator delete (void* p)
{
	const_cast<CTaskList*>(CObjectManager::Instance()->getM_EnemyList())->Delete(p);
}
