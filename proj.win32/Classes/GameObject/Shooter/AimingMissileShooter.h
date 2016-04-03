#pragma once
#include "Shooter.h"

/* AimingMissileShooter : ĳ���͸� �����ϰ� �̻����� ��� ����*/
class CAimingMissileShooter : public CShooter {
public:
	/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
	static CAimingMissileShooter* create(sSHOOTER_PARAM param);

	virtual void Execute(float delta) override;
	virtual void ShootOnce() override;

private:
	// interval = Bullet ���� ����
	CAimingMissileShooter(sSHOOTER_PARAM param);
	virtual ~CAimingMissileShooter(){}
};
