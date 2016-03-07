#pragma once
#include "../Enemy.h"
#include "../Bullet.h"

//-------------------Random Shooter-------------------
class CRandomShooter : public CEnemy {
public:
	/*create�� ȣ���ϸ� operator new(size_t size, const std::nothrow_t);�� ȣ��Ǹ鼭 CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CRandomShooter* create(float interval);						// interval = Bullet ���� ����
	virtual void Execute(float delta = 0.f) override;
	void* operator new (size_t size, const std::nothrow_t);
	CRandomShooter() : m_fRandomInterval(0.0f), m_fShotAngle(0.0f), m_nBulletColor(1){};
	virtual ~CRandomShooter(){}

private:
	bool initVariable(float interval);

private:
	float m_fRandomInterval;
	float m_fShotAngle;
	int m_nBulletColor;
};

void RandomShoot();
//----------------------------------------------------
