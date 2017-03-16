#pragma once

#include "../Common/HSHUtility.h"

// 게임 모든 오브젝트의 최상위 클래스
class CGameObject : public cocos2d::Node
{
public:
	virtual void Execute(float delta = 0.f){}

protected:
    CGameObject() : m_BoundingRadius(0.0f){}
    virtual ~CGameObject(){};
    
    virtual bool init() override { if(!cocos2d::Node::init())return false; return true; }

	//getter & setter
	CC_SYNTHESIZE(float, m_BoundingRadius, BoundingRadius);
};

