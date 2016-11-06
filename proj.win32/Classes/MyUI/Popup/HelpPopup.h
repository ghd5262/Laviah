#pragma once
#include "../Popup.h"

class CMyButton;
class CHelpPopup : public CPopup
{
public:
	static CHelpPopup* create();

protected:
	virtual bool init() override;

private:
	void End(cocos2d::Node* sender);

	CHelpPopup(){};
	virtual ~CHelpPopup(){};
};