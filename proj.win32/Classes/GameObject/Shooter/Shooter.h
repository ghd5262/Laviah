#pragma once
#include "../Enemy.h"
#include "../Bullet.h"

//--------------------------Random Shooter--------------------------
class CRandomShooter : public CEnemy {
public:
	/*create�� ȣ���ϸ� operator new�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CRandomShooter* create(float interval);						// interval = Bullet ���� ����
	virtual void Execute(float delta = 0.f) override;

private:
	void* operator new (size_t size, const std::nothrow_t);
	CRandomShooter(float interval);
	virtual ~CRandomShooter(){}

private:
	float m_fRandomInterval;
	float m_fShotAngle;
	int m_nBulletColor;
};

void RandomShoot();
//------------------------------------------------------------------
