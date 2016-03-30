#pragma once
#include "Shooter.h"

/* RandomShooter : 360�� ��濡�� �������� �Ѿ��� ��� ����*/
class CRandomShooter : public CShooter {

public:
	/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
	static CRandomShooter* create(sSHOOTER_PARAM param);

	virtual void Execute(float delta) override;

private:
	// interval = Bullet ���� ����
	CRandomShooter(sSHOOTER_PARAM param);
	virtual ~CRandomShooter(){}
};
