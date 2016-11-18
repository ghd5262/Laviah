#pragma once
#include "Bullet.h"

enum eCOIN_TYPE;
class CPlayCoin : public CBullet
{
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CPlayCoin* create();

	virtual void CollisionWithPlayer() override;
	virtual void CollisionWithPlanet() override;

protected:
	virtual bool init() override;

private:
	CPlayCoin();
	virtual ~CPlayCoin(){};

private:
	float m_fCoinValue;
	//충돌 파티클
	cocos2d::ParticleSystemQuad* m_pParticleCrash;
};

