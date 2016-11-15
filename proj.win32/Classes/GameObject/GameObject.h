#pragma once

#include "../Common/HSHUtility.h"


// 게임 모든 오브젝트의 최상위 클래스
class CGameObject : public cocos2d::Node
{
public:
	CGameObject(float bindingRadius)
		: m_fBoundingRadius(bindingRadius){}
	virtual ~CGameObject(){}
	virtual void Execute(float delta = 0.f){}

protected:
	CGameObject() : m_fBoundingRadius(0.0f){}
    virtual bool init() override { if(!cocos2d::Node::init())return false; return true; }
	virtual bool initVariable(){ return true; }

	void DrawDebugBinding();												// DEBUG용 BindingRound를 그려준다. 
	void DrawDebugRect(cocos2d::Point pos1, cocos2d::Point pos2, std::string text = "");
	void DrawDebugLine(cocos2d::Point pos1, cocos2d::Point pos2, std::string text = "");

	//getter & setter
	CC_SYNTHESIZE(float, m_fBoundingRadius, BRadius);
};

