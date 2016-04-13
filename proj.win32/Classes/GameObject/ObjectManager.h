#pragma once
#include <vector>
#include "../Common/HSHUtility.h"
#include "../AI/StateMachine.h"
#include "../GameObject/Shooter/ShooterHeaders.h"

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


	void ShooterPause();
    void ShooterResume();

	//callback
	void RotationObject(float dir);

	//getter & setter
	CC_SYNTHESIZE(CPlanet*, m_Planet, Planet);
	CC_SYNTHESIZE(CPlayer*, m_Player, Player);
	CC_SYNTHESIZE(CStateMachine<CObjectManager>*, m_FSM, FSM);
	CC_SYNTHESIZE(float, m_fStageTime, StageTime);
	CC_SYNTHESIZE(float, m_fDelta, Delta);
private:
	void CreateShooterByTimer();
	void Auto_ReturnToMemoryBlock();	// Alive�� false�� ������Ʈ�� ��� �޸� ������ �ǵ�����.
	void RemoveAllBullet();				// Delete�Լ� ȣ��! ������ �����ο�~
	void RemoveAllShooter();			// Delete�Լ� ȣ��! ������ �����ο�~
	CObjectManager();
	~CObjectManager(){};

private:
	std::vector<CBullet*> m_BulletList;
	std::vector<CShooter*> m_ShooterList;
	const std::vector<sSHOOTER_PARAM>* m_StageList;
	int m_CurrentShooterIdx;
};