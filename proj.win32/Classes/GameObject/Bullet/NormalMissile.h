#pragma once
#include "Bullet.h"

class CNormalMissile : public CBullet {
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
	static CNormalMissile* create();

	virtual void Rotation(float dir, float delta) override;
	virtual void CollisionWithPlayer() override;
	virtual void CollisionWithPlanet() override;
	virtual void CollisionWithBarrier() override;
	virtual void ChangeToCoinOrStar() override;
	virtual void Execute(float delta) override;
	virtual CBullet* build() override;

protected:
	virtual bool init() override;

private:
    void createTargetLine();
    void createParticle_Flame();
    void createParticle_Explosion();

	CNormalMissile();
	virtual ~CNormalMissile(){};

	CC_SYNTHESIZE(bool, m_bIsTargetMarkCreate, IsTargetMarkCreate);

private:
	cocos2d::Rect m_ScreenRect;

	//불꽃 파티클
	cocos2d::ParticleSystemQuad* m_pParticleFlame;

	//충돌 파티클
	cocos2d::ParticleSystemQuad* m_pParticleCrash;
};
