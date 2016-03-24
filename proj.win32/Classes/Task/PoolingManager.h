#pragma once
#include <vector>
#include "../Common/HSHUtility.h"

/*------------------------------PoolingManager Ŭ��������----------------------------------
*
* ������ Ư���� ���� ������ Shooter�� Bullet�� �����ϰ� �������ϴµ�
* �̰Ϳ� ���� ����ũ�� �޸� ����ȭ�� ���̱� ���� ObjectPool ������ Ŭ�����̴�.
* ���� ����Ŭ�� �Ʒ��� ����.
*
* 1. ���ϴ� ��ŭ�� �޸� �� ���� �� ����Ʈ�� �߰�(ObjectManager�� ����Ʈ���� �߰��ȴ�. ������ ObjectManager���� �����ߴ�.)
* 2. �����ϰ��� �ϴ� ������Ʈ�� �ʱ�ȭ
* 3. ���
* 4. ��ȯ
* 5. ����� �� ���� 2������ �ݺ� 
* 6. ���� ����� �޸� ����
*
* cocos2d������ ReferenceCount����� �Բ� �����Ǿ� �ټ� �����ϰ� �����Ǿ���.
* ������ PoolingManager���� ��� �޸� ���� ó������ Shooter��ü�� �ʱ�ȭ�ϰ� 
* ������ ȣ�� �� addChild�� �ϴ� ������ ®������ �߰��� �ٸ� Shooter�� �ʱ�ȭ �� ����� �� ����
* ������ �����.
* ��, ������ ȣ���� �޸𸮸� ����ϴ� �������� ���� �̷�� ���� ������ �������� �̿����� ���ߴ�.
* �� �����ڸ� �� �� ȣ���ϰ� �Ǹ� Ref�� �����ڷ� ���� RefCount�� ����Ǵ� �� ���� ������ �־���.
* ������ ���� �����丵�� ���Ͽ� ������ ���ư��� �� ���̴�. 
*
*----------------------------------------------------------------------------------------*/

class CPoolingManager
{
	typedef char* MEMORYBLOCK;

public:
	static CPoolingManager* Instance();

	void CreateBulletList(size_t count, size_t size);	// size��ŭ�� char�� �����͸� count��ŭ Bullet����Ʈ�� add
	void CreateShooterList(size_t count, size_t size);	// size��ŭ�� char�� �����͸� count��ŭ Shooter����Ʈ�� add
	void* BulletNew();									// pool�� ������ �޸� ����Ʈ�� �����Ϸ��� �ͺ��� ������ ���� ����
	void* ShooterNew();									// pool�� ������ �޸� ����Ʈ�� �����Ϸ��� �ͺ��� ������ ���� ����
	void Bullet_ReturnToFreeMemory(void* bullet);		// Bullet�� �޸𸮺����� ��ȯ (alive off)
	void Shooter_ReturnToFreeMemory(void* shooter);		// Shooter�� �޸𸮺����� ��ȯ (alive off)
	void Bullet_ReturnToFreeMemoryAll();				// ��� Bullet�� �޸� ������ ��ȯ (alive off)
	void Shooter_ReturnToFreeMemoryAll();				// ��� SHooter�� �޸� ������ ��ȯ (alive off)
	void DeleteAllMemory();								// ��� �޸𸮸� �����Ѵ�. (������ ����ǰų� Scene�� ����ɶ� ȣ��)

private:
	MEMORYBLOCK NewMemoryBlock(size_t size) const;		// size��ŭ�� �޸� ���� �����Ѵ�.
	CPoolingManager();
	~CPoolingManager();									// �޸� ���� �����Ѵ�.

private:
	std::vector<MEMORYBLOCK> m_BulletList;
	std::vector<MEMORYBLOCK> m_ShooterList;
	int m_BulletSize;
	int m_ShooterSize;
};

