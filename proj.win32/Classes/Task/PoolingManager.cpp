#include "PoolingManager.h"
#include "../GameObject/ObjectManager.h"

CPoolingManager::CPoolingManager()
{
}

CPoolingManager::~CPoolingManager()
{
	DeleteAllMemory();
}

void CPoolingManager::DeleteAllMemory()
{
	for (auto shooter : m_ShooterList)
	{
		delete[] shooter;
		shooter = nullptr;
	}

	for (auto bullet : m_BulletList)
	{
		delete[] bullet;
		bullet = nullptr;
	}
	m_ShooterList.clear();
	m_BulletList.clear();
}

CPoolingManager::MEMORYBLOCK CPoolingManager::NewMemoryBlock(size_t size) const
{
	MEMORYBLOCK block = new char[size + 1]; // memory alive를 위한 1바이트 추가 생성
	memset(block, 0, size + 1);				// memory 초기화 및 memory alive = false
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
	m_BulletList.reserve(m_BulletSize);
	while (count--)
	{
		MEMORYBLOCK memBlock = NewMemoryBlock(m_BulletSize);	// 하나의 크기가 size만큼의 메모리 블럭을
		m_BulletList.emplace_back(memBlock);					// count만큼 생성한다.
		CObjectManager::Instance()->AddBullet(memBlock);
	}
}

void CPoolingManager::CreateShooterList(size_t count, size_t size)
{
	m_ShooterSize = size;
	m_ShooterList.reserve(m_ShooterSize);
	while (count--)
	{
		MEMORYBLOCK memBlock = NewMemoryBlock(m_ShooterSize);		// 하나의 크기가 size만큼의 메모리 블럭을 
		m_ShooterList.emplace_back(memBlock);						// count만큼 생성한다.
		CObjectManager::Instance()->AddShooter(memBlock);
	}
}

void* CPoolingManager::BulletNew()
{
	for (auto bullet : m_BulletList)
	{
		if (false == bullet[m_BulletSize]) {	// 메모리가 Free상태면
			bullet[m_BulletSize] = true;		// 메모리를 사용 상태로 전환 후 반환
			return bullet;
		}
	}
	CCLOG("BULLET LIST OVERFLOWED");
	MEMORYBLOCK memBlock = NewMemoryBlock(m_BulletSize);
	m_BulletList.emplace_back(memBlock);
	CObjectManager::Instance()->AddBullet(memBlock);
	memBlock[m_BulletSize] = true;	// 메모리를 사용 중 상태로 전환

	return memBlock;
}

void* CPoolingManager::ShooterNew()
{
	for (auto shooter : m_ShooterList)
	{
		if (false == shooter[m_ShooterSize]) {
			shooter[m_ShooterSize] = true;
			return shooter;
		}
	}
	CCLOG("SHOOTER LIST OVERFLOWED");
	MEMORYBLOCK memBlock = NewMemoryBlock(m_ShooterSize);
	m_ShooterList.emplace_back(memBlock);
	CObjectManager::Instance()->AddShooter(memBlock);
	memBlock[m_BulletSize] = true;	// 메모리를 사용 중 상태로 전환

	return memBlock;
}

void CPoolingManager::Bullet_ReturnToFreeMemory(void* bullet)
{
	static_cast<char*>(bullet)[m_BulletSize] = false;

	//memset(bullet, 0, m_BulletSize + 1);			// memory 초기화 및 memory alive = false
}

void CPoolingManager::Shooter_ReturnToFreeMemory(void* shooter)
{
	static_cast<char*>(shooter)[m_ShooterSize] = false;
	//memset(shooter, 0, m_ShooterSize + 1);				// memory 초기화 및 memory alive = false
}

void CPoolingManager::Bullet_ReturnToFreeMemoryAll()
{
	for (auto bullet : m_BulletList)
	{
		bullet[m_BulletSize] = false;
		//memset(bullet, 0, m_BulletSize + 1);
	}
}

void CPoolingManager::Shooter_ReturnToFreeMemoryAll()
{
	for (auto shooter : m_ShooterList)
	{
		shooter[m_ShooterSize] = false;
		//memset(shooter, 0, m_ShooterSize + 1);
	}
}