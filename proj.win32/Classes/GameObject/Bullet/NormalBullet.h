#pragma once
#include "Bullet.h"

class CGameObject;

class CNormalBullet : public CBullet {
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CNormalBullet* create(sBULLET_PARAM bulletParam, float angle);

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer() override;
	virtual void CollisionWithPlanet() override;
	virtual void CollisionWithBarrier() override;
	virtual void ChangeToCoinOrStar() override;

protected:
	virtual bool init() override;

private:
	CNormalBullet(sBULLET_PARAM bulletParam, float angle);
	virtual ~CNormalBullet(){};

private:
	//충돌 파티클
	ParticleSystemQuad* m_pParticleCrash;
};