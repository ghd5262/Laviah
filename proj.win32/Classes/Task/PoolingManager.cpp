#include "PoolingManager.h"
#include "../GameObject/ObjectManager.h"

CPoolingManager::CPoolingManager()
{
}

CPoolingManager::~CPoolingManager()
{
	for (auto enemy : m_EnemyList)
	{
		delete enemy;
	}

	for (auto bullet : m_BulletList)
	{
		delete bullet;
	}
	m_EnemyList.clear();
	m_BulletList.clear();
}

CPoolingManager::MEMORYBLOCK CPoolingManager::NewMemoryBlock(size_t size) const
{
	MEMORYBLOCK block = new char[size + 1]; // memory alive�� ���� 1����Ʈ �߰� ����
	memset(block, 0, size + 1);				// memory �ʱ�ȭ �� memory alive = false
	return block;
}

CPoolingManager* CPoolingManager::Instance()
{
	static CPoolingManager instance;
	return &instance;
}

void CPoolingManager::CreateBulletList(size_t count, size_t size)
{
	m_BulletSize = size;
	while (count--)
	{
		MEMORYBLOCK memBlock = NewMemoryBlock(m_BulletSize);
		m_BulletList.emplace_back(memBlock);
		CObjectManager::Instance()->AddBullet(memBlock);
	}
}

void CPoolingManager::CreateItemList(size_t count, size_t size)
{

}

void CPoolingManager::CreateEnemyList(size_t count, size_t size)
{
	m_EnemySize = size;
	while (count--)
	{
		MEMORYBLOCK memBlock = NewMemoryBlock(m_EnemySize);
		m_EnemyList.emplace_back(memBlock);
		CObjectManager::Instance()->AddEnemy(memBlock);
	}
}

void* CPoolingManager::BulletNew()
{
	for (auto bullet : m_BulletList)
	{
		if (false == bullet[m_BulletSize]) {	// �޸𸮰� Free���¸�
			bullet[m_BulletSize] = true;		// �޸𸮸� ��� ���·� ��ȯ �� ��ȯ
			return bullet;
		}
	}
	//CCASSERT(false, "BULLET LIST OVERFLOWED");
	MEMORYBLOCK memBlock = NewMemoryBlock(m_BulletSize);
	m_BulletList.emplace_back(memBlock);
	CObjectManager::Instance()->AddBullet(memBlock);
	memBlock[m_BulletSize] = true;	// �޸𸮸� ��� �� ���·� ��ȯ

	return memBlock;
}

void* CPoolingManager::EnemyNew()
{
	for (auto enemy : m_EnemyList)
	{
		if (false == enemy[m_EnemySize]) {
			enemy[m_EnemySize] = true;
			return enemy;
		}
	}
	CCASSERT(false, "ENEMY LIST OVERFLOWED");
	MEMORYBLOCK memBlock = NewMemoryBlock(m_EnemySize);
	m_EnemyList.emplace_back(memBlock);
	CObjectManager::Instance()->AddEnemy(memBlock);
	memBlock[m_BulletSize] = true;	// �޸𸮸� ��� �� ���·� ��ȯ

	return memBlock;
}

void CPoolingManager::Bullet_ReturnToFreeMemory(void* bullet)
{
	//static_cast<char*>(bullet)[m_BulletSize] = false;			// �޸𸮸� Free ���·� ��ȯ
	memset(bullet, 0, m_BulletSize + 1);				// memory �ʱ�ȭ �� memory alive = false
}

void CPoolingManager::Enemy_ReturnToFreeMemory(void* enemy)
{
	//static_cast<char*>(enemy)[m_EnemySize] = false;				// �޸𸮸� Free ���·� ��ȯ
	memset(enemy, 0, m_EnemySize + 1);				// memory �ʱ�ȭ �� memory alive = false
}

void CPoolingManager::Bullet_ReturnToFreeMemoryAll()
{
	for (auto bullet : m_BulletList)
	{
		bullet[m_BulletSize] = false;
	}
}

void CPoolingManager::Enemy_ReturnToFreeMemoryAll()
{
	for (auto enemy : m_EnemyList)
	{
		enemy[m_EnemySize] = false;
	}
}