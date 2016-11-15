#pragma once
#include "Bullet.h"

enum eCOIN_TYPE;
class CPlayCoin : public CBullet
{
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CPlayCoin* create(sBULLET_PARAM bulletParam, float angle, Vec2 createPosition = Vec2::ZERO);

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer() override;
	virtual void CollisionWithPlanet() override;

protected:
	virtual bool init() override;

private:
	CPlayCoin(sBULLET_PARAM bulletParam, float angle, Vec2 createPosition );
	virtual ~CPlayCoin(){};

private:
	Vec2 m_CreatePos;
	float m_fCoinValue;
	//충돌 파티클
	ParticleSystemQuad* m_pParticleCrash;
};

