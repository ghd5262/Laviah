#pragma once
#include "Shooter.h"

/* ScrewShooter : �ð����, �ݽð�������� ���鼭 ��� ����*/
class CScrewShooter : public CShooter {

public:
	/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
	static CScrewShooter* create(
		float speed,
		float interval,
		int bulletCountAtOneShoot,
		eSHOOTER_OPTION direction);

	virtual void Execute(float delta) override;

private:
	// direction = Bullet ����
	CScrewShooter(
		float speed,
		float interval,
		int bulletCountAtOneShoot,
		eSHOOTER_OPTION direction);
	virtual ~CScrewShooter(){}

private:
	eSHOOTER_OPTION m_Direction;
};

void ScrewShoot(
	float speed = 250.0f,
	float interval = 0.1f,
	int bulletCountAtOneShoot = 1,
	eSHOOTER_OPTION direction = eSHOOTER_OPTION_right);
