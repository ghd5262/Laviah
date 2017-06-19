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
    CPopup* show(Node* parent = nullptr, int zOrder = 0);
    void AddRewardToList(std::string key, int value);

private:
	CPopup* createRewardDP(sREWARD_DATA data);
	void open();
    void end();
    bool isItemRemain();

	CRewardPopup()
    : m_RewardDP(nullptr)
    , m_ExitCallback(nullptr)
    , m_RewardIndex(0)
    , m_OpenEnable(true){};
	virtual ~CRewardPopup(){};

private:
	CPopup* m_RewardDP;
	REWARD_LIST m_RewardList;
    EXIT_CALLBACK m_ExitCallback;
	int m_RewardIndex;
    bool m_OpenEnable;
};