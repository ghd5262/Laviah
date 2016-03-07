#pragma once
#include "Mover.h"

class CEnemy : public CMover {
protected:
	CEnemy(float interval) : CMover(0.0f), m_fInterval(interval), m_fTime(0.0f){}
	virtual ~CEnemy(){};

	//getter & setter
	CC_SYNTHESIZE(float, m_fInterval, Interval);
	CC_SYNTHESIZE(float, m_fTime, Time);
};