#pragma once

#include "../Common/HSHUtility.h"


// ���� ��� ������Ʈ�� �ֻ��� Ŭ����
class CGameObject : public cocos2d::Node
{
public:
	CGameObject(float bindingRadius)
		: m_fBoundingRadius(bindingRadius){}
	virtual ~CGameObject(){ removeAllChildrenWithCleanup(true); }

	virtual void Execute(float delta = 0.f){}

protected:
	CGameObject() : m_fBoundingRadius(0.0f){}
	virtual bool init() override { return true; }
	virtual bool initVariable(){ return true; }

	void DrawDebugBinding();												// DEBUG�� BindingRound�� �׷��ش�. 
	void DrawDebugRect(Point pos1, Point pos2, std::string text = "");
	void DrawDebugLine(Point pos1, Point pos2, std::string text = "");

	//getter & setter
	CC_SYNTHESIZE(float, m_fBoundingRadius, BRadius);
};

