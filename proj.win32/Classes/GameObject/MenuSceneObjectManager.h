#pragma once

#include <vector>
#include "../Common/HSHUtility.h"

/*-----------------------MenuSceneObjectManager Ŭ��������------------------------------
*
* MenuScene���� �����ϴ� ������Ʈ���� ������.
* CMover�� ��ӹ޴� ��� Ŭ������ Execute�� Remove�ϴ� �Լ��̴�.
* ���� �����ؾ� �� ������ Remove�Լ��� Delete�Լ��� ȣ���ϴ� �����̴�.
* ������ Delete���� removeFromParent()�� operator delete�� ȣ���Ͽ� �Ҹ��ڸ� ȣ���ϱ� ����
*
*-----------------------------------------------------------------------------------*/

class CAlien;
class CPlanet;

class CMenuSceneObjectManager
{
public:
	static CMenuSceneObjectManager* Instance();

	void AddAlien(void* alien);
	void RemoveAllObject();
	void Execute(float delta);

	// �ʱ�ȭ
	void Clear();

protected:
	CC_SYNTHESIZE(CPlanet*, m_Planet, Planet);

private:
	void Auto_ReturnToMemoryBlock();	// Alive�� false�� ������Ʈ�� ��� �޸� ������ �ǵ�����.
	void RemoveAllAlien();				// Delete�Լ� ȣ��! ������ �����ο�~
	CMenuSceneObjectManager();
	~CMenuSceneObjectManager(){};

private:
	std::vector<CAlien*> m_AlienList;
};