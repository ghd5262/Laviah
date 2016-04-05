#pragma once
#include "Shooter.h"

/* RandomShooter : 360도 사방에서 랜덤으로 총알을 쏘는 패턴*/
class CRandomShooter : public CShooter {

public:
	/* create를 호출하면 operator new가 호출되면서 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다. */
	static CRandomShooter* create(sSHOOTER_PARAM param);

	virtual void Execute(float delta) override;
	virtual void ShootOnce() override;

	// 초기 각도와 거리를 받아서 Shoot
	void ShootWithPosition(sSHOOTER_PARAM param, float angle, float distance = 0.0f) override;

private:
	// interval = Bullet 생성 간격
	CRandomShooter(sSHOOTER_PARAM param);
	virtual ~CRandomShooter(){}
};
