#pragma once
#include "Shooter.h"

/* RandomShooter : 360�� ��濡�� �������� �Ѿ��� ��� ����*/
class CRandomShooter : public CShooter {

public:
	/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
	static CRandomShooter* create(sSHOOTER_PARAM param, float distance = -1.f);

	virtual void Execute(float delta) override;
	virtual void ShootOnce() override;

private:
	// interval = Bullet ���� ����
	CRandomShooter(sSHOOTER_PARAM param, float distance);
	virtual ~CRandomShooter(){}
};
