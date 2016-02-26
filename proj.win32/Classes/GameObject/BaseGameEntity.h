//#pragma once
//
//#include "GameObject.h"
//
//struct Telegram;
//
//typedef enum eENTITY_TYPE{
//	eENTITY_TYPE_default = -1,
//};
//
//class CBaseGameEntity : public CGameObject
//{
//public:
//	CBaseGameEntity(int ID)
//		: m_dBoundingRadius(0.0f)
//		, m_nEntityType(eENTITY_TYPE_default)
//		, m_bTag(false)
//	{
//		setM_ID(ID);
//	}
//		
//	virtual ~CBaseGameEntity();
//
//public:
//	virtual bool HandleMessage(const Telegram& msg){ return false; }
//
//	static int   getM_NextValidID(){ return m_nNextValidID; }
//	static void  resetNextValidID(){ m_nNextValidID = 0; }
//
//protected:
//	///getter & setter
//	CC_SYNTHESIZE(double, m_dBoundingRadius, BRadius);
//	CC_SYNTHESIZE(int, m_nEntityType, EntityType);
//	void setM_ID(int ID);
//	int getM_ID(){ return m_nID; }
//
//private:
//	int m_nID;
//	static int m_nNextValidID;
//	bool m_bTag;
//
//};
//
