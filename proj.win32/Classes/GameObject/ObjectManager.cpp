#include "ObjectManager.h"
#include "Mover.h"
#include "Planet.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

CObjectManager::CObjectManager()
{
}

CObjectManager::~CObjectManager()
{
}

CObjectManager* CObjectManager::Instance()
{
	static CObjectManager instance;
	return &instance;
}

void CObjectManager::ExitGame()
{
	for (auto enemy : m_EnemyList)
	{
		enemy->release();
	}
	for (auto bullet : m_BulletList)
	{
		bullet->release();
	}

	/*for (auto item : m_ItemList)
	{
	if (item->IsAlive()) {
	item->ex
	}
	}*/
	m_Planet->release();
	//m_Player->release();
}

void CObjectManager::CreateBulletList(size_t count)
{
	while (count--)
	{
		m_BulletList.emplace_back(::new CBullet());
	}
}

void CObjectManager::CreateItemList(size_t count)
{

}

void CObjectManager::CreateEnemyList(size_t count)
{
	while (count--)
	{
		m_EnemyList.emplace_back(::new CEnemy());
	}
}

CBullet* CObjectManager::BulletNew()
{
	for(auto bullet : m_BulletList)
	{
		if (!bullet->IsAlive()) { 
			bullet->setAlive(true);
			return bullet;
		}
	}
	//CCASSERT(false, "BULLET LIST OVERFLOWED");
	CBullet* tempBullet = ::new CBullet();
	m_BulletList.emplace_back(tempBullet);
	tempBullet->setAlive(true);
	return tempBullet;
}

CEnemy* CObjectManager::EnemyNew()
{
	for (auto enemy : m_EnemyList)
	{
		if (!enemy->IsAlive()) {
			enemy->setAlive(true);
			return enemy;
		}
	}
	CCASSERT(false, "ENEMY LIST OVERFLOWED");
	CEnemy* tempEnemy = ::new CEnemy();
	m_EnemyList.emplace_back(tempEnemy);
	tempEnemy->setAlive(true);
	return tempEnemy;
}

void CObjectManager::ObjectDelete(CMover* object)
{
	object->setVisible(false);
	object->setAlive(false);
}

void CObjectManager::EnemyDeleteAll()
{
	for (auto enemy : m_EnemyList)
	{
		enemy->setVisible(false);
		enemy->setAlive(false);
	}
}

void CObjectManager::BulletDeleteAll()
{
	for (auto bullet : m_BulletList)
	{
		bullet->setVisible(false);
		bullet->setAlive(false);
	}
}

void CObjectManager::Execute(float delta)
{
	for (auto enemy : m_EnemyList)
	{
		if (enemy->IsAlive()) {
			enemy->Execute(delta);
		}
	}
	for (auto bullet : m_BulletList)
	{
		if (bullet->IsAlive()) {
			bullet->Execute(delta);
		}
	}
	/*for (auto item : m_ItemList)
	{
		if (item->IsAlive()) {
			item->ex
		}
	}*/
	
	m_Planet->Execute();
	//m_Player->Execute();
}