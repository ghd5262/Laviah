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

enum eCOIN_TYPE{
	eCOIN_TYPE_bronze = 0,
	eCOIN_TYPE_silver = 1,
	eCOIN_TYPE_gold = 2,
	eCOIN_TYPE_bigSilver = 3,
	eCOIN_TYPE_bigGold = 4,

	eCOIN_TYPE_MAX
};

enum eSTAR_TYPE{
	eSTAR_TYPE_bronze = 0,
	eSTAR_TYPE_silver = 1,
	eSTAR_TYPE_gold = 2,
	eSTAR_TYPE_bigSilver = 3,
	eSTAR_TYPE_bigGold = 4,

	eSTAR_TYPE_MAX
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
};

