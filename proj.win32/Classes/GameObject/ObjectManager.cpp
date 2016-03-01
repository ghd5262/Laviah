#include "ObjectManager.h"
#include "Mover.h"
#include "Planet.h"
#include "Player.h"
#include "../Task/Task.h"

CObjectManager::CObjectManager()
{
}

CObjectManager::~CObjectManager()
{
}

CObjectManager* CObjectManager::Instance()
{
	static CObjectManager instance;
	return &instance;
}

void CObjectManager::Execute(float delta)
{
	for (CTaskIter i(m_BulletList); i.HasNext();) {
		CMover* bullet = static_cast<CMover*>(i.Next());
		bullet->Execute(delta);
		if (!bullet->IsAlive()) 
			i.Remove();
	}
	for (CTaskIter i(m_ItemList); i.HasNext();) {
		CMover* item = static_cast<CMover*>(i.Next());
		item->Execute(delta);
		if (!item->IsAlive()) 
			i.Remove();
	}
	for (CTaskIter i(m_EnemyList); i.HasNext();) {
		CMover* enemy = static_cast<CMover*>(i.Next());
		enemy->Execute(delta);
		if (!enemy->IsAlive()) 
			i.Remove();
	}
	m_Planet.get()->Execute();
	//m_Player->Execute();
}