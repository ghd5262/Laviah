#pragma once
#include "Planet.h"
class CTaskList;
class CPlayer;
class CObjectManager
{
public:
	static CObjectManager* Instance();

	//getter & setter
	const CTaskList* getM_BulletList(){ return m_BulletList; }
	const CTaskList* getM_ItemList(){ return m_ItemList; }
	const CTaskList* getM_EnemyList(){ return m_EnemyList; }
	const CPlanet* getM_Planet(){ return m_Planet.get(); }
	void setM_BulletList(CTaskList* tasklist){ m_BulletList = tasklist; }
	void setM_ItemList(CTaskList* tasklist){ m_ItemList = tasklist; }
	void setM_EnemyList(CTaskList* tasklist){ m_EnemyList = tasklist; }
	void setM_Planet(std::shared_ptr<CPlanet> planet){ m_Planet = planet; }
	void setM_Player(CPlayer* player){ m_Player = player; }

	void Execute(float delta);
private:
	CObjectManager();
	~CObjectManager();

private:
	CTaskList* m_BulletList;
	CTaskList* m_ItemList;
	CTaskList* m_EnemyList;
	std::shared_ptr<CPlanet> m_Planet;
	CPlayer* m_Player;
};

