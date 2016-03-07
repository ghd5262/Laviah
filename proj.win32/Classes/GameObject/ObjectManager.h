#pragma once
#include <vector>

//------------------------------ObjectManager Ŭ��������----------------------------------
//
// CMover�� ��ӹ޴� ��� Ŭ������ Execute�� Remove�ϴ� �Լ��̴�.
// ���� �����ؾ� �� ������ Remove�Լ��� RemoveAllChildren�� ȣ���ϴ� �����̴�.
// ������ ��� CMover��ä �� �Ļ� ��ä�� Pooling�� �޸𸮸� �����ϰ� �����Ƿ� 
// ����� PoolingManager���� �޸𸮸� �����ϱ� ������ 
// ������ addChild�� �޸𸮵��� ������ �־�� �ϱ� �����̴�.
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
	void AddBullet(void* bullet);		// Bullet�� Enemy�� ���� �����ϴ� ������ Bullet�� �����Ǵ� ��ƾ�� Enemy�� �־ 
	void AddEnemy(void* enemy);			// �ϳ��� ����Ʈ���� �����ϸ� �ش� ����Ʈ�� ��ȸ�Ǵ� ���߿� Enemy�� Execute�� ���������ν� 
	void RemoveAllObject();				// ��ȸ �߿� ����Ʈ�� ������ ������ �� �ֱ⶧���̴�. (poolMng�� objectManager�� ����Ʈ�� �߰��ϴ� ��ƾ�� �����Ѵ�.)
	void RemoveAllBullet();
	void RemoveAllEnemy();				// RemoveAllChildren�Լ� ȣ�� ������ �����ο�~
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

