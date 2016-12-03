#include "MenuSceneObjectManager.h"
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
	m_Planet = nullptr;
	m_SpaceShip = nullptr;
}

void CMenuSceneObjectManager::RemoveAllObject()
{
	Clear();
}

void CMenuSceneObjectManager::Execute(float delta)
{
	if (m_SpaceShip != nullptr)
		m_SpaceShip->Execute(delta);
}