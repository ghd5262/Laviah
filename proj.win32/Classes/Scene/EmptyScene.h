#pragma once

#include "cocos2d.h"

class CEmptyScene : public cocos2d::Layer
{
public:
	CREATE_FUNC(CEmptyScene);
	static cocos2d::Scene* createScene();
	virtual ~CEmptyScene();
};
