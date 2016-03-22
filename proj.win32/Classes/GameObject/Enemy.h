#pragma once
#include "Mover.h"

class CEnemy : public CMover {
public:
	virtual void ReturnToMemoryBlock() override;
	
protected:
	// PoolingManager에서 메모리를 할당 받는다.
	void* operator new (size_t size, const std::nothrow_t);

	// 실제 메모리 해제는 memorypooling에서 담당하지만 
	// 소멸자를 호출하여 Node계열이 아닌 메모리들을 삭제하기 위함
	void operator delete(void* ptr){};

	CEnemy(float interval, float shotSpeed) : CMover(0.0f), m_fShotSpeed(shotSpeed), m_fShotAngle(0.0f), m_fInterval(interval), m_fTime(0.0f){}
	virtual ~CEnemy();

	//getter & setter
	CC_SYNTHESIZE(float, m_fShotSpeed, ShotSpeed);
	CC_SYNTHESIZE(float, m_fShotAngle, ShotAngle);
	CC_SYNTHESIZE(float, m_fInterval, Interval);
	CC_SYNTHESIZE(float, m_fTime, Time);
};