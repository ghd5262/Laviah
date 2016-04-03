#pragma once
#include "Shooter.h"

/* AimingMissileShooter : 캐릭터를 조준하고 미사일을 쏘는 패턴*/
class CAimingMissileShooter : public CShooter {
public:
	/* create를 호출하면 operator new가 호출되면서 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다. */
	static CAimingMissileShooter* create(sSHOOTER_PARAM param);

	virtual void Execute(float delta) override;
	virtual void ShootOnce() override;

private:
	// interval = Bullet 생성 간격
	CAimingMissileShooter(sSHOOTER_PARAM param);
	virtual ~CAimingMissileShooter(){}
};
