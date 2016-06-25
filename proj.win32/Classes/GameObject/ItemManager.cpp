#include "ItemManager.h"
#include "ObjectManager.h"
#include "Player.h"
#include "../Scene/GameScene.h"
#include "../DataManager/UserDataManager.h"

CItemManager::CItemManager()
{
	// ±âº» °ª
	m_ValueOfCoin[eCOIN_TYPE_bronze]	= 5.f;
	m_ValueOfCoin[eCOIN_TYPE_silver]	= 10.f;
	m_ValueOfCoin[eCOIN_TYPE_gold]		= 15.f;
	m_ValueOfCoin[eCOIN_TYPE_bigSilver] = 50.f;
	m_ValueOfCoin[eCOIN_TYPE_bigGold]	= 100.f;

	m_ValueOfStar[eSTAR_TYPE_bronze]	= 3.f;
	m_ValueOfStar[eSTAR_TYPE_silver]	= 5.f;
	m_ValueOfStar[eSTAR_TYPE_gold]		= 10.f;
	m_ValueOfStar[eSTAR_TYPE_bigSilver] = 20.f;
	m_ValueOfStar[eSTAR_TYPE_bigGold]	= 30.f;

	Clear();
}

CItemManager* CItemManager::Instance()
{
	static CItemManager instance;
	return &instance;
}

void CItemManager::Clear()
{
	memset(m_ItemTimers, 0, sizeof(m_ItemTimers));
	memset(m_ItemTimersLimit, 0, sizeof(m_ItemTimersLimit));

	m_CurrentItems = 0;
}

void CItemManager::StartItemTimer(eITEM_TYPE itemType)
{
    switch (itemType) {
        case eITEM_TYPE_health:
            CObjectManager::Instance()->getPlayer()->GotSomeHealth(20);
            break;
            
        case eITEM_TYPE_shield:
            CObjectManager::Instance()->getPlayer()->GotBarrierItem();
            break;
            
        case eITEM_TYPE_magnet:
            CObjectManager::Instance()->getPlayer()->GotMagnetItem();
            m_ItemTimersLimit[itemType] = m_ItemTimers[itemType] + CUserDataManager::Instance()->getUserDataMyMagnet();
            break;
            
        case eITEM_TYPE_coin:
            m_ItemTimersLimit[itemType] = m_ItemTimers[itemType] + CUserDataManager::Instance()->getUserDataMyCoin();
            break;
            
        case eITEM_TYPE_star:
            m_ItemTimersLimit[itemType] = m_ItemTimers[itemType] + CUserDataManager::Instance()->getUserDataMyStar();
            break;
            
        case eITEM_TYPE_giant:
            m_ItemTimersLimit[itemType] = m_ItemTimers[itemType] + CUserDataManager::Instance()->getUserDataMyGiant();
            break;
            
        case eITEM_TYPE_bonustime:
            m_ItemTimersLimit[itemType] = m_ItemTimers[itemType] + CUserDataManager::Instance()->getUserDataMyBonus();
            break;
            
        default:
            break;
    }
	m_CurrentItems |= (1 << itemType);
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



