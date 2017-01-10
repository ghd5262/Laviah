#pragma once
#include "../../DataManager/ChallengeDataManager.hpp"
#include "../Popup.h"
#include <vector>

typedef std::vector<sREWARD_DATA> REWARD_LIST;

class CRewardPopup : public CPopup
{
public:
	static CRewardPopup* create();
	void AddRewardToList(std::string key, int value);

protected:
	virtual bool init() override;

private:
	CPopup* createRewardDP(sREWARD_DATA data);
	CRewardPopup()
		: m_RewardDP(nullptr)
		, m_RewardIndex(0){};
	virtual ~CRewardPopup(){};

private:
	CPopup* m_RewardDP;
	REWARD_LIST m_RewardList;
	int m_RewardIndex;
};