#include "PoolingManager.h"
#include "../GameObject/ObjectManager.h"

CPoolingManager::CPoolingManager()
{
}

CPoolingManager::~CPoolingManager()
{
	DeleteAllMemory();
}

/* 모든 메모리를 해제한다. (게임이 종료되거나 Scene이 변경될때 호출) */
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

/* size만큼의 메모리 블럭을 생성한다. */
CPoolingManager::MEMORYBLOCK CPoolingManager::NewMemoryBlock(size_t size) const
{
	/* memory alive를 위한 1바이트 추가 생성 */
	MEMORYBLOCK block = new char[size + 1];

	/* memory 초기화 및 memory alive = false */
	memset(block, 0, size + 1);
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
		/* 하나의 크기가 size만큼의 메모리 블럭을	count만큼 생성한다. */
		MEMORYBLOCK memBlock = NewMemoryBlock(m_BulletSize);
		m_BulletList.emplace_back(memBlock);

		/* 오브젝트 매니저 리스트에 추가한다. */
		CObjectManager::Instance()->AddBullet(memBlock);
	}
}

void CPoolingManager::CreateShooterList(size_t count, size_t size)
{
	m_ShooterSize = size;
	m_ShooterList.reserve(m_ShooterSize);
	while (count--)
	{
		/* 하나의 크기가 size만큼의 메모리 블럭을 count만큼 생성한다. */
		MEMORYBLOCK memBlock = NewMemoryBlock(m_ShooterSize);
		m_ShooterList.emplace_back(memBlock);

		/* 오브젝트 매니저 리스트에 추가한다. */
		CObjectManager::Instance()->AddShooter(memBlock);
	}
}

void* CPoolingManager::BulletNew()
{
	for (auto bullet : m_BulletList)
	{
		/* 메모리가 Free(false)상태면 메모리를 사용 중 상태(true)로 전환 후 반환 */
		if (false == bullet[m_BulletSize]) {
			bullet[m_BulletSize] = true;
			return bullet;
		}
	}

	/* 모든 메모리가 사용 중 상태라면 새롭게 하나 생성 */
	CCLOG("BULLET LIST OVERFLOWED");
	MEMORYBLOCK memBlock = NewMemoryBlock(m_BulletSize);
	m_BulletList.emplace_back(memBlock);

	/* 오브젝트 매니저 리스트에 추가한다. */
	CObjectManager::Instance()->AddBullet(memBlock);

	/* 메모리를 사용 중 상태로 전환 */
	memBlock[m_BulletSize] = true;

	return memBlock;
}

void* CPoolingManager::ShooterNew()
{
	for (auto shooter : m_ShooterList)
	{
		/* 메모리가 Free(false)상태면 메모리를 사용 중 상태(true)로 전환 후 반환 */
		if (false == shooter[m_ShooterSize]) {
			shooter[m_ShooterSize] = true;
			return shooter;
		}
	}

	/* 모든 메모리가 사용 중 상태라면 새롭게 하나 생성 */
	CCLOG("SHOOTER LIST OVERFLOWED");
	MEMORYBLOCK memBlock = NewMemoryBlock(m_ShooterSize);
	m_ShooterList.emplace_back(memBlock);

	/* 오브젝트 매니저 리스트에 추가한다. */
	CObjectManager::Instance()->AddShooter(memBlock);

	/* 메모리를 사용 중 상태로 전환 */
	memBlock[m_BulletSize] = true;

	return memBlock;
}

/* Bullet을 메모리블럭으로 전환 ( free 상태 ) */
void CPoolingManager::Bullet_ReturnToFreeMemory(void* bullet)
{
	//static_cast<char*>(bullet)[m_BulletSize] = false;
	memset(bullet, 0, m_BulletSize + 1);
}

/* shooter를 메모리블럭으로 전환 ( free 상태 ) */
void CPoolingManager::Shooter_ReturnToFreeMemory(void* shooter)
{
	//static_cast<char*>(shooter)[m_ShooterSize] = false;
	memset(shooter, 0, m_ShooterSize + 1);
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