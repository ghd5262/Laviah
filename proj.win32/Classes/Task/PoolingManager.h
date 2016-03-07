#pragma once
#include <vector>
#include "../Common/HSHUtility.h"

//------------------------------PoolingManager 클래스설명----------------------------------
//
// 게임의 특성상 여러 종류의 Enemy와 Bullet을 생성하고 지워야하는데
// 이것에 대한 리스크와 메모리 단편화를 줄이기 위한 ObjectPool 개념의 클래스이다.
// 동작 사이클은 아래와 같다.
//
// 1. 원하는 만큼의 메모리 블럭 생성 및 리스트에 추가(ObjectManager의 리스트에도 추가된다.)
// 2. 생성하고자 하는 오브젝트로 초기화
// 3. 사용
// 4. 반환
// 5. 2번부터 다시 시작
// 6. 게임 종료시 메모리 해제
//
// cocos2d엔진의 ReferenceCount개념과 함께 구현되어 다소 복잡하게 구현되었다.
// 이전에 PoolingManager에서 모든 메모리 블럭을 처음부터 CEnemy객체로 초기화하고 
// 생성자 호출 및 addChild를 하는 구조로 짰을때는 중간에 다른 Enemy로 초기화 및 사용할 수 없는
// 문제가 생겼다.
// 즉, 생성자 호출이 메모리를 사용하는 시점보다 먼저 이루어 졌기 때문에 다형성을 이용하지 못했다.
// 또 생성자를 두 번 호출하게 되면 Ref의 생성자로 인해 RefCount가 변경되는 등 여러 문제가 있었다.
// 앞으로 많은 리펙토링을 통하여 수정해 나아가야 할 것이다. 
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

	void CreateBulletList(size_t count, size_t size);	// size만큼의 char형 포인터를 count만큼 Bullet리스트에 add
	void CreateItemList(size_t count, size_t size);		// size만큼의 char형 포인터를 count만큼 Item리스트에 add
	void CreateEnemyList(size_t count, size_t size);	// size만큼의 char형 포인터를 count만큼 Enemy리스트에 add
	void* BulletNew();									// pool이 보유한 메모리 리스트가 생성하려는 것보다 적으면 새로 생성
	void* EnemyNew();									// pool이 보유한 메모리 리스트가 생성하려는 것보다 적으면 새로 생성
	void ObjectDelete(void* object);					// Object 초기화 (alive off)
	void EnemyDeleteAll();								// Enemy 모두 초기화 (alive off)
	void BulletDeleteAll();								// Bullet 모두 초기화 (alive off)

private:
	CMemoryBlock* NewMemoryBlock(size_t size) const;	// size만큼의 메모리 블럭을 생성한다.
	CPoolingManager();
	~CPoolingManager();									// 메모리 블럭을 해제한다.

private:
	std::vector<CMemoryBlock*> m_BulletList;
	std::vector<CMemoryBlock*> m_ItemList;
	std::vector<CMemoryBlock*> m_EnemyList;
	int m_BulletSize;
	int m_ItemSize;
	int m_EnemySize;
};

