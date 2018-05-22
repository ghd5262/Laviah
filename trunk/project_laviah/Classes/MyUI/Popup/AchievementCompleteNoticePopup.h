#pragma once
#include "cocos2d.h"
#include "../Popup.h"

class CAchievementCompleteNoticePopup : public CPopup
{
public:
	static CAchievementCompleteNoticePopup* create();

protected:
	virtual bool init() override;

private:
	void checkAchievementCompleteOnRealTime();
	void show();

	CAchievementCompleteNoticePopup()
		: m_LayerBG(nullptr)
		, m_AchievementLabel(nullptr)
        , m_Checkable(true){};
	virtual ~CAchievementCompleteNoticePopup(){};

private:
	cocos2d::Sprite* m_LayerBG;
	cocos2d::Label* m_AchievementLabel;
    bool m_Checkable;
};