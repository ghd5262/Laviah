#include "MenuSceneObjectManager.h"
#include "Rocket.h"

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
	m_Rocket = nullptr;
}

void CMenuSceneObjectManager::RemoveAllObject()
{
	Clear();
}

void CMenuSceneObjectManager::Execute(float delta)
{
	if (m_Rocket != nullptr)
		m_Rocket->Execute(delta);
}