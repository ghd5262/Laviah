#pragma once
#include "Bullet/BulletHeaders.h"
#include "../Common/HSHUtility.h"

class CItemManager
{
public:
	static CItemManager* Instance();
	void Execute(float delta);

	//���� ���ư��� Ÿ�̸� ��ȯ(�÷��� ����)
	int getCurrentItem(){ return m_CurrentItems; }

	//������ �������� ȹ�������� ȣ���
	void StartItemTimer(eITEM_TYPE itemType);

	//������ Ÿ�̸� ����
	void FinishItemTimer(eITEM_FLAG itemType){ m_CurrentItems ^= itemType; }

	// �ʱ�ȭ
	void Clear();

	float getValueOfCoin(eCOIN_TYPE type) { return m_ValueOfCoin[type]; }
	float getValueOfStar(eSTAR_TYPE type) { return m_ValueOfStar[type]; }
private:
	CItemManager();
	~CItemManager(){};

private:
	// ���� �÷��̾ ȹ���Ͽ� ����ǰ� �ִ� ������
	int m_CurrentItems;
	float m_ValueOfCoin[eCOIN_TYPE_MAX];
	float m_ValueOfStar[eSTAR_TYPE_MAX];
	float m_ItemTimers[eITEM_TYPE_MAX];
	float m_ItemTimersLimit[eITEM_TYPE_MAX];
};

