#pragma once
#include <vector>
#include "../Common/HSHUtility.h"


/*------------------------------ObjectManager Ŭ��������----------------------------------
*
* CMover�� ��ӹ޴� ��� Ŭ������ Execute�� Remove�ϴ� �Լ��̴�.
* ���� �����ؾ� �� ������ Remove�Լ��� Delete�Լ��� ȣ���ϴ� �����̴�.
* ������ Delete���� removeFromParent()�� operator delete�� ȣ���Ͽ� �Ҹ��ڸ� ȣ���ϱ� ����
*
*----------------------------------------------------------------------------------------*/

class CShooter;
class CBullet;
class CPlanet;
class CPlayer;

class CObjectManager
{
public:
	static CObjectManager* Instance();

	/* AddBullet(), AddShooter() => Bullet�� Shooter�� ���� �����ϴ� ������
	ù°�� Bullet�� �����Ǵ� ��ƾ�� Shooter�� �ִ�.
	������ �ϳ��� ����Ʈ���� �� �� �����ϸ� ����Ʈ ��ȸ �� Bullet�� ����Ʈ�� �߰��� �� �ִ�.
	��, ����Ʈ�� ����ϰ� �ִ� �߿� ������ �ʴ� ������ ������ �� �� �ִ�.
	
	���� Bullet���� ���������� Shooter���� �������� �ʴ� �Լ����� �ִ�.*/
	void AddBullet(void* bullet);											
	void AddShooter(void* shooter);			
	void RemoveAllObject();				// ���� ���� ������ ȣ��ȴ�. RemoveAllBullet(), RemoveAllShooter() ȣ����
	void Execute(float delta);

	//callback
	void RotationObject(int dir);

	//getter & setter
	CPlanet* getM_Planet(){ return m_Planet; }
	CPlayer* getM_Player(){ return m_Player; }
	void setM_Planet(CPlanet* planet){ m_Planet = planet; }
	void setM_Player(CPlayer* player){ m_Player = player; }

private:
	void Auto_ReturnToMemoryBlock()		// Alive�� false�� ������Ʈ�� ��� �޸� ������ �ǵ�����.
	void RemoveAllBullet();				// Delete�Լ� ȣ��! ������ �����ο�~
	void RemoveAllShooter();			// Delete�Լ� ȣ��! ������ �����ο�~
	CObjectManager(){};
	~CObjectManager(){};

private:
	std::vector<CBullet*> m_BulletList;
	std::vector<CShooter*> m_ShooterList;
	CPlanet* m_Planet;
	CPlayer* m_Player;
};

