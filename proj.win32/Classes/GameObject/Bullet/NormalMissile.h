#pragma once
#include "Bullet.h"

class CGameObject;

class CNormalMissile : public CBullet {
public:
	/*create�� ȣ���ϸ� CObjectManager���� �޸𸮸� �޴´�.
	���� �޸𸮴� �޸�Ǯ�� �̸� �����Ǿ��ִ� �޸��̴�.*/
	static CNormalMissile* create(
		sBULLET_PARAM bulletParam,
		float angle,				//bullet �ʱ� ���� 
		float speed);				//bullet �ʱ� �ӵ�

	virtual void Execute(float delta = 0.f) override;
	virtual void Rotation(float dir, float delta) override;
	virtual void CollisionWithPlayer();
	virtual void CollisionWithPlanet();
	virtual void CollisionWithBarrier();
	virtual void ChangeToCoinOrStar();

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CNormalMissile(
		sBULLET_PARAM bulletParam,
		float angle,
		float speed);
	virtual ~CNormalMissile(){};

	CC_SYNTHESIZE(bool, m_bIsTargetMarkCreate, IsTargetMarkCreate);

private:
	Rect m_ScreenRect;

	//�Ҳ� ��ƼŬ
	ParticleSystemQuad* m_pParticleFlame;

	//�浹 ��ƼŬ
	ParticleSystemQuad* m_pParticleCrash;
};
