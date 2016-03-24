#pragma once
#include "Shooter.h"

/* RandomShooter : 360�� ��濡�� �������� �Ѿ��� ��� ����*/
class CRandomShooter : public CShooter {

public:
	/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
	static CRandomShooter* create(float speed, float interval, int bulletCountAtOneShoot);

	virtual void Execute(float delta) override;

private:
	// interval = Bullet ���� ����
	CRandomShooter(float speed, float interval, int bulletCountAtOneShoot);
	virtual ~CRandomShooter(){}
};

void RandomShoot(float speed = 250.0f, float interval = 0.1f, int bulletCountAtOneShoot = 1);
