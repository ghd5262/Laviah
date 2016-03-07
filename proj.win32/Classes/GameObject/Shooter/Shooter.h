#pragma once
#include "../Enemy.h"
#include "../Bullet.h"

//-------------------Random Shooter-------------------
class CRandomShooter : public CEnemy {
public:
	/*create를 호출하면 operator new(size_t size, const std::nothrow_t);가 호출되면서 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CRandomShooter* create(float interval);						// interval = Bullet 생성 간격
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
