#include "ItemManager.h"
#include "ObjectManager.h"
#include "Player.h"
#include "../Scene/GameScene.h"
#include "../DataManager/UserDataManager.h"

CItemManager::CItemManager()
{
	// 기본 값
	m_ValueOfCoin[eCOIN_TYPE_bronze]	= 1.f;
	m_ValueOfCoin[eCOIN_TYPE_silver]	= 3.f;
	m_ValueOfCoin[eCOIN_TYPE_gold]		= 5.f;
	m_ValueOfCoin[eCOIN_TYPE_bigSilver] = 10.f;
	m_ValueOfCoin[eCOIN_TYPE_bigGold]	= 20.f;

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
            m_ItemTimersLimit[itemType] = m_ItemTimers[itemType] + CUserDataManager::Instance()->getItemCurrentValue("USER_MAGNET_LIMIT_TIME_IDX");
            break;
            
        case eITEM_TYPE_coin:
            m_ItemTimersLimit[itemType] = m_ItemTimers[itemType] + CUserDataManager::Instance()->getItemCurrentValue("USER_COIN_LIMIT_TIME_IDX");
            break;
            
        case eITEM_TYPE_star:
            m_ItemTimersLimit[itemType] = m_ItemTimers[itemType] + CUserDataManager::Instance()->getItemCurrentValue("USER_STAR_LIMIT_TIME_IDX");
            break;
            
        case eITEM_TYPE_giant:
            m_ItemTimersLimit[itemType] = m_ItemTimers[itemType] + CUserDataManager::Instance()->getItemCurrentValue("USER_GIANT_LIMIT_TIME_IDX");
            break;
            
        case eITEM_TYPE_bonustime:
            m_ItemTimersLimit[itemType] = m_ItemTimers[itemType] + CUserDataManager::Instance()->getItemCurrentValue("USER_BONUS_LIMIT_TIME_IDX");
            break;
            
        default:
            break;
    }
    
    // 계산된 값이 0보다 작거나 같은 경우 CurrentItem에 연산하지 않는다.
    if(m_ItemTimersLimit[itemType] > 0)
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



