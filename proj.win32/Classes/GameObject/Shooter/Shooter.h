#pragma once
#include "../Enemy.h"
#include "../Bullet.h"

//-------------------Random Shooter-------------------
class CRandomShooter : public CEnemy {
public:
	/*create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CRandomShooter* create(float interval);						// Bullet ���� ����
	virtual void Execute(float delta = 0.f) override;

private:
	CRandomShooter(float interval);
	virtual ~CRandomShooter();
	void* operator new (size_t size, const std::nothrow_t);

private:
	float m_fRandomInterval;
	float m_fShotAngle;
	int m_nBulletColor;
};

void RandomShoot();
//----------------------------------------------------
