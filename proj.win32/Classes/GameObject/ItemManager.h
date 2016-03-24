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
	eITEM_FLAG_none = 0x0000,		//eITEM_TYPE���� eITEM_FLAG���� ��ȯ�ϸ� �Ʒ��� ����.
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

	

	//���� ���ư��� Ÿ�̸� ��ȯ(�÷��� ����)
	int getCurrentItem(){ return m_CurrentItems; }

	//������ �������� ȹ�������� ȣ���
	void StartItemTimer(eITEM_FLAG itemType);

	//������ Ÿ�̸� ����
	void FinishItemTimer(eITEM_FLAG itemType){ m_CurrentItems ^= itemType; }

private:
	CItemManager(){};
	~CItemManager(){};

private:
	// ���� �÷��̾ ȹ���Ͽ� ����ǰ� �ִ� ������
	int m_CurrentItems;
};

