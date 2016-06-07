#pragma once
#include "Bullet.h"

class CGameObject;

class CNormalBullet : public CBullet {
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CNormalBullet* create(
		sBULLET_PARAM bulletParam,
		float angle,				//bullet 초기 각도 
		float speed);				//bullet 초기 속도

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer();
	virtual void CollisionWithPlanet();
	virtual void CollisionWithBarrier();
	virtual void ChangeToCoinOrStar();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CNormalBullet(
		sBULLET_PARAM bulletParam,
		float angle,
		float speed);
	virtual ~CNormalBullet(){};
};