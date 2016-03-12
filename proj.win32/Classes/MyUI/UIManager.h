#pragma once
#include "../Common/HSHUtility.h"
#include "../GameObject/GameObject.h"
#include <map>

/*------------------------------ UIManager ------------------------------
* 첫째로 singleton 클래스가 아니다
* Scene별로 UIManager를 생성해서 사용하고 Scene이 전환될때 UI도 함께 삭제된다.
* AddUIWithName함수는 addChild를 하지 않는다.
* 리스트에만 등록하기 때문에 addChild는 해당Scene에서 해줘야한다.
* 포인터를 가지고 있는 곳에서 아무런 생각없이 남용하는 것을 막는차원에서
* 일부러 포인터로 찾는 함수는 만들지 않았다. 
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

