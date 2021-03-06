#pragma once
#include "Bullet.h"

class CGameObject;

class CTargetMark : public CBullet {
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
    static CTargetMark* create();
    
    CTargetMark* setBullet(CBullet* bullet);
    CTargetMark* build() override;
    
	virtual void Execute(float delta = 0.f) override;

private:
    void setParticle();
    void setOpacityByTimer();
    
	CTargetMark();
	virtual ~CTargetMark();

private:
	CBullet* m_Bullet;              // owner missile
	float m_DeleteTime;             // 미사일 도착시간 =  target
	
	//파티클
	cocos2d::ParticleSystemQuad* m_Particle;
};