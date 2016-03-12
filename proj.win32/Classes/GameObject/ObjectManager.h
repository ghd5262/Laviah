#pragma once
#include <vector>

/*------------------------------ObjectManager 클래스설명----------------------------------
*
* CMover를 상속받는 모든 클래스를 Execute및 Remove하는 함수이다.
* 현재 주의해야 할 사항은 Remove함수가 RemoveAllChildren을 호출하는 구조이다.
* 이유는 모든 CMover객채 및 파생 객채는 Pooling된 메모리를 참조하고 있으므로 
* 종료시 PoolingManager에서 메모리를 해제하기 때문에 
* 그전에 addChild된 메모리들을 해제해 주어야 하기 때문이다.
*
*----------------------------------------------------------------------------------------*/

class CEnemy;
class CBullet;
class CPlanet;
class CPlayer;

class CObjectManager
{
public:
	static CObjectManager* Instance();

	/* AddBullet(), AddEnemy() => Bullet과 Enemy를 따로 관리하는 이유는
	첫째로 Bullet이 생성되는 루틴이 Enemy에 있다.
	때문에 하나의 리스트에서 둘 다 관리하면 리스트 순회 중 Bullet이 리스트에 추가될 수 있다.
	즉, 리스트를 사용하고 있는 중에 원하지 않는 변형을 가지고 올 수 있다 .*/
	void AddBullet(void* bullet);											
	void AddEnemy(void* enemy);			
	void RemoveAllObject();				// 게임 종료 시점에 호출된다. RemoveAllBullet(), RemoveAllEnemy() 호출함
	void Execute(float delta);
	
	//getter & setter
	CPlanet* getM_Planet(){ return m_Planet; }
	CPlayer* getM_Player(){ return m_Player; }
	void setM_Planet(CPlanet* planet){ m_Planet = planet; }
	void setM_Player(CPlayer* player){ m_Player = player; }

private:
	void RemoveAllBullet();				// RemoveAllChildren함수 호출! 이유는 구현부에~
	void RemoveAllEnemy();				// RemoveAllChildren함수 호출! 이유는 구현부에~
	CObjectManager();
	~CObjectManager();

private:
	std::vector<CBullet*> m_BulletList;
	std::vector<CEnemy*> m_EnemyList;
	CPlanet* m_Planet;
	CPlayer* m_Player;
};

