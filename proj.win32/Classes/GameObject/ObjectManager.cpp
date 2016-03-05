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
		if (!enemy->IsAlive())
			enemy->release();
	}
	m_EnemyList.clear();
	for (auto bullet : m_BulletList)
	{
		bullet->release();
	}
	m_BulletList.clear();
	/*for (auto item : m_ItemList)
	{
	if (item->IsAlive()) {
	item->ex
	}
	}*/
	//m_Planet->release();
	//m_Player->release();
}

void CObjectManager::CreateBulletList(size_t count, size_t size)
{
	m_BulletSize = size;
	while (count--)
	{
		CBullet* temp = (CBullet*)new char[m_BulletSize];
		temp->setAlive(false);
		m_BulletList.emplace_back(temp);
	}
}

void CObjectManager::CreateItemList(size_t count, size_t size)
{

}

void CObjectManager::CreateEnemyList(size_t count, size_t size)
{
	m_EnemySize = size;
	while (count--)
	{
		CEnemy* temp = (CEnemy*)new char[m_EnemySize];
		temp->setAlive(false);
		m_EnemyList.emplace_back(temp);
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
	CBullet* tempBullet = (CBullet*)new char[m_BulletSize];
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
	CEnemy* tempEnemy = (CEnemy*)new char[m_BulletSize];
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