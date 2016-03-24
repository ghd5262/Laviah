#pragma once
#include "Shooter.h"

/* RandomShooter : 360도 사방에서 랜덤으로 미사일을 쏘는 패턴, 미사일은 빠르다*/
class CRandomMissileShooter : public CShooter {

public:
	/* create를 호출하면 operator new가 호출되면서 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다. */
	static CRandomMissileShooter* create(float speed, float interval, int bulletCountAtOneShoot);

	virtual void Execute(float delta) override;

private:
	// interval = Bullet 생성 간격
	CRandomMissileShooter(float speed, float interval, int bulletCountAtOneShoot);
	virtual ~CRandomMissileShooter(){}
};

void RandomMissileShoot(float speed = 600.0f, float interval = 0.1f, int bulletCountAtOneShoot = 1);
