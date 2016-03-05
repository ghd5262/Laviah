#pragma once
#include <vector>
class CMover;
class CPlanet;
class CPlayer;
class CBullet;
class CEnemy;
class CItem;

class CObjectManager
{
public:
	static CObjectManager* Instance();

	void CreateBulletList(size_t count, size_t size);	// size만큼의 char형 포인터를 count만큼 Bullet리스트에 add
	void CreateItemList(size_t count, size_t size);		// size만큼의 char형 포인터를 count만큼 Item리스트에 add
	void CreateEnemyList(size_t count, size_t size);	// size만큼의 char형 포인터를 count만큼 Enemy리스트에 add
	CBullet* BulletNew();									// List에서 bullet포인터 반환 사이즈 적으면 새로 생성
	CEnemy* EnemyNew();									// List에서 enemy포인터 반환 사이즈 적으면 새로 생성
	void ObjectDelete(CMover* object);					// Object 초기화 (visible off, alive off)
	void EnemyDeleteAll();								// Enemy 모두 초기화 (visible off, alive off)
	void BulletDeleteAll();								// Bullet 모두 초기화 (visible off, alive off)
	void ExitGame();									// Game종료시 리스트 초기화 및 삭제

	//getter & setter
	const CPlanet* getM_Planet(){ return m_Planet; }
	void setM_Planet(CPlanet* planet){ m_Planet = planet; }
	void setM_Player(CPlayer* player){ m_Player = player; }

	void Execute(float delta);
private:
	CObjectManager();
	~CObjectManager();

private:
	std::vector<CBullet*> m_BulletList;
	std::vector<char*> m_ItemList;
	std::vector<CEnemy*> m_EnemyList;
	CPlanet* m_Planet;
	CPlayer* m_Player;
	int m_BulletSize;
	int m_ItemSize;
	int m_EnemySize;
};

