#pragma once
#include "../../DataManager/ChallengeDataManager.hpp"
#include "../Popup.h"

class CChallengePopupDP;
class CPausePopup : public CPopup
{
public:
	static CPausePopup* create();

protected:
	virtual bool init() override;

private:
    void initChallengeList();
    void createChallengeDP(const sCHALLENGE_PARAM* data,
                           int posIndex);
    void createRewardBox();
	void Play(cocos2d::Node* sender);
	void Reset(cocos2d::Node* sender);
	void GoHome(cocos2d::Node* sender);
	void GameExit(cocos2d::Node* sender);
	void Help(cocos2d::Node* sender);
    void Skip(CChallengePopupDP* sender, int sequence);

	CPausePopup();
	virtual ~CPausePopup();
    
private:
    std::vector<CPopup*> m_ChallengeList;
};