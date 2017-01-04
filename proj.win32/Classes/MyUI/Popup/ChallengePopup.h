#pragma once
#include "../Popup.h"

class CChallengePopupDP;
class CChallengePopup : public CPopup
{
public:
	static CChallengePopup* create();

protected:
	virtual bool init() override;

private:
    void skip(CChallengePopupDP* sender);
    
	CChallengePopup(){};
	virtual ~CChallengePopup(){};
};