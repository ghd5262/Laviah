#pragma once
#include "Shooter.h"

/* DoubleScrewShooter : ����� �ð����, �ݽð�������� ���鼭 ��� ����*/
class CDoubleScrewShooter : public CShooter {

public:
	/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
	static CDoubleScrewShooter* create(sSHOOTER_PARAM param);

	virtual void Execute(float delta) override;

private:
	CDoubleScrewShooter(sSHOOTER_PARAM param); // direction = Bullet ����

	virtual ~CDoubleScrewShooter(){}
};
