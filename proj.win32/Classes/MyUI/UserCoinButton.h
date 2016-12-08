#pragma once
#include "../Common/HSHUtility.h"

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
	cocos2d::Label* m_CoinLabel;
	cocos2d::Sprite* m_CoinIcon;
	cocos2d::LayerColor* m_Back;
	CUserDataManager* m_UserData;
};