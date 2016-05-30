#pragma once

#include "cocos2d.h"

class CMenuScene : public cocos2d::Layer
{
public:
	CREATE_FUNC(CMenuScene);
	static cocos2d::Scene* createScene();
	virtual ~CMenuScene();


protected:
	virtual bool init() override;

private:
	bool initVariable();
};
