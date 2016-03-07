#include "PoolingManager.h"
#include "../GameObject/ObjectManager.h"
//-------------------------MEMORY BLOCK-------------------------

CMemoryBlock::CMemoryBlock(char* buffer, bool isAlive)
	: m_Buffer(buffer)
	, m_IsAlive(isAlive)
{}

CMemoryBlock::~CMemoryBlock()
{
	delete m_Buffer;
}

//-------------------------MEMORY BLOCK-------------------------

//------------------------Pooling Manager-----------------------
CPoolingManager::CPoolingManager()
{
}

CPoolingManager::~CPoolingManager()
{
	for (auto enemy : m_EnemyList)
	{
		delete enemy;
	}
}

CMemoryBlock* CPoolingManager::NewMemoryBlock(size_t size) const
{
	CMemoryBlock* block = new CMemoryBlock(new char[m_BulletSize], false);
	memset(block->m_Buffer, 0, size);
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
		CMemoryBlock* memBlock = NewMemoryBlock(m_BulletSize);
		m_BulletList.emplace_back(memBlock);
		CObjectManager::Instance()->AddBullet(memBlock->m_Buffer);
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
		CMemoryBlock* memBlock = NewMemoryBlock(m_EnemySize);
		m_EnemyList.emplace_back(memBlock);
		CObjectManager::Instance()->AddEnemy(memBlock->m_Buffer);
	}
}

void* CPoolingManager::BulletNew()
{
	for (auto bullet : m_BulletList)
	{
		if (!bullet->m_IsAlive) {
			bullet->m_IsAlive = true;
			return bullet->m_Buffer;
		}
	}
	//CCASSERT(false, "BULLET LIST OVERFLOWED");
	CMemoryBlock* memBlock = NewMemoryBlock(m_BulletSize);
	m_BulletList.emplace_back(memBlock);
	CObjectManager::Instance()->AddBullet(memBlock->m_Buffer);
	memBlock->m_IsAlive = true;

	return memBlock->m_Buffer;
}

void* CPoolingManager::EnemyNew()
{
	for (auto enemy : m_EnemyList)
	{
		if (!enemy->m_IsAlive) {
			enemy->m_IsAlive = true;
			return enemy->m_Buffer;
		}
	}
	CCASSERT(false, "ENEMY LIST OVERFLOWED");
	CMemoryBlock* memBlock = NewMemoryBlock(m_EnemySize);
	m_EnemyList.emplace_back(memBlock);
	CObjectManager::Instance()->AddEnemy(memBlock->m_Buffer);
	memBlock->m_IsAlive = true;

	return memBlock->m_Buffer;
}

void CPoolingManager::ObjectDelete(void* object)
{
	std::for_each(m_BulletList.begin(), m_BulletList.end(), [&](CMemoryBlock* temp){if (temp->m_Buffer == object)temp->m_IsAlive = false; });
	//static_cast<CMemoryBlock*>(object)->m_IsAlive = false;//
}

void CPoolingManager::EnemyDeleteAll()
{
	for (auto enemy : m_EnemyList)
	{
		enemy->m_IsAlive = false;
	}
}

void CPoolingManager::BulletDeleteAll()
{
	for (auto bullet : m_BulletList)
	{
		bullet->m_IsAlive = false;
	}
}

//------------------------Pooling Manager-----------------------