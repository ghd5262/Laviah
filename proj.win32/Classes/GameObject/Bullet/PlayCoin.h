#pragma once
#include "Bullet.h"

enum eCOIN_TYPE;
class CPlayCoin : public CBullet
{
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CPlayCoin* create(
		sBULLET_PARAM bulletParam,
		float angle,				//coin 초기 각도 
		float speed,				//coin 초기 속도
		Vec2 createPosition = Vec2(0.0f, 0.0f));

	virtual void Execute(float delta = 0.f) override;
	virtual void CollisionWithPlayer();
	virtual void CollisionWithPlanet();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CPlayCoin(
		sBULLET_PARAM bulletParam,
		float angle,
		float speed,
		Vec2 createPosition );

	virtual ~CPlayCoin(){};

private:
	Vec2 m_CreatePos;
	float m_fCoinValue;
	//충돌 파티클
	ParticleSystemQuad* m_pParticleCrash;
};

