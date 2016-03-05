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

	void CreateBulletList(size_t count, size_t size);	// size��ŭ�� char�� �����͸� count��ŭ Bullet����Ʈ�� add
	void CreateItemList(size_t count, size_t size);		// size��ŭ�� char�� �����͸� count��ŭ Item����Ʈ�� add
	void CreateEnemyList(size_t count, size_t size);	// size��ŭ�� char�� �����͸� count��ŭ Enemy����Ʈ�� add
	CBullet* BulletNew();									// List���� bullet������ ��ȯ ������ ������ ���� ����
	CEnemy* EnemyNew();									// List���� enemy������ ��ȯ ������ ������ ���� ����
	void ObjectDelete(CMover* object);					// Object �ʱ�ȭ (visible off, alive off)
	void EnemyDeleteAll();								// Enemy ��� �ʱ�ȭ (visible off, alive off)
	void BulletDeleteAll();								// Bullet ��� �ʱ�ȭ (visible off, alive off)
	void ExitGame();									// Game����� ����Ʈ �ʱ�ȭ �� ����

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

