#pragma once
#include "../../DataManager/ChallengeDataManager.hpp"
#include "../Popup.h"
#include <vector>

typedef std::vector<sREWARD_DATA> REWARD_LIST;

class CRewardPopup : public CPopup
{
    typedef std::function<void()> EXIT_CALLBACK;
    
public:
	static CRewardPopup* create();
    CPopup* setExitCallback(const EXIT_CALLBACK &callback);
	void AddRewardToList(std::string key, int value);

protected:
	virtual bool init() override;

private:
	CPopup* createRewardDP(sREWARD_DATA data);
	CRewardPopup()
    : m_RewardDP(nullptr)
    , m_ExitCallback(nullptr)
    , m_RewardIndex(0){};
	virtual ~CRewardPopup(){};

private:
	CPopup* m_RewardDP;
	REWARD_LIST m_RewardList;
    EXIT_CALLBACK m_ExitCallback;
	int m_RewardIndex;
};