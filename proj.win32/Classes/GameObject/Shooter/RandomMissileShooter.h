#pragma once
#include "Shooter.h"

/* RandomShooter : 360�� ��濡�� �������� �̻����� ��� ����, �̻����� ������ */
class CRandomMissileShooter : public CShooter {

public:
	/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
	static CRandomMissileShooter* create(sSHOOTER_PARAM param);

	virtual void Execute(float delta) override;
	virtual void ShootOnce() override;

private:
	// interval = Bullet ���� ����
	CRandomMissileShooter(sSHOOTER_PARAM param);
	virtual ~CRandomMissileShooter(){}
};
