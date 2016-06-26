#pragma once
#include "../GameObject/GameObject.h"

class CScoreUI;
class CMyButton;

class CUserCoinButton : public CGameObject
{
public:
	static CUserCoinButton* create();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	void EarnCoin();

	CUserCoinButton(){}
	virtual ~CUserCoinButton(){};

};