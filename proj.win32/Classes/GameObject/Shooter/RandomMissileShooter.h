#pragma once
#include "Shooter.h"

/* RandomShooter : 360�� ��濡�� �������� �̻����� ��� ����, �̻����� ������*/
class CRandomMissileShooter : public CShooter {

public:
	/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
	static CRandomMissileShooter* create(float speed, float interval, int bulletCountAtOneShoot);

	virtual void Execute(float delta) override;

private:
	// interval = Bullet ���� ����
	CRandomMissileShooter(float speed, float interval, int bulletCountAtOneShoot);
	virtual ~CRandomMissileShooter(){}
};

void RandomMissileShoot(float speed = 600.0f, float interval = 0.1f, int bulletCountAtOneShoot = 1);
