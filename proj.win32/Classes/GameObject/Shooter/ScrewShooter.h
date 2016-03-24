#pragma once
#include "Shooter.h"

/* ScrewShooter : 시계방향, 반시계방향으로 돌면서 쏘는 패턴*/
class CScrewShooter : public CShooter {

public:
	/* create를 호출하면 operator new가 호출되면서 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다. */
	static CScrewShooter* create(
		float speed,
		float interval,
		int bulletCountAtOneShoot,
		eSHOOTER_OPTION direction);

	virtual void Execute(float delta) override;

private:
	// direction = Bullet 방향
	CScrewShooter(
		float speed,
		float interval,
		int bulletCountAtOneShoot,
		eSHOOTER_OPTION direction);
	virtual ~CScrewShooter(){}

private:
	eSHOOTER_OPTION m_Direction;
};

void ScrewShoot(
	float speed = 250.0f,
	float interval = 0.1f,
	int bulletCountAtOneShoot = 1,
	eSHOOTER_OPTION direction = eSHOOTER_OPTION_right);
