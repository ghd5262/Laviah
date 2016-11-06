#pragma once
#include "../Popup.h"

class CMyButton;
class CUserCoinButton;
class CResultPopup : public CPopup
{
public:
	static CResultPopup* create();

protected:
	virtual bool init() override;

private: 
	void Reset(cocos2d::Node* sender);
	void GoHome(cocos2d::Node* sender);

	CResultPopup(){};
	virtual ~CResultPopup(){};

};