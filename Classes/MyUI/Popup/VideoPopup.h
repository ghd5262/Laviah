#pragma once
#include "../Popup.h"

class CMyButton;
class CUserCoinButton;
class CCountDown;

class CVideoPopup : public CPopup
{
public:
	static CVideoPopup* create();

protected:
	virtual bool init() override;

private:
	void End(cocos2d::Node* sender);
	void ReviveByCoin(cocos2d::Node* sender);
    void ReviveByVideo(cocos2d::Node* sender);
    void GetCoinFromVideo(cocos2d::Node* sender);
    void GetNewCharacter(cocos2d::Node* sender);
    void GetFreeReward(cocos2d::Node* sender);
	void Resume();
    
    void createRewardPopup(std::string key, int value);
    
	CVideoPopup(){};
	virtual ~CVideoPopup(){};
    
private:
    CCountDown* m_CountDown;
};