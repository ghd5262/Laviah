#pragma once
#include "Mover.h"

class CEnemy : public CMover {

public:
	CEnemy() : m_fTime(0.0f), m_fInterval(0.0f){}
	virtual ~CEnemy();
	void* operator new (size_t size, const std::nothrow_t);

protected:
	//getter & setter
	CC_SYNTHESIZE(float, m_fInterval, Interval);
	CC_SYNTHESIZE(float, m_fTime, Time);
};