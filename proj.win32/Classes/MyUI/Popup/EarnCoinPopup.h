#pragma once
#include "../Popup.h"

class CMyButton;
class CEarnCoinPopup : public CPopup
{
public:
	static CEarnCoinPopup* create();

protected:
	virtual bool init() override;

private:
	void Video(cocos2d::Node* sender);

	CEarnCoinPopup(){};
	virtual ~CEarnCoinPopup(){};

};