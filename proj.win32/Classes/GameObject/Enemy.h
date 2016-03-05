#pragma once
#include "Mover.h"

class CEnemy : public CMover {
public:
	CEnemy(){}
	CEnemy(float boundingRadius,
		float interval);
	virtual ~CEnemy();

protected:
	//getter & setter
	CC_SYNTHESIZE(float, m_fInterval, Interval);
	CC_SYNTHESIZE(float, m_fTime, Time);
};