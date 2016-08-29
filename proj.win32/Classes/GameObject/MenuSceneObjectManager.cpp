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
* ���� ����� ������ �ִ� Non_Node�迭�� �����͸� �����ϱ����� */
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
	if (m_SpaceShip != nullptr)
		m_SpaceShip->Execute(delta);
}