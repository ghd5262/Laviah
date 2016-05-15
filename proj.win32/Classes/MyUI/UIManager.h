#pragma once
#include "../Common/HSHUtility.h"
#include <map>

/*------------------------------ UIManager ------------------------------
* singleton Ŭ����
* AddUIWithName�Լ��� addChild�� ���� �ʴ´�.
* ����Ʈ���� ����ϱ� ������ addChild�� �ش�Scene���� ������Ѵ�.
------------------------------------------------------------------------*/

class CGameObject;
class CUIManager
{
public:
	static CUIManager* Instance();
	void Execute(float delta = 0.f);

	bool AddUIWithName(CGameObject* ui, std::string name);
	CGameObject* FindUIWithName(std::string name) const ;

	// �ʱ�ȭ
	void Clear();

private:
	CUIManager(){ Clear(); };
	virtual ~CUIManager(){};

private:
	std::map<std::string, CGameObject*> m_UIList;
};

