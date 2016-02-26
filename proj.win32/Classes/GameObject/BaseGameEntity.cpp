//#include "BaseGameEntity.h"
//
//int CBaseGameEntity::m_nNextValidID = 0;
//
//CBaseGameEntity::~CBaseGameEntity()
//{
//}
//
//void CBaseGameEntity::setM_ID(int ID)
//{
//	CCASSERT((ID >= m_nNextValidID), "CBaseGameEntity::setM_ID(int ID) : invalid ID, ID should be equal or greater than pre id");
//	m_nID = ID;
//	m_nNextValidID = m_nID + 1;
//}