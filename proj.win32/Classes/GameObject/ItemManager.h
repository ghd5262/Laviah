#pragma once
#include "Bullet/BulletHeaders.h"
#include "../Common/HSHUtility.h"

class CItemManager
{
public:
	static CItemManager* Instance();
	void Execute(float delta);

    bool isCurrentItem(eITEM_FLAG itemType) { return (m_CurrentItems & itemType) == itemType; }

    //현재 돌아가는 타이머 반환(플래그 형식)
	int getCurrentItem(){ return m_CurrentItems; }

	//유저가 아이템을 획득했을때 호출됨
	void StartItemTimer(eITEM_TYPE itemType);

	//아이템 타이머 종료
	void FinishItemTimer(eITEM_FLAG itemType){ m_CurrentItems ^= itemType; }

	// 초기화
	void Clear();

	float getValueOfCoin(eCOIN_TYPE type) { return m_ValueOfCoin[type]; }
	float getValueOfStar(eSTAR_TYPE type) { return m_ValueOfStar[type]; }
private:
	CItemManager();
	~CItemManager(){};

private:
	// 현재 플레이어가 획득하여 적용되고 있는 아이템
	int m_CurrentItems;
	float m_ValueOfCoin[eCOIN_TYPE_MAX];
	float m_ValueOfStar[eSTAR_TYPE_MAX];
	float m_ItemTimers[eITEM_TYPE_MAX];
	float m_ItemTimersLimit[eITEM_TYPE_MAX];
};

