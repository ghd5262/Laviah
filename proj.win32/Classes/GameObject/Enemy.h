#pragma once
#include "Mover.h"

class CEnemy : public CMover {

protected:
	CEnemy(float boundingRadius,
		float interval);
	virtual ~CEnemy();

	//getter & setter
	CC_SYNTHESIZE(float, m_fInterval, Interval);
	CC_SYNTHESIZE(float, m_fTime, Time);
};