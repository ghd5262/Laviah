#pragma once
#include "Mover.h"

class CEnemy : public CMover {
public:
	CEnemy(std::string textureName, float boundingRadius, float interval, Vec2 targetVec);
	virtual ~CEnemy();

	void* operator new (size_t n);
	void operator delete (void* p);

protected:
	//getter & setter
	CC_SYNTHESIZE(float, m_fInterval, Interval);
	CC_SYNTHESIZE(float, m_fTime, Time);
	CC_SYNTHESIZE(Vec2, m_TargetVec, TargetVec)
};