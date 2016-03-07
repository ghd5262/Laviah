#include "ObjectManager.h"
#include "Mover.h"
#include "Planet.h"
#include "Player.h"
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

void CObjectManager::RemoveAllBullet()			
{												// bullet->removeAllChildren();	�� ����
	for (auto bullet : m_BulletList)			// ��� CMover��ä �� �Ļ� ��ä�� Pooling�� �޸𸮸� �����ϰ� �����Ƿ�
	{											// ����� PoolingManager���� �޸𸮸� �����ϱ� ������ 
		bullet->removeAllChildren();			// ������ addChild�� �޸𸮵��� ������ �־�� �ϱ� �����̴�.
	}
}

void CObjectManager::RemoveAllEnemy()
{
	for (auto enemy : m_EnemyList)			    // bullet->removeAllChildren();	�� ����
	{										    // ��� CMover��ä �� �Ļ� ��ä�� Pooling�� �޸𸮸� �����ϰ� �����Ƿ�
		enemy->removeAllChildren();			    // ����� PoolingManager���� �޸𸮸� �����ϱ� ������ 
	}										    // ������ addChild�� �޸𸮵��� ������ �־�� �ϱ� �����̴�.
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