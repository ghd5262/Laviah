#include "PoolingManager.h"
#include "../GameObject/ObjectManager.h"

CPoolingManager::CPoolingManager()
{
}

CPoolingManager::~CPoolingManager()
{
	DeleteAllMemory();
}

/* ��� �޸𸮸� �����Ѵ�. (������ ����ǰų� Scene�� ����ɶ� ȣ��) */
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

/* size��ŭ�� �޸� ���� �����Ѵ�. */
CPoolingManager::MEMORYBLOCK CPoolingManager::NewMemoryBlock(size_t size) const
{
	/* memory alive�� ���� 1����Ʈ �߰� ���� */
	MEMORYBLOCK block = new char[size + 1];

	/* memory �ʱ�ȭ �� memory alive = false */
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
		/* �ϳ��� ũ�Ⱑ size��ŭ�� �޸� ����	count��ŭ �����Ѵ�. */
		MEMORYBLOCK memBlock = NewMemoryBlock(m_BulletSize);
		m_BulletList.emplace_back(memBlock);

		/* ������Ʈ �Ŵ��� ����Ʈ�� �߰��Ѵ�. */
		CObjectManager::Instance()->AddBullet(memBlock);
	}
}

void CPoolingManager::CreateShooterList(size_t count, size_t size)
{
	m_ShooterSize = size;
	m_ShooterList.reserve(m_ShooterSize);
	while (count--)
	{
		/* �ϳ��� ũ�Ⱑ size��ŭ�� �޸� ���� count��ŭ �����Ѵ�. */
		MEMORYBLOCK memBlock = NewMemoryBlock(m_ShooterSize);
		m_ShooterList.emplace_back(memBlock);

		/* ������Ʈ �Ŵ��� ����Ʈ�� �߰��Ѵ�. */
		CObjectManager::Instance()->AddShooter(memBlock);
	}
}

void* CPoolingManager::BulletNew()
{
	for (auto bullet : m_BulletList)
	{
		/* �޸𸮰� Free(false)���¸� �޸𸮸� ��� �� ����(true)�� ��ȯ �� ��ȯ */
		if (false == bullet[m_BulletSize]) {
			bullet[m_BulletSize] = true;
			return bullet;
		}
	}

	/* ��� �޸𸮰� ��� �� ���¶�� ���Ӱ� �ϳ� ���� */
	CCLOG("BULLET LIST OVERFLOWED");
	MEMORYBLOCK memBlock = NewMemoryBlock(m_BulletSize);
	m_BulletList.emplace_back(memBlock);

	/* ������Ʈ �Ŵ��� ����Ʈ�� �߰��Ѵ�. */
	CObjectManager::Instance()->AddBullet(memBlock);

	/* �޸𸮸� ��� �� ���·� ��ȯ */
	memBlock[m_BulletSize] = true;

	return memBlock;
}

void* CPoolingManager::ShooterNew()
{
	for (auto shooter : m_ShooterList)
	{
		/* �޸𸮰� Free(false)���¸� �޸𸮸� ��� �� ����(true)�� ��ȯ �� ��ȯ */
		if (false == shooter[m_ShooterSize]) {
			shooter[m_ShooterSize] = true;
			return shooter;
		}
	}

	/* ��� �޸𸮰� ��� �� ���¶�� ���Ӱ� �ϳ� ���� */
	CCLOG("SHOOTER LIST OVERFLOWED");
	MEMORYBLOCK memBlock = NewMemoryBlock(m_ShooterSize);
	m_ShooterList.emplace_back(memBlock);

	/* ������Ʈ �Ŵ��� ����Ʈ�� �߰��Ѵ�. */
	CObjectManager::Instance()->AddShooter(memBlock);

	/* �޸𸮸� ��� �� ���·� ��ȯ */
	memBlock[m_BulletSize] = true;

	return memBlock;
}

/* Bullet�� �޸𸮺����� ��ȯ ( free ���� ) */
void CPoolingManager::Bullet_ReturnToFreeMemory(void* bullet)
{
	//static_cast<char*>(bullet)[m_BulletSize] = false;
	memset(bullet, 0, m_BulletSize + 1);
}

/* shooter�� �޸𸮺����� ��ȯ ( free ���� ) */
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