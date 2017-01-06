#pragma once
#include "../Popup.h"

class CChallengePopupDP;
class CChallengePopup : public CPopup
{
public:
	static CChallengePopup* create();
	CPopup* EndButtonVisible(bool visible);
	virtual CPopup* show(Node* parent, int zOrder = 0) override;

protected:
	virtual bool init() override;

private:
	void End(cocos2d::Node* sender);
    void Skip(CChallengePopupDP* sender, int posIndex);
    
	CChallengePopup()
		: m_EndButtonVisible(false){};
	virtual ~CChallengePopup(){};

private:
	bool m_EndButtonVisible;
};