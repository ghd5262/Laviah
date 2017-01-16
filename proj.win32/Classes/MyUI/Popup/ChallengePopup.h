#pragma once
#include "../Popup.h"
#include "../../DataManager/ChallengeDataManager.hpp"
#include <array>

class CChallengePopupDP;
class CChallengePopup : public CPopup
{
public:
	static CChallengePopup* create();
	virtual CPopup* show(Node* parent, int zOrder = 0) override;

protected:
	virtual bool init() override;

private:
    void initChallengeList();
    void createChallengeDP(const sCHALLENGE_PARAM* data,
                           int posIndex,
                           bool isSkip);
    
    void Reset(cocos2d::Node* sender);
    void GoHome(cocos2d::Node* sender);
    void Skip(CChallengePopupDP* sender, int posIndex);
    
    
	CChallengePopup(){};
	virtual ~CChallengePopup(){};
    
private:
    std::vector<CPopup*> m_ChallengeList;
    std::array<Vec2, 4> m_DPStartPosArray;
    std::array<Vec2, 4> m_DPTargetPosArray;
};