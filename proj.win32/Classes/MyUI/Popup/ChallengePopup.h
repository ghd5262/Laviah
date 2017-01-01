#pragma once
#include "../Popup.h"

class CChallengePopup : public CPopup
{
public:
	static CChallengePopup* create();

protected:
	virtual bool init() override;

private:
	CChallengePopup(){};
	virtual ~CChallengePopup(){};

};