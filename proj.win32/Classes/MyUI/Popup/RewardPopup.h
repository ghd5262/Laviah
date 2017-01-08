#pragma once
#include "../Popup.h"
#include <vector>

struct sREWARD_DATA{
	std::string _key;
	int _value;

	sREWARD_DATA(std::string key, int value)
		: _key(key)
		, _value(value){};
};

typedef std::vector<sREWARD_DATA> REWARD_LIST;

class CRewardPopup : public CPopup
{
public:
	static CRewardPopup* create();
	void AddRewardToList(std::string key, int value);

protected:
	virtual bool init() override;

private:
	CRewardPopup()
		: m_RewardIndex(0){};
	virtual ~CRewardPopup(){};

private:
	REWARD_LIST m_RewardList;
	int m_RewardIndex;
};