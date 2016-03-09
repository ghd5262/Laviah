#pragma once
#include "Mover.h"

class CEnemy : public CMover {
public:
	virtual void ReturnToMemoryBlock() override;

protected:
	void* operator new (size_t size, const std::nothrow_t);
	
	CEnemy(float interval) : CMover(0.0f), m_fShotSpeed(250.0f), m_fShotAngle(0.0f), m_fInterval(interval), m_fTime(0.0f){}
	virtual ~CEnemy(){};

	//getter & setter
	CC_SYNTHESIZE(float, m_fShotSpeed, ShotSpeed);
	CC_SYNTHESIZE(float, m_fShotAngle, ShotAngle);
	CC_SYNTHESIZE(float, m_fInterval, Interval);
	CC_SYNTHESIZE(float, m_fTime, Time);
};