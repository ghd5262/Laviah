#pragma once
#include "cocos2d.h"
#include "../Popup.h"

class CChallengeCompleteNoticePopup : public CPopup
{
public:
	static CChallengeCompleteNoticePopup* create();

protected:
	virtual bool init() override;

private:
	void checkChallengeCompleteOnRealTime();
	void show();

	CChallengeCompleteNoticePopup()
		: m_LayerBG(nullptr)
		, m_ChallengeLabel(nullptr)
		, m_Content("")
		, m_DelayTime(0.f){};
	virtual ~CChallengeCompleteNoticePopup(){};

private:
	cocos2d::Sprite* m_LayerBG;
	cocos2d::Label* m_ChallengeLabel;
	std::string m_Content;
	float m_DelayTime;
};