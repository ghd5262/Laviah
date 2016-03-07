#pragma once

#include "../Common/HSHUtility.h"


// 게임 모든 오브젝트의 최상위 클래스
class CGameObject : public cocos2d::Node
{
public:
	CGameObject(float bindingRadius)
		: m_fBoundingRadius(bindingRadius){}
	virtual ~CGameObject(){ removeAllChildrenWithCleanup(true); }

	virtual void Execute(float delta = 0.f){}

	//getter & setter
	bool IsAlive()const { return m_bAlive; }
	void setAlive(bool alive){ m_bAlive = alive; }

protected:
	virtual bool init() override { return true; }
	virtual bool initVariable(){ return true; };

	void DrawDebugBinding();												// DEBUG용 BindingRound를 그려준다. 
	void DrawDebugRect(Point pos1, Point pos2, std::string text = "");
	void DrawDebugLine(Point pos1, Point pos2, std::string text = "");

	//getter & setter
	CC_SYNTHESIZE(float, m_fBoundingRadius, BRadius);

private:
	bool m_bAlive;
};

