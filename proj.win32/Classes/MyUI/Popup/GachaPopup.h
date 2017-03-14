#pragma once
#include "../Popup.h"

class CMyButton;
class CUserCoinButton;

class CGachaPopup : public CPopup
{
public:
	static CGachaPopup* create();

protected:
	virtual bool init() override;

private:
	void PlayGacha(cocos2d::Node* sender);
	void End(cocos2d::Node* sender);

	CGachaPopup() {};
	virtual ~CGachaPopup(){};
};