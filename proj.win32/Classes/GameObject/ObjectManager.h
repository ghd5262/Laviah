#pragma once
#include <vector>
#include "../Common/HSHUtility.h"


/*------------------------------ObjectManager Ŭ��������----------------------------------
*
* CMover�� ��ӹ޴� ��� Ŭ������ Execute�� Remove�ϴ� �Լ��̴�.
* ���� �����ؾ� �� ������ Remove�Լ��� RemoveAllChildren�� ȣ���ϴ� �����̴�.
* ������ ��� CMover��ä �� �Ļ� ��ä�� Pooling�� �޸𸮸� �����ϰ� �����Ƿ� 
* ����� PoolingManager���� �޸𸮸� �����ϱ� ������ 
* ������ addChild�� �޸𸮵��� ������ �־�� �ϱ� �����̴�.
*
*----------------------------------------------------------------------------------------*/

class CEnemy;
class CBullet;
class CPlanet;
class CPlayer;
enum eITEM_FLAG;

class CObjectManager
{
public:
	static CObjectManager* Instance();

	/* AddBullet(), AddEnemy() => Bullet�� Enemy�� ���� �����ϴ� ������
	ù°�� Bullet�� �����Ǵ� ��ƾ�� Enemy�� �ִ�.
	������ �ϳ��� ����Ʈ���� �� �� �����ϸ� ����Ʈ ��ȸ �� Bullet�� ����Ʈ�� �߰��� �� �ִ�.
	��, ����Ʈ�� ����ϰ� �ִ� �߿� ������ �ʴ� ������ ������ �� �� �ִ� .*/
	void AddBullet(void* bullet);											
	void AddEnemy(void* enemy);			
	void RemoveAllObject();				// ���� ���� ������ ȣ��ȴ�. RemoveAllBullet(), RemoveAllEnemy() ȣ����
	void Execute(float delta);

	void PlayerGetItem(eITEM_FLAG itemType);

	//���� ����Ǵ� �������� �´��� �˻�
	int getCurrentItem(){ return m_CurrentItems; }

	//������ Ÿ�̸� ����
	void FinishItemTimer(eITEM_FLAG itemType){ m_CurrentItems ^= itemType; }

	//callback
	void RotationObject(int dir);

	//getter & setter
	CPlanet* getM_Planet(){ return m_Planet; }
	CPlayer* getM_Player(){ return m_Player; }
	void setM_Planet(CPlanet* planet){ m_Planet = planet; }
	void setM_Player(CPlayer* player){ m_Player = player; }

private:
	void RemoveAllBullet();				// RemoveAllChildren�Լ� ȣ��! ������ �����ο�~
	void RemoveAllEnemy();				// RemoveAllChildren�Լ� ȣ��! ������ �����ο�~
	CObjectManager();
	~CObjectManager();

private:
	std::vector<CBullet*> m_BulletList;
	std::vector<CEnemy*> m_EnemyList;
	CPlanet* m_Planet;
	CPlayer* m_Player;

	// ���� �÷��̾ ȹ���Ͽ� ����ǰ� �ִ� ������
	int m_CurrentItems;
};

