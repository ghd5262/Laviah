#pragma once

#include "../Common/HSHUtility.h"


///게임 모든 오브젝트의 최상위 클래스
class CGameObject : public cocos2d::Node
{
public:
	CGameObject(std::string textureName, float bindingRadius)
		: m_TextureName(textureName)
		, m_fBoundingRadius(bindingRadius)
		, m_bAlive(true)
	{
		if (!CGameObject::init())
		{
			CCLOG("FAILED TO INIT OBJECT");
			CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
			assert(false);
		}
		scheduleUpdate();
	}
	virtual ~CGameObject(){}

	virtual void Execute(float delta = 0.f) = 0;

	//getter & setter
	bool IsAlive()const { return m_bAlive; }
	void setAlive(bool alive){ m_bAlive = alive; }

protected:
	virtual bool init() override;
	virtual bool initVariable();

	void DrawDebugRect(Point pos1, Point pos2, std::string text = "");
	void DrawDebugLine(Point pos1, Point pos2, std::string text = "");

	//getter & setter
	CC_SYNTHESIZE(float, m_fBoundingRadius, BRadius);

private:
	Sprite* m_pTexture;
	std::string m_TextureName;
	bool m_bAlive;
};

