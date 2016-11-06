#pragma once
#include "../Common/HSHUtility.h"

class CScoreUI;
class CMyButton;
class CUserDataManager;

class CUserCoinButton : public cocos2d::Node
{
public:
	static CUserCoinButton* create();
	
protected:
	virtual bool init() override;
	virtual void update(float delta) override;

private:
	void EarnCoin();

	CUserCoinButton()
		: m_CoinLabel(nullptr)
		, m_CoinIcon(nullptr)
		, m_Back(nullptr)
		, m_UserData(nullptr){}
	virtual ~CUserCoinButton(){};

private:
	Label* m_CoinLabel;
	Sprite* m_CoinIcon;
	LayerColor* m_Back;
	CUserDataManager* m_UserData;
};