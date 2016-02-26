#pragma once

#include "cocos2d.h"

class CGameScene : public cocos2d::Layer
{
public:
	CREATE_FUNC(CGameScene);
	static cocos2d::Scene* createScene();
	virtual ~CGameScene();
protected:
	virtual bool init();

private:
	bool initVariable();
	void menuCloseCallback(cocos2d::Ref* pSender);
};
