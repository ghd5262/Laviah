#include "ItemManager.h"
#include "../Scene/GameScene.h"

CItemManager* CItemManager::Instance()
{
	static CItemManager instance;
	return &instance;
}

void CItemManager::StartItemTimer(eITEM_FLAG itemType)
{
	m_CurrentItems |= itemType;

	CGameScene::getGameScene()->scheduleOnce([this, itemType](float dt){
		this->FinishItemTimer(itemType);
	}, 20.0f, MakeString("Item_%d_Timer", static_cast<int>(itemType)));
}