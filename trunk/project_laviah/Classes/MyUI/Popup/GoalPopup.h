#pragma once
#include "../Popup.h"
#include "../../DataManager/AchievementDataManager.hpp"
#include <array>

class CGoalPopupDP;
class CGoalPopup : public CPopup
{
public:
	static CGoalPopup* create();
	virtual CPopup* show(Node* parent, int zOrder = 0) override;

protected:
	virtual bool init() override;

private:
    void initAchievementList();
    void createAchievementDP(const ACHIEVEMENT* data,
                           int posIndex,
                           bool isSkip);
    
    void Reset(cocos2d::Node* sender);
    void GoHome(cocos2d::Node* sender);
    void Skip(CGoalPopupDP* sender, int posIndex);
    
    
	CGoalPopup(){};
	virtual ~CGoalPopup(){};
    
private:
    std::vector<CPopup*> m_AchievementList;
    std::array<Vec2, 4> m_DPStartPosArray;
    std::array<Vec2, 4> m_DPTargetPosArray;
};