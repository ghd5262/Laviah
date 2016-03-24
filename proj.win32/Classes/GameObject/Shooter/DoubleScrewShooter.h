#pragma once
#include "Shooter.h"

/* DoubleScrewShooter : ����� �ð����, �ݽð�������� ���鼭 ��� ����*/
class CDoubleScrewShooter : public CShooter {

public:
	/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
	static CDoubleScrewShooter* create(float speed, float interval, int bulletCountAtOneShoot, eSHOOTER_OPTION direction);

	virtual void Execute(float delta) override;

private:
	CDoubleScrewShooter(
		float speed,
		float interval,
		int bulletCountAtOneShoot,
		eSHOOTER_OPTION direction); // direction = Bullet ����

	virtual ~CDoubleScrewShooter(){}

private:
	eSHOOTER_OPTION m_Direction;
};

void DoubleScrewShoot(
	float speed = 250.0f,
	float interval = 0.1f,
	int bulletCountAtOneShoot = 1,
	eSHOOTER_OPTION direction = eSHOOTER_OPTION_right);
