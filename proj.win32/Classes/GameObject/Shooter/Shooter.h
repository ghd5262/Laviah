#pragma once
#include "../Enemy.h"
#include "../Bullet.h"
//-------------------Random Shooter-------------------
class CRandomShooter : public CEnemy
{
private:
	float m_fShotAngle;
	int m_nBulletColor;

public:
	CRandomShooter(float interval, Vec2 targetVec)
		: CEnemy("enemy.png", 0.f, interval, targetVec)
		, m_fShotAngle(0.f)
	{}
	virtual ~CRandomShooter(){};

	virtual void Execute(float delta = 0.f) override {
		m_fTime += delta;
		if (m_fTime >= m_fInterval){
			m_fInterval = 100;// random<float>(0.0f, 0.5f);
			m_fShotAngle = random<float>(0.f, 360.f);
			m_nBulletColor = random<int>(1, 2);
			CBullet* temp = new CBullet(MakeString("bullet_%d.png", m_nBulletColor), 5.f, m_fShotAngle, 70.0f, getTargetVec());
			addChild(temp);
			temp->setName("SSIBAL");
			m_fTime = 0.f;
		}
	}
};

void RandomShoot() {
}
//----------------------------------------------------
