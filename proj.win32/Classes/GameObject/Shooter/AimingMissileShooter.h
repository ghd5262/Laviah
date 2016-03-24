#pragma once
#include "Shooter.h"

/* AimingMissileShooter : ĳ���͸� �����ϰ� �̻����� ��� ����*/
class CAimingMissileShooter : public CShooter {
public:
	/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
	static CAimingMissileShooter* create(float speed, float interval);

	virtual void Execute(float delta) override;

private:
	// interval = Bullet ���� ����
	CAimingMissileShooter(float speed, float interval);
	virtual ~CAimingMissileShooter(){}
};

void AimingMissileShoot(float speed = 1200.0f, float interval = 0.1f);
