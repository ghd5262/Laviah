#include "ObjectManager.h"
#include "Mover.h"
#include "Planet.h"
#include "Player.h"
#include "Bullet.h"
#include "Shooter/Shooter.h"
#include "../Scene/GameScene.h"

CObjectManager::CObjectManager(){}

CObjectManager::~CObjectManager(){}

CObjectManager* CObjectManager::Instance()
{
	static CObjectManager instance;
	return &instance;
}

void CObjectManager::AddBullet(void* bullet)
{
	m_BulletList.emplace_back(static_cast<CBullet*>(bullet));
}

void CObjectManager::AddEnemy(void* enemy)
{
	m_EnemyList.emplace_back(static_cast<CEnemy*>(enemy));
}


/* bullet->removeAllChildren();	의 이유 :
모든 CMover객채 및 파생 객채는 Pooling된 메모리를 참조하고 있으며
게임 종료 시 해당 메모리는 PoolingManager에서 해제한다.
때문에 그전에 addChild된 메모리들을 해제해 주어야 하기 때문이다. */
void CObjectManager::RemoveAllBullet()
{											
	for (auto bullet : m_BulletList)
	{
		if (bullet->HasChild())
			bullet->removeAllChildren();
	}
}

/* enemy->removeAllChildren();	의 이유 :
모든 CMover객채 및 파생 객채는 Pooling된 메모리를 참조하고 있으며
게임 종료 시 해당 메모리는 PoolingManager에서 해제한다.
때문에 그전에 addChild된 메모리들을 해제해 주어야 하기 때문이다. */
void CObjectManager::RemoveAllEnemy()
{
	for (auto enemy : m_EnemyList)
	{						
		if (enemy->HasChild())
			enemy->removeAllChildren(); 
	}
}

void CObjectManager::RemoveAllObject()
{
	RemoveAllBullet();
	RemoveAllEnemy();
}

void CObjectManager::Execute(float delta)
{
	for (auto bullet : m_BulletList)
	{
		if (bullet->IsAlive()) {
			bullet->Execute(delta);
		}
	}
	
	for (auto enemy : m_EnemyList)
	{
		if (enemy->IsAlive()) {
			enemy->Execute(delta);
		}
	}

	//m_Planet->Execute();
	//m_Player->Execute();
}