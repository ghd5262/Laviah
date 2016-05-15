#pragma once
#include "../Mover.h"

//enum eSHOOTER_OPTION{
//	eSHOOTER_OPTION_right = 0,
//	eSHOOTER_OPTION_left = 1
//};

//enum eSHOOTER_TYPE{
//
//	// ���ʽ� Ÿ�� ���� 0 ~ 99
//	eSHOOTER_TYPE_bonusTime_0 = 0,
//	eSHOOTER_TYPE_bonusTime_1 = 1,
//	eSHOOTER_TYPE_bonusTime_2 = 2,
//	eSHOOTER_TYPE_bonusTime_3 = 3,
//	eSHOOTER_TYPE_bonusTime_4 = 4,
//	eSHOOTER_TYPE_bonusTime_5 = 5,
//	eSHOOTER_TYPE_bonusTime_6 = 6,
//
//	// �Ϲ� bullet ���� 100 ~ 499
//	eSHOOTER_TYPE_normalBullet  = 100,
//	eSHOOTER_TYPE_screwBullet = 101,
//	eSHOOTER_TYPE_doubleScrewBullet = 102,
//	eSHOOTER_TYPE_normalMissile = 103,
//	eSHOOTER_TYPE_aimingMissile = 104,
//
//	// item ���� 500 ~ 999
//	eSHOOTER_TYPE_item_0 = 500,
//	eSHOOTER_TYPE_item_1 = 501,
//	eSHOOTER_TYPE_item_2 = 502,
//	eSHOOTER_TYPE_item_3 = 503,
//	eSHOOTER_TYPE_item_4 = 504,
//	eSHOOTER_TYPE_item_5 = 505,
//	eSHOOTER_TYPE_item_6 = 506,
//
//	// ��Ÿ 1000 ~
//	eSHOOTER_TYPE_letter = 1000
//};

struct sSHOOTER_PARAM{
	std::string _ShooterName;
	std::string _PatternName;
	float _fStartTime;
	float _fEndTime;
	float _fSpeed;
	float _fAngle;
	float _fInterval;
	int   _nDir;
	int   _nBulletCountAtOneShoot;
	char  _randomShootSymbol;
	bool  _isFly;
	bool  _isAngleRandom;
	bool  _isOneShoot;

	sSHOOTER_PARAM(
		std::string patternName,
		float startTime,
		float endTime,
		float speed,
		float angle,
		float interval,
		int bulletCountAtOneShoot = 1,
		int dir = 1,
		bool isFly = true,
		bool isOneShoot = false,
		bool isAngleRandom = false)
		: _PatternName(patternName)
		, _fStartTime(startTime)
		, _fEndTime(endTime)
		, _fSpeed(speed)
		, _fAngle(angle)
		, _fInterval(interval)
		, _nDir(dir)
		, _nBulletCountAtOneShoot(bulletCountAtOneShoot)
		, _isFly(isFly)
		, _randomShootSymbol('1')
		, _isOneShoot(isOneShoot)
		, _isAngleRandom(isAngleRandom){}
	sSHOOTER_PARAM(){}
};

class CShooter : public CMover {
public:
	virtual void ReturnToMemoryBlock() override;
	
protected:
	virtual void ShootOnce(){};

	// PoolingManager���� �޸𸮸� �Ҵ� �޴´�.
	void* operator new (size_t size, const std::nothrow_t);

	// ���� �޸� ������ memorypooling���� ��������� 
	// �Ҹ��ڸ� ȣ���Ͽ� Node�迭�� �ƴ� �޸𸮵��� �����ϱ� ����
	void operator delete(void* ptr){};

	CShooter(sSHOOTER_PARAM param, float distance)
		: CMover(0.0f)
		, m_ShooterParam(param)
		, m_fIntervalTimer(0.0f)
		, m_fIntervalRandom(0.0f)
		, m_nBulletCountAtOnceRandom(1)
		, m_fDistance(distance){
		m_fTime = m_ShooterParam._fStartTime;
	}
	virtual ~CShooter();

	//getter & setter
	CC_SYNTHESIZE(float, m_fTime, Time);//���� �� ���� �ð�
	CC_SYNTHESIZE(float, m_fIntervalTimer, IntervalTimer)//Interval�� �ð�
	CC_SYNTHESIZE(float, m_fIntervalRandom, IntervalRandom);//�Ѿ��� ��� ���� 0.1 ~ Max ���� ��
	CC_SYNTHESIZE(float, m_fDistance, Distance);// 0 > Distance �̸� bullet�� ���� distance���� �����
	CC_SYNTHESIZE(int, m_nBulletCountAtOnceRandom, BulletCountAtOnceRandom);// �ѹ��� ��� �Ѿ��� �� 1 ~ Max ���� ��
	CC_SYNTHESIZE(sSHOOTER_PARAM, m_ShooterParam, ShooterParam);
};