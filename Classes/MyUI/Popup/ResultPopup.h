#pragma once
#include "../Popup.h"

class CMyButton;
class CUserCoinButton;
class CResultPopup : public CPopup
{
public:
	static CResultPopup* create();

protected:
	virtual bool init() override;

private: 
	void Reset(cocos2d::Node* sender);
	void GoHome(cocos2d::Node* sender);
    void End(cocos2d::Node* sender);
    void GetCoinFromVideo(cocos2d::Node* sender);
    void GetNewCharacter(cocos2d::Node* sender);
    void GetFreeReward(cocos2d::Node* sender);
    void Share(cocos2d::Node* sender);
    
    void createRewardPopup(std::string key, int value);
    void exit();
    
	CResultPopup()
    : m_GoalPopupOpen(false){};
	virtual ~CResultPopup(){};

private:
    bool m_GoalPopupOpen;
};