#pragma once
#include "../Popup.h"

class CPausePopup : public CPopup
{
public:
	static CPausePopup* create();

protected:
	virtual bool init() override;

private:
	void Play(cocos2d::Node* sender);
	void Reset(cocos2d::Node* sender);
	void GoHome(cocos2d::Node* sender);
	void GameExit(cocos2d::Node* sender);
	void Help(cocos2d::Node* sender);

	CPausePopup();
	virtual ~CPausePopup();
};