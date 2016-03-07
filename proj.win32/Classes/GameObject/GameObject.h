#pragma once

#include "../Common/HSHUtility.h"


///���� ��� ������Ʈ�� �ֻ��� Ŭ����
class CGameObject : public cocos2d::Node
{
public:
	CGameObject() : m_fBoundingRadius(0.0f){}
	CGameObject(float bindingRadius)
		: m_fBoundingRadius(bindingRadius){}
	virtual ~CGameObject(){ removeAllChildrenWithCleanup(true); }

	virtual void Execute(float delta = 0.f){}

	//getter & setter
	bool IsAlive()const { return m_bAlive; }
	void setAlive(bool alive){ m_bAlive = alive; }

protected:

	void DrawDebugBinding();
	void DrawDebugRect(Point pos1, Point pos2, std::string text = "");
	void DrawDebugLine(Point pos1, Point pos2, std::string text = "");

	//getter & setter
	CC_SYNTHESIZE(float, m_fBoundingRadius, BRadius);

private:
	bool m_bAlive;
};

