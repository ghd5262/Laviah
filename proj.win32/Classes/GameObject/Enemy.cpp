#include "Enemy.h"
#include "ObjectManager.h"

CEnemy::~CEnemy()
{
}

void* CEnemy::operator new (size_t size, const std::nothrow_t)
{
	// ObjectManager에서 메모리를 할당 받는다.
	return CObjectManager::Instance()->EnemyNew();
}