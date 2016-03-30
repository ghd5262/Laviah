#pragma once
#include "Shooter.h"

/* ScrewShooter : �ð����, �ݽð�������� ���鼭 ��� ����*/
class CScrewShooter : public CShooter {

public:
	/* create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�. */
	static CScrewShooter* create(sSHOOTER_PARAM param);

	virtual void Execute(float delta) override;

private:
	// direction = Bullet ����
	CScrewShooter(sSHOOTER_PARAM param);
	virtual ~CScrewShooter(){}
};
