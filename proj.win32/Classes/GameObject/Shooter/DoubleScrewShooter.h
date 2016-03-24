#pragma once
#include "Shooter.h"

/* DoubleScrewShooter : 더블로 시계방향, 반시계방향으로 돌면서 쏘는 패턴*/
class CDoubleScrewShooter : public CShooter {

public:
	/* create를 호출하면 operator new가 호출되면서 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다. */
	static CDoubleScrewShooter* create(float speed, float interval, int bulletCountAtOneShoot, eSHOOTER_OPTION direction);

	virtual void Execute(float delta) override;

private:
	CDoubleScrewShooter(
		float speed,
		float interval,
		int bulletCountAtOneShoot,
		eSHOOTER_OPTION direction); // direction = Bullet 방향

	virtual ~CDoubleScrewShooter(){}

private:
	eSHOOTER_OPTION m_Direction;
};

void DoubleScrewShoot(
	float speed = 250.0f,
	float interval = 0.1f,
	int bulletCountAtOneShoot = 1,
	eSHOOTER_OPTION direction = eSHOOTER_OPTION_right);
