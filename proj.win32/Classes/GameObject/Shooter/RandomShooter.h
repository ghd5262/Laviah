#pragma once
#include "Shooter.h"

/* RandomShooter : 360�� ��濡�� �������� �Ѿ��� ��� ����*/
class CRandomShooter : public CShooter {

public:
	/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
	static CRandomShooter* create(sSHOOTER_PARAM param);

	virtual void Execute(float delta) override;
	virtual void ShootOnce() override;

	// �ʱ� ������ �Ÿ��� �޾Ƽ� Shoot
	void ShootWithPosition(sSHOOTER_PARAM param, float angle, float distance = 0.0f) override;

private:
	// interval = Bullet ���� ����
	CRandomShooter(sSHOOTER_PARAM param);
	virtual ~CRandomShooter(){}
};
