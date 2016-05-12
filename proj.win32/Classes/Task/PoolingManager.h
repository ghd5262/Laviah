#pragma once
#include <vector>
#include "../Common/HSHUtility.h"

/*-----------------------CPoolingManager------------------------
 *
 * ������ Ư���� ���� ������ Shooter�� Bullet�� �����ϰ� �������ϴµ�
 * �̰Ϳ� ���� ����ũ�� �޸� ����ȭ�� ���̱� ���� ObjectPool ������ Ŭ�����̴�.
 * ���� ����Ŭ�� �Ʒ��� ����.
 *
 * 1. CreateBulletList() - ���ϴ� ��ŭ�� �޸� �� ���� �� ����Ʈ�� �߰�
 * 2. BulletNew() - �����ϰ��� �ϴ� ������Ʈ�� �ʱ�ȭ(operator new, ������ ȣ��)
 * 3. ���
 * 4. ReturnToFreeMemory() - �޸� ��ȯ
 * 5. ����� �� ���� 2������ �ݺ�
 * 6. ���� ����� �޸� ����
 *
----------------------------------------------------------------*/

class CPoolingManager
{

	/* �޸� �� Ÿ�� ����	*/
	typedef char* MEMORYBLOCK;

public:
	static CPoolingManager* Instance();

	/* size��ŭ�� char�� �����͸� count��ŭ Bullet����Ʈ�� add */
	void CreateBulletList(size_t count, size_t size);
	void CreateShooterList(size_t count, size_t size);


	/* pool�� ������ �޸� ����Ʈ�� �����Ϸ��� �ͺ��� ������ ���� ���� */
	void* BulletNew();
	void* ShooterNew();


	/* Bullet�� �޸𸮺����� ��ȯ (alive off) */
	void Bullet_ReturnToFreeMemory(void* bullet);
	void Shooter_ReturnToFreeMemory(void* shooter);


	/* ��� Bullet�� �޸� ������ ��ȯ (alive off) */
	void Bullet_ReturnToFreeMemoryAll();
	void Shooter_ReturnToFreeMemoryAll();


	/* ��� �޸𸮸� �����Ѵ�. (������ ����ǰų� Scene�� ����ɶ� ȣ��) */
	void DeleteAllMemory();


private:
	/* size��ŭ�� �޸� ���� �����Ѵ�. */
	MEMORYBLOCK NewMemoryBlock(size_t size) const;


	CPoolingManager();
	/* �޸� ���� ��� �����Ѵ�. */
	~CPoolingManager();


private:
	/* �޸� �� ����Ʈ */
	std::vector<MEMORYBLOCK> m_BulletList;
	std::vector<MEMORYBLOCK> m_ShooterList;

	/* �޸� �� �ϳ� �� ũ�� */
	size_t m_BulletSize;
	size_t m_ShooterSize;
};

