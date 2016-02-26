#pragma once

#include "..\Common\HSHUtility.h"

struct Telegram;

typedef enum eENTITY_TYPE{
	eENTITY_TYPE_default = -1,
};

///게임 모든 오브젝트의 최상위 클래스
class CGameObject : public cocos2d::Node
{
public:
	CGameObject(){};
	CGameObject(int ID)
		: m_dBoundingRadius(0.0f)
		, m_nEntityType(eENTITY_TYPE_default)
		, m_bTag(false)
	{
		setM_ID(ID);
	}

	virtual ~CGameObject();

public:
	virtual bool HandleMessage(const Telegram& msg){ return false; }

	static int   getM_NextValidID(){ return m_nNextValidID; }
	static void  resetNextValidID(){ m_nNextValidID = 0; }

protected:
	
	void DrawDebugRect(Point pos1, Point pos2, std::string text = "");
	void DrawDebugLine(Point pos1, Point pos2, std::string text = "");

protected:
	///getter & setter
	CC_SYNTHESIZE(double, m_dBoundingRadius, BRadius);
	CC_SYNTHESIZE(int, m_nEntityType, EntityType);
	void setM_ID(int ID);
	int getM_ID(){ return m_nID; }

private:
	static int m_nNextValidID;
	int m_nID;
	bool m_bTag;
};

