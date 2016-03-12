#pragma once
#include "../Common/HSHUtility.h"
#include "../GameObject/GameObject.h"
#include <map>

/*------------------------------ UIManager ------------------------------
* ù°�� singleton Ŭ������ �ƴϴ�
* Scene���� UIManager�� �����ؼ� ����ϰ� Scene�� ��ȯ�ɶ� UI�� �Բ� �����ȴ�.
* AddUIWithName�Լ��� addChild�� ���� �ʴ´�.
* ����Ʈ���� ����ϱ� ������ addChild�� �ش�Scene���� ������Ѵ�.
* �����͸� ������ �ִ� ������ �ƹ��� �������� �����ϴ� ���� ������������
* �Ϻη� �����ͷ� ã�� �Լ��� ������ �ʾҴ�. 
------------------------------------------------------------------------*/

class CUIManager : public CGameObject
{
public:
	static CUIManager* create();
	virtual void Execute(float delta = 0.f);

	bool AddUIWithName(CGameObject* ui, std::string name);
	CGameObject* FindUIWithName(std::string name);

private:
	CUIManager(){};
	virtual ~CUIManager(){};

private:
	std::map<std::string, CGameObject*> m_UIList;
};

