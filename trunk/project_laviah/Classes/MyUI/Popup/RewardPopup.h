#pragma once
#include "../../DataManager/AchievementDataManager.hpp"
#include "../Popup.h"
#include <vector>
#include <map>

typedef std::vector<sREWARD_DATA> REWARD_LIST;

class CRewardPopup : public CPopup
{
    typedef std::function<void(bool)> EXIT_CALLBACK;
    
public:
	static CRewardPopup* create();
    CRewardPopup* AddRewardToList(std::vector<sREWARD_DATA> list);
    CRewardPopup* setExitCallback(const EXIT_CALLBACK &callback);
    CRewardPopup* setTitle(std::string title);
    CRewardPopup* setIsPaidFeature(int cost);
    CRewardPopup* setIsUFOEnable(bool enable);
    CPopup* show(Node* parent = nullptr, int zOrder = 0);
    void AddRewardToList(std::string key, int value);

    static void createFlyAction(Node* sender,
                                Vec2 targetPos1,
                                Vec2 targetPos2,
                                float delayTime = 1.f);
private:
	CPopup* createRewardDP(sREWARD_DATA data);
    void createUFO();
    void lightOn();
	void open();
    void play();
    void end();
    void exit(bool isPlay = false);
    void showButtons();
    bool isItemRemain();

	CRewardPopup()
    : m_RewardDP(nullptr)
    , m_GetButton(nullptr)
    , m_PlayButton(nullptr)
    , m_ExitCallback(nullptr)
    , m_UFO(nullptr)
    , m_UFOLight(nullptr)
    , m_TitleLabel(nullptr)
    , m_Title("")
    , m_RewardIndex(0)
    , m_Cost(0)
    , m_IsPaidFeature(false)
    , m_IsUFOEnable(false){};
	virtual ~CRewardPopup(){};

private:
	CPopup* m_RewardDP;
    CMyButton* m_GetButton;
    CMyButton* m_PlayButton;
	REWARD_LIST m_RewardList;
    EXIT_CALLBACK m_ExitCallback;
    sREWARD_DATA m_LastSavedData;
    cocos2d::Sprite* m_UFO;
    cocos2d::Sprite* m_UFOLight;
    cocos2d::Label* m_TitleLabel;
    std::string m_Title;
	int m_RewardIndex;
    int m_Cost;
    bool m_IsPaidFeature;
    bool m_IsUFOEnable;
};
