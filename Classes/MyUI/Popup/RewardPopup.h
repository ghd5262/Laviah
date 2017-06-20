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
    CRewardPopup* setIsPaidFeature(int cost);
    CPopup* show(Node* parent = nullptr, int zOrder = 0);
    void AddRewardToList(std::string key, int value);

private:
	CPopup* createRewardDP(sREWARD_DATA data);
	void open();
    void play();
    void end();
    void exit();
    void showButtons();
    bool isItemRemain();

	CRewardPopup()
    : m_RewardDP(nullptr)
    , m_GetButton(nullptr)
    , m_PlayButton(nullptr)
    , m_ExitCallback(nullptr)
    , m_RewardIndex(0)
    , m_Cost(0)
    , m_IsPaidFeature(false){};
	virtual ~CRewardPopup(){};

private:
	CPopup* m_RewardDP;
    CMyButton* m_GetButton;
    CMyButton* m_PlayButton;
	REWARD_LIST m_RewardList;
    EXIT_CALLBACK m_ExitCallback;
    sREWARD_DATA m_LastSavedData;
	int m_RewardIndex;
    int m_Cost;
    bool m_IsPaidFeature;
};