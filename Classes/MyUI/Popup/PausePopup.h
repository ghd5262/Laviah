#pragma once
#include "../../DataManager/AchievementDataManager.hpp"
#include "../Popup.h"

class CGoalPopupDP;
class CPausePopup : public CPopup
{
public:
	static CPausePopup* create();

protected:
	virtual bool init() override;

private:
    void initAchievementList();
    void createAchievementDP(const ACHIEVEMENT* data,
                             int posIndex);
    void createRewardBox();
	void Play(cocos2d::Node* sender);
	void Reset(cocos2d::Node* sender);
	void GoHome(cocos2d::Node* sender);
	void GameExit(cocos2d::Node* sender);
	void Help(cocos2d::Node* sender);
    void Skip(CGoalPopupDP* sender, int sequence);

	CPausePopup();
	virtual ~CPausePopup();
    
private:
    std::vector<CPopup*> m_AchievementList;
};