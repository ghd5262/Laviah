#include "ItemManager.h"
#include "../Scene/GameScene.h"

CItemManager::CItemManager()
{
	// ±âº» °ª
	m_ValueOfCoin[eCOIN_TYPE_bronze]	= 5.f;
	m_ValueOfCoin[eCOIN_TYPE_silver]	= 10.f;
	m_ValueOfCoin[eCOIN_TYPE_gold]		= 15.f;
	m_ValueOfCoin[eCOIN_TYPE_bigSilver] = 50.f;
	m_ValueOfCoin[eCOIN_TYPE_bigGold]	= 100.f;

	m_ValueOfStar[eSTAR_TYPE_bronze]	= 5.f;
	m_ValueOfStar[eSTAR_TYPE_silver]	= 10.f;
	m_ValueOfStar[eSTAR_TYPE_gold]		= 15.f;
	m_ValueOfStar[eSTAR_TYPE_bigSilver] = 50.f;
	m_ValueOfStar[eSTAR_TYPE_bigGold]	= 100.f;

	memset(m_ItemTimers, 0, sizeof(m_ItemTimers));
	memset(m_ItemTimersLimit, 0, sizeof(m_ItemTimersLimit));

	m_CurrentItems = 0;
}

CItemManager* CItemManager::Instance()
{
	static CItemManager instance;
	return &instance;
}

void CItemManager::StartItemTimer(eITEM_TYPE itemType)
{
	m_CurrentItems |= (1 << itemType);
	m_ItemTimersLimit[itemType] = m_ItemTimers[itemType] + 8.f;
	//CGameScene::getGameScene()->scheduleOnce([this, itemType](float dt){
	//	this->FinishItemTimer(itemType);
	//}, 20.0f, MakeString("Item_%d_Timer", static_cast<int>(itemType)));
}

void CItemManager::Execute(float delta)
{
	for (int i = 0; i < eITEM_TYPE_MAX; i++)
	{
		if (m_ItemTimersLimit[i])
			m_ItemTimers[i] += delta;
		if (m_ItemTimers[i] > m_ItemTimersLimit[i])
		{
			this->FinishItemTimer(static_cast<eITEM_FLAG>(1 << i));
			m_ItemTimersLimit[i] = 0.f;
			m_ItemTimers[i] = 0.f;
		}
	}
}



