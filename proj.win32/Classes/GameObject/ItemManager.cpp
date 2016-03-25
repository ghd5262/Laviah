#include "ItemManager.h"
#include "../Scene/GameScene.h"

CItemManager::CItemManager()
{
	// ±âº» °ª
	m_ValueOfCoin[eCOIN_TYPE_bronze]	= 10.f;
	m_ValueOfCoin[eCOIN_TYPE_silver]	= 20.f;
	m_ValueOfCoin[eCOIN_TYPE_gold]		= 30.f;
	m_ValueOfCoin[eCOIN_TYPE_bigSilver] = 50.f;
	m_ValueOfCoin[eCOIN_TYPE_bigGold]	= 100.f;

	m_ValueOfStar[eSTAR_TYPE_bronze]	= 10.f;
	m_ValueOfStar[eSTAR_TYPE_silver]	= 20.f;
	m_ValueOfStar[eSTAR_TYPE_gold]		= 30.f;
	m_ValueOfStar[eSTAR_TYPE_bigSilver] = 50.f;
	m_ValueOfStar[eSTAR_TYPE_bigGold]	= 100.f;
}

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