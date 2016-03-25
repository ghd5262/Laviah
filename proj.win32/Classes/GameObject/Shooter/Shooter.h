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
	// PoolingManager���� �޸𸮸� �Ҵ� �޴´�.
	void* operator new (size_t size, const std::nothrow_t);

	// ���� �޸� ������ memorypooling���� ��������� 
	// �Ҹ��ڸ� ȣ���Ͽ� Node�迭�� �ƴ� �޸𸮵��� �����ϱ� ����
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
	CC_SYNTHESIZE(float, m_fShotSpeed, ShotSpeed);//�Ѿ��� �ӵ�
	CC_SYNTHESIZE(float, m_fShotAngle, ShotAngle);//�Ѿ��� ��� ����
	CC_SYNTHESIZE(float, m_fTime, Time);//�ð�
	CC_SYNTHESIZE(float, m_fInterval, Interval);//�Ѿ��� ��� ���� max��
	CC_SYNTHESIZE(float, m_fIntervalRandom, IntervalRandom);//�Ѿ��� ��� ���� 0.1 ~ Max ���� ��
	CC_SYNTHESIZE(int, m_nBulletCountAtOneShoot, BulletCountAtOneShoot);// �ѹ��� ��� �Ѿ��� �� max��
	CC_SYNTHESIZE(int, m_nBulletCountAtOnceRandom, BulletCountAtOnceRandom);// �ѹ��� ��� �Ѿ��� �� 1 ~ Max ���� ��

		
			
};