#pragma once
#include "../Common/HSHUtility.h"
#include <map>

/*------------------------------ UIManager ------------------------------
* singleton 클래스
* AddUIWithName함수는 addChild를 하지 않는다.
* 리스트에만 등록하기 때문에 addChild는 해당Scene에서 해줘야한다.
------------------------------------------------------------------------*/

class CGameObject;
class CUIManager
{
public:
	static CUIManager* Instance();
	void Execute(float delta = 0.f);

	bool AddUIWithName(CGameObject* ui, std::string name);
	CGameObject* FindUIWithName(std::string name) const ;

	// 초기화
	void Clear();

private:
	CUIManager(){ Clear(); };
	virtual ~CUIManager(){};

private:
	std::map<std::string, CGameObject*> m_UIList;
};

