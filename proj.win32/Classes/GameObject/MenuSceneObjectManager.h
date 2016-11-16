#pragma once

#include <vector>
#include "../Common/HSHUtility.h"

/*-----------------------MenuSceneObjectManager 클래스설명------------------------------
*
* MenuScene에서 동작하는 오브젝트들을 관리함.
* CMover를 상속받는 모든 클래스를 Execute및 Remove하는 함수이다.
* 현재 주의해야 할 사항은 Remove함수가 Delete함수를 호출하는 구조이다.
* 이유는 Delete에서 removeFromParent()와 operator delete를 호출하여 소멸자를 호출하기 위함
*
*-----------------------------------------------------------------------------------*/

class CAlien;
class CPlanet;
class CSpaceShip;
class CMenuSceneObjectManager
{
public:
	static CMenuSceneObjectManager* Instance();

	void AddAlien(void* alien);
	void RemoveAllObject();
	void Execute(float delta);

	void Clear();

protected:
	CC_SYNTHESIZE(CPlanet*, m_Planet, Planet);
	CC_SYNTHESIZE(CSpaceShip*, m_SpaceShip, SpaceShip);

private:
	void Auto_ReturnToMemoryBlock();
	void RemoveAllAlien();
	CMenuSceneObjectManager();
	~CMenuSceneObjectManager(){};

private:
	std::vector<CAlien*> m_AlienList;
};