#include "ItemManager.h"
#include "ObjectManager.h"
#include "Player.h"
#include "../Scene/GameScene.h"
#include "../DataManager/UserDataManager.h"
#include "../MyUI/UILayer.hpp"

CItemManager::CItemManager()
{
	// 기본 값
	m_ValueOfCoin[eCOIN_TYPE_bronze]	= 1.f;
	m_ValueOfCoin[eCOIN_TYPE_silver]	= 2.f;
	m_ValueOfCoin[eCOIN_TYPE_gold]		= 3.f;
	m_ValueOfCoin[eCOIN_TYPE_bigSilver] = 4.f;
	m_ValueOfCoin[eCOIN_TYPE_bigGold]	= 5.f;

	m_ValueOfStar[eSTAR_TYPE_bronze]	= 1.f;
	m_ValueOfStar[eSTAR_TYPE_silver]	= 2.f;
	m_ValueOfStar[eSTAR_TYPE_gold]		= 3.f;
	m_ValueOfStar[eSTAR_TYPE_bigSilver] = 4.f;
	m_ValueOfStar[eSTAR_TYPE_bigGold]	= 5.f;
}

CItemManager* CItemManager::Instance()
{
	static CItemManager instance;
	return &instance;
}

void CItemManager::Clear()
{
	for (int idx = 1; idx < eITEM_TYPE_MAX; idx++){
		m_ItemTimers[idx] = 0;
		m_ItemTimersLimit[idx] = 0;
		CUILayer::Instance()->setItemTimer((eITEM_TYPE)idx, 0);
	}

	m_CurrentItems = 0;
}

void CItemManager::StartItemTimer(eITEM_TYPE itemType)
{
    auto player = CObjectManager::Instance()->getPlayer();
    auto setTimer = [=](eITEM_TYPE  type, float limitTime){
		m_ItemTimers[type] = 0.f;
        m_ItemTimersLimit[type] = limitTime;
        CUILayer::Instance()->setItemTimer(type, m_ItemTimersLimit[type]);
    };
    
    switch (itemType) {
        case eITEM_TYPE_health: player->GotSomeHealth(20); break;
        case eITEM_TYPE_shield: player->GotBarrierItem(); break;
        case eITEM_TYPE_magnet: player->GotMagnetItem();
            setTimer(itemType, player->getMagnetLimitTime());
            break;
            
        case eITEM_TYPE_coin: setTimer(itemType, player->getCoinLimitTime()); break;
        case eITEM_TYPE_star: setTimer(itemType, player->getStarLimitTime()); break;
        case eITEM_TYPE_giant: setTimer(itemType, player->getGiantLimitTime()); break;
        case eITEM_TYPE_bonustime: setTimer(itemType, player->getBonusTimeLimitTime()); break;
            
        default: break;
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



