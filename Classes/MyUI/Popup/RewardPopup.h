#pragma once
#include "../../DataManager/AchievementDataManager.hpp"
#include "../Popup.h"
#include <vector>

typedef std::vector<sREWARD_DATA> REWARD_LIST;

class CRewardPopup : public CPopup
{
    typedef std::function<void()> EXIT_CALLBACK;
    
public:
	static CRewardPopup* create();
    CRewardPopup* setExitCallback(const EXIT_CALLBACK &callback);
	void AddRewardToList(std::string key, int value);

protected:
	virtual bool init() override;

private:
	CPopup* createRewardDP(sREWARD_DATA data);
	void open();
    void end();
    
	CRewardPopup()
    : m_RewardDP(nullptr)
    , m_ExitCallback(nullptr)
    , m_BtnEnd(nullptr)
    , m_BG(nullptr)
    , m_RewardIndex(0)
    , m_OpenEnable(true){};
	virtual ~CRewardPopup(){};

private:
	CPopup* m_RewardDP;
	REWARD_LIST m_RewardList;
    EXIT_CALLBACK m_ExitCallback;
    CMyButton* m_BtnEnd;
    cocos2d::LayerColor* m_BG;
	int m_RewardIndex;
    bool m_OpenEnable;
};