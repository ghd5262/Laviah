#pragma once
#include "../Mover.h"

enum eSHOOTER_OPTION{
	eSHOOTER_OPTION_right = 0,
	eSHOOTER_OPTION_left = 1
};

class CShooter : public CMover {
public:
	virtual void ReturnToMemoryBlock() override;
	
protected:
	// PoolingManager에서 메모리를 할당 받는다.
	void* operator new (size_t size, const std::nothrow_t);

	// 실제 메모리 해제는 memorypooling에서 담당하지만 
	// 소멸자를 호출하여 Node계열이 아닌 메모리들을 삭제하기 위함
	void operator delete(void* ptr){};

	CShooter(float interval, float shotSpeed, int bulletCountAtOneShoot)
		: CMover(0.0f)
		, m_fShotSpeed(shotSpeed)
		, m_fShotAngle(0.0f)
		, m_fTime(0.0f)
		, m_fInterval(interval)
		, m_fIntervalRandom(1.0f)
		, m_nBulletCountAtOneShoot(bulletCountAtOneShoot)
		, m_nBulletCountAtOnceRandom(1){}
	virtual ~CShooter();

	//getter & setter
	CC_SYNTHESIZE(float, m_fShotSpeed, ShotSpeed);//총알의 속도
	CC_SYNTHESIZE(float, m_fShotAngle, ShotAngle);//총알을 쏘는 각도
	CC_SYNTHESIZE(float, m_fTime, Time);//시간
	CC_SYNTHESIZE(float, m_fInterval, Interval);//총알을 쏘는 간격 max값
	CC_SYNTHESIZE(float, m_fIntervalRandom, IntervalRandom);//총알을 쏘는 간격 0.1 ~ Max 사이 값
	CC_SYNTHESIZE(int, m_nBulletCountAtOneShoot, BulletCountAtOneShoot);// 한번에 쏘는 총알의 수 max값
	CC_SYNTHESIZE(int, m_nBulletCountAtOnceRandom, BulletCountAtOnceRandom);// 한번에 쏘는 총알의 수 1 ~ Max 사이 값

		
			
};