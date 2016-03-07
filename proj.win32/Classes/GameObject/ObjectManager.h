#pragma once
#include <vector>

//------------------------------ObjectManager 클래스설명----------------------------------
//
// CMover를 상속받는 모든 클래스를 Execute및 Remove하는 함수이다.
// 현재 주의해야 할 사항은 Remove함수가 RemoveAllChildren을 호출하는 구조이다.
// 이유는 모든 CMover객채 및 파생 객채는 Pooling된 메모리를 참조하고 있으므로 
// 종료시 PoolingManager에서 메모리를 해제하기 때문에 
// 그전에 addChild된 메모리들을 해제해 주어야 하기 때문이다.
//
//----------------------------------------------------------------------------------------

class CEnemy;
class CBullet;
class CPlanet;
class CPlayer;

class CObjectManager
{
public:
	static CObjectManager* Instance();
	void AddBullet(void* bullet);		// Bullet과 Enemy를 따로 관리하는 이유는 Bullet이 생성되는 루틴이 Enemy에 있어서 
	void AddEnemy(void* enemy);			// 하나의 리스트에서 실행하면 해당 리스트가 순회되는 도중에 Enemy의 Execute를 실행함으로써 
	void RemoveAllObject();				// 순회 중에 리스트에 변형을 가져올 수 있기때문이다. (poolMng에 objectManager의 리스트에 추가하는 루틴이 존재한다.)
	void RemoveAllBullet();
	void RemoveAllEnemy();				// RemoveAllChildren함수 호출 이유는 구현부에~
	void Execute(float delta);
	
	//getter & setter
	CPlanet* getM_Planet(){ return m_Planet; }
	CPlayer* getM_Player(){ return m_Player; }
	void setM_Planet(CPlanet* planet){ m_Planet = planet; }
	void setM_Player(CPlayer* player){ m_Player = player; }
private:
	CObjectManager();
	~CObjectManager();

private:
	std::vector<CBullet*> m_BulletList;
	std::vector<CEnemy*> m_EnemyList;
	CPlanet* m_Planet;
	CPlayer* m_Player;
};

