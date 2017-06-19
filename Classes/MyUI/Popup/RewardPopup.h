#pragma once
#include "../../DataManager/AchievementDataManager.hpp"
#include "../Popup.h"
#include <vector>

struct ITEM_DATA{
    sREWARD_DATA _rewardData;
    int _cost;
    ITEM_DATA(sREWARD_DATA data, int cost)
    : _rewardData(data)
    , _cost(cost){};
};
typedef std::vector<ITEM_DATA> ITEM_LIST;

class CRewardPopup : public CPopup
{
    typedef std::function<void()> EXIT_CALLBACK;
    
public:
	static CRewardPopup* create();
    CRewardPopup* setExitCallback(const EXIT_CALLBACK &callback);
    CPopup* show(cocos2d::Node* parent, unsigned zOrder = 0);
    void AddItemToList(std::string key, int value, int cost);

private:
	CPopup* createRewardDP(sREWARD_DATA data);
    void uiUpdate();
	void open();
    void end();
    bool isItemRemain();

	CRewardPopup()
    : m_RewardDP(nullptr)
    , m_ExitCallback(nullptr)
    , m_BtnEnd(nullptr)
    , m_BtnGet(nullptr)
    , m_ItemIndex(0)
    , m_OpenEnable(true){};
	virtual ~CRewardPopup(){};

private:
	CPopup* m_RewardDP;
	ITEM_LIST m_ItemList;
    EXIT_CALLBACK m_ExitCallback;
    CMyButton* m_BtnEnd;
    CMyButton* m_BtnGet;
	int m_ItemIndex;
    bool m_OpenEnable;
};