#pragma once
#include <vector>
#include "../Common/HSHUtility.h"

//------------------------------PoolingManager Ŭ��������----------------------------------
//
// ������ Ư���� ���� ������ Enemy�� Bullet�� �����ϰ� �������ϴµ�
// �̰Ϳ� ���� ����ũ�� �޸� ����ȭ�� ���̱� ���� ObjectPool ������ Ŭ�����̴�.
// ���� ����Ŭ�� �Ʒ��� ����.
//
// 1. ���ϴ� ��ŭ�� �޸� �� ���� �� ����Ʈ�� �߰�(ObjectManager�� ����Ʈ���� �߰��ȴ�.)
// 2. �����ϰ��� �ϴ� ������Ʈ�� �ʱ�ȭ
// 3. ���
// 4. ��ȯ
// 5. 2������ �ٽ� ����
// 6. ���� ����� �޸� ����
//
// cocos2d������ ReferenceCount����� �Բ� �����Ǿ� �ټ� �����ϰ� �����Ǿ���.
// ������ PoolingManager���� ��� �޸� ���� ó������ CEnemy��ü�� �ʱ�ȭ�ϰ� 
// ������ ȣ�� �� addChild�� �ϴ� ������ ®������ �߰��� �ٸ� Enemy�� �ʱ�ȭ �� ����� �� ����
// ������ �����.
// ��, ������ ȣ���� �޸𸮸� ����ϴ� �������� ���� �̷�� ���� ������ �������� �̿����� ���ߴ�.
// �� �����ڸ� �� �� ȣ���ϰ� �Ǹ� Ref�� �����ڷ� ���� RefCount�� ����Ǵ� �� ���� ������ �־���.
// ������ ���� �����丵�� ���Ͽ� ������ ���ư��� �� ���̴�. 
//
//----------------------------------------------------------------------------------------

class CPoolingManager;

class CMemoryBlock
{
	friend CPoolingManager;

private:
	CMemoryBlock(char* buffer, bool isAlive);
	~CMemoryBlock();

	char* m_Buffer;
	bool m_IsAlive;
};



class CPoolingManager
{
public:
	static CPoolingManager* Instance();

	void CreateBulletList(size_t count, size_t size);	// size��ŭ�� char�� �����͸� count��ŭ Bullet����Ʈ�� add
	void CreateItemList(size_t count, size_t size);		// size��ŭ�� char�� �����͸� count��ŭ Item����Ʈ�� add
	void CreateEnemyList(size_t count, size_t size);	// size��ŭ�� char�� �����͸� count��ŭ Enemy����Ʈ�� add
	void* BulletNew();									// pool�� ������ �޸� ����Ʈ�� �����Ϸ��� �ͺ��� ������ ���� ����
	void* EnemyNew();									// pool�� ������ �޸� ����Ʈ�� �����Ϸ��� �ͺ��� ������ ���� ����
	void ObjectDelete(void* object);					// Object �ʱ�ȭ (alive off)
	void EnemyDeleteAll();								// Enemy ��� �ʱ�ȭ (alive off)
	void BulletDeleteAll();								// Bullet ��� �ʱ�ȭ (alive off)

private:
	CMemoryBlock* NewMemoryBlock(size_t size) const;	// size��ŭ�� �޸� ���� �����Ѵ�.
	CPoolingManager();
	~CPoolingManager();									// �޸� ���� �����Ѵ�.

private:
	std::vector<CMemoryBlock*> m_BulletList;
	std::vector<CMemoryBlock*> m_ItemList;
	std::vector<CMemoryBlock*> m_EnemyList;
	int m_BulletSize;
	int m_ItemSize;
	int m_EnemySize;
};

