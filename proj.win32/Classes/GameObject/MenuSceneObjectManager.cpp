#include "MenuSceneObjectManager.h"
#include "Alien.h"
#include "SpaceShip.h"

CMenuSceneObjectManager::CMenuSceneObjectManager()
{
	this->Clear();
}

CMenuSceneObjectManager* CMenuSceneObjectManager::Instance()
{
	static CMenuSceneObjectManager instance;
	return &instance;
}

void CMenuSceneObjectManager::Clear()
{
	m_AlienList.clear();
	m_Planet = nullptr;
    m_SpaceShip = nullptr;
}

void CMenuSceneObjectManager::AddAlien(void* alien)
{
	m_AlienList.emplace_back(static_cast<CAlien*>(alien));
}

/* bullet->Delete() :
* 게임 종료시 가지고 있는 Non_Node계열의 포인터를 해제하기위해 */
void CMenuSceneObjectManager::RemoveAllAlien()
{
	for (auto alien : m_AlienList)
	{
		if (alien->HasPointer())
			alien->Delete();
	}
}

void CMenuSceneObjectManager::Auto_ReturnToMemoryBlock()
{

}

void CMenuSceneObjectManager::RemoveAllObject()
{
	RemoveAllAlien();
	Clear();
}

void CMenuSceneObjectManager::Execute(float delta)
{
	for (auto alien : m_AlienList)
	{
		if (alien->IsAlive())
		{
			alien->Execute(delta);
		}
	}
    
    m_SpaceShip->Execute(delta);
}