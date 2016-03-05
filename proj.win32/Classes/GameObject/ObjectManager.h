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

	void CreateBulletList(size_t count);	// count��ŭ Bullet����Ʈ�� add
	void CreateItemList(size_t count);		// count��ŭ Item����Ʈ�� add
	void CreateEnemyList(size_t count);		// count��ŭ Enemy����Ʈ�� add
	CBullet* BulletNew();					// List���� bullet������ ��ȯ ������ ������ ���� ����
	CEnemy* EnemyNew();						// List���� enemy������ ��ȯ ������ ������ ���� ����
	void ObjectDelete(CMover* object);		// Object �ʱ�ȭ (visible off, alive off)
	void EnemyDeleteAll();					// Enemy ��� �ʱ�ȭ (visible off, alive off)
	void BulletDeleteAll();					// Bullet ��� �ʱ�ȭ (visible off, alive off)
	void ExitGame();						// Game����� ����Ʈ �ʱ�ȭ �� ����

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
	std::vector<CItem*> m_ItemList;
	std::vector<CEnemy*> m_EnemyList;
	CPlanet* m_Planet;
	CPlayer* m_Player;
};

