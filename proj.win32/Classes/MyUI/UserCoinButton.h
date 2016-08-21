#pragma once
#include "../GameObject/GameObject.h"

class CScoreUI;
class CMyButton;

class CUserCoinButton : public CGameObject
{
public:
	static CUserCoinButton* create();
	void UpdateUI();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	void EarnCoin();

	CUserCoinButton()
		: m_CoinLabel(nullptr)
		, m_CoinIcon(nullptr)
		, m_Back(nullptr)
		, m_UserCoin(nullptr){}
	virtual ~CUserCoinButton(){};

private:
	Label* m_CoinLabel;
	Sprite* m_CoinIcon;
	LayerColor* m_Back;
	CScoreUI* m_UserCoin;
};