#pragma once
#include "Shooter.h"

/* DoubleScrewShooter : 더블로 시계방향, 반시계방향으로 돌면서 쏘는 패턴*/
class CDoubleScrewShooter : public CShooter {

public:
	/* create를 호출하면 operator new가 호출되면서 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다. */
	static CDoubleScrewShooter* create(sSHOOTER_PARAM param);

	virtual void Execute(float delta) override;

private:
	CDoubleScrewShooter(sSHOOTER_PARAM param); // direction = Bullet 방향

	virtual ~CDoubleScrewShooter(){}
};
