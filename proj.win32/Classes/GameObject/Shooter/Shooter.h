#pragma once
#include "../Mover.h"

enum eSHOOTER_OPTION{
	eSHOOTER_OPTION_right = 0,
	eSHOOTER_OPTION_left = 1
};

enum eSHOOTER_TYPE{

	// 보너스 타임 패턴 0 ~ 99
	eSHOOTER_TYPE_bonusTime_0 = 0,
	eSHOOTER_TYPE_bonusTime_1 = 1,
	eSHOOTER_TYPE_bonusTime_2 = 2,
	eSHOOTER_TYPE_bonusTime_3 = 3,
	eSHOOTER_TYPE_bonusTime_4 = 4,
	eSHOOTER_TYPE_bonusTime_5 = 5,
	eSHOOTER_TYPE_bonusTime_6 = 6,

	// 일반 bullet 패턴 100 ~ 499
	eSHOOTER_TYPE_normalBullet  = 100,
	eSHOOTER_TYPE_screwBullet = 101,
	eSHOOTER_TYPE_doubleScrewBullet = 102,
	eSHOOTER_TYPE_normalMissile = 103,
	eSHOOTER_TYPE_aimingMissile = 104,

	// item 패턴 500 ~ 999
	eSHOOTER_TYPE_item_0 = 500,
	eSHOOTER_TYPE_item_1 = 501,
	eSHOOTER_TYPE_item_2 = 502,
	eSHOOTER_TYPE_item_3 = 503,
	eSHOOTER_TYPE_item_4 = 504,
	eSHOOTER_TYPE_item_5 = 505,
	eSHOOTER_TYPE_item_6 = 506,

	// 기타 1000 ~
	eSHOOTER_TYPE_letter = 1000
};

struct sSHOOTER_PARAM{
	eSHOOTER_TYPE _ShooterType;
	float _fStartTime;
	float _fEndTime;
	float _fSpeed;
	float _fAngle;
	float _fInterval;
	int   _nDir;
	int   _nBulletCountAtOneShoot;
	bool  _isFly;

	sSHOOTER_PARAM(
		eSHOOTER_TYPE shooterType,
		float startTime,
		float endTime,
		float speed,
		float angle,
		float interval,
		int bulletCountAtOneShoot = 1,
		int dir = 1,
		bool isFly = 1)
		: _ShooterType(shooterType)
		, _fStartTime(startTime)
		, _fEndTime(endTime)
		, _fSpeed(speed)
		, _fAngle(angle)
		, _fInterval(interval)
		, _nDir(dir)
		, _nBulletCountAtOneShoot(bulletCountAtOneShoot)
		, _isFly(isFly){}
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

	CShooter(sSHOOTER_PARAM param)
		: CMover(0.0f)
		, m_ShooterParam(param)
		, m_fTime(0.0f)
		, m_fIntervalRandom(1.0f)
		, m_nBulletCountAtOnceRandom(1){}
	virtual ~CShooter();

	//getter & setter
	CC_SYNTHESIZE(float, m_fTime, Time);//시간
	CC_SYNTHESIZE(float, m_fIntervalRandom, IntervalRandom);//총알을 쏘는 간격 0.1 ~ Max 사이 값
	CC_SYNTHESIZE(int, m_nBulletCountAtOnceRandom, BulletCountAtOnceRandom);// 한번에 쏘는 총알의 수 1 ~ Max 사이 값
	CC_SYNTHESIZE(sSHOOTER_PARAM, m_ShooterParam, ShooterParam);
};