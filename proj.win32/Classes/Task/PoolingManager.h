#pragma once
#include <vector>
#include "../Common/HSHUtility.h"

/*-----------------------CPoolingManager------------------------
 *
 * 게임의 특성상 여러 종류의 Shooter와 Bullet을 생성하고 지워야하는데
 * 이것에 대한 리스크와 메모리 단편화를 줄이기 위한 ObjectPool 개념의 클래스이다.
 * 동작 사이클은 아래와 같다.
 *
 * 1. CreateBulletList() - 원하는 만큼의 메모리 블럭 생성 및 리스트에 추가
 * 2. BulletNew() - 생성하고자 하는 오브젝트로 초기화(operator new, 생성자 호출)
 * 3. 사용
 * 4. ReturnToFreeMemory() - 메모리 반환
 * 5. 사용할 때 마다 2번부터 반복
 * 6. 게임 종료시 메모리 해제
 *
----------------------------------------------------------------*/

class CPoolingManager
{

	/* 메모리 블럭 타입 정의	*/
	typedef char* MEMORYBLOCK;

public:
	static CPoolingManager* Instance();

	/* size만큼의 char형 포인터를 count만큼 Bullet리스트에 add */
	void CreateBulletList(size_t count, size_t size);
	void CreateShooterList(size_t count, size_t size);


	/* pool이 보유한 메모리 리스트가 생성하려는 것보다 적으면 새로 생성 */
	void* BulletNew();
	void* ShooterNew();


	/* Bullet을 메모리블럭으로 전환 (alive off) */
	void Bullet_ReturnToFreeMemory(void* bullet);
	void Shooter_ReturnToFreeMemory(void* shooter);


	/* 모든 Bullet을 메모리 블럭으로 전환 (alive off) */
	void Bullet_ReturnToFreeMemoryAll();
	void Shooter_ReturnToFreeMemoryAll();


	/* 모든 메모리를 해제한다. (게임이 종료되거나 Scene이 변경될때 호출) */
	void DeleteAllMemory();


private:
	/* size만큼의 메모리 블럭을 생성한다. */
	MEMORYBLOCK NewMemoryBlock(size_t size) const;


	CPoolingManager();
	/* 메모리 블럭을 모두 해제한다. */
	~CPoolingManager();


private:
	/* 메모리 블럭 리스트 */
	std::vector<MEMORYBLOCK> m_BulletList;
	std::vector<MEMORYBLOCK> m_ShooterList;

	/* 메모리 블럭 하나 당 크기 */
	size_t m_BulletSize;
	size_t m_ShooterSize;
};

