#pragma once
#include "../Common/HSHUtility.h"


enum eITEM_TYPE{
	eITEM_TYPE_health = 0,
	eITEM_TYPE_shield = 1,
	eITEM_TYPE_magnet = 2,
	eITEM_TYPE_coin = 3,
	eITEM_TYPE_star = 4,
	eITEM_TYPE_giant = 5,
	eITEM_TYPE_bonustime = 6,

	eITEM_TYPE_MAX
};

enum eITEM_FLAG{
	eITEM_FLAG_none = 0x0000,		//eITEM_TYPE에서 eITEM_FLAG으로 변환하면 아래와 같다.
	eITEM_FLAG_shield = 0x0002,		//== eITEM_TYPE_shield = 1,
	eITEM_FLAG_magnet = 0x0004,		//== eITEM_TYPE_magnet = 2,
	eITEM_FLAG_coin = 0x0008,		//== eITEM_TYPE_coin = 3,
	eITEM_FLAG_star = 0x00010,		//== eITEM_TYPE_star = 4,
	eITEM_FLAG_giant = 0x0020,		//== eITEM_TYPE_giant = 5,
	eITEM_FLAG_bonustime = 0x0040,	//== eITEM_TYPE_bonustime = 6,
};

class CItemManager
{
public:
	static CItemManager* Instance();
	void Execute(float delta);

	

	//현재 돌아가는 타이머 반환(플래그 형식)
	int getCurrentItem(){ return m_CurrentItems; }

	//유저가 아이템을 획득했을때 호출됨
	void StartItemTimer(eITEM_FLAG itemType);

	//아이템 타이머 종료
	void FinishItemTimer(eITEM_FLAG itemType){ m_CurrentItems ^= itemType; }

private:
	CItemManager(){};
	~CItemManager(){};

private:
	// 현재 플레이어가 획득하여 적용되고 있는 아이템
	int m_CurrentItems;
};

