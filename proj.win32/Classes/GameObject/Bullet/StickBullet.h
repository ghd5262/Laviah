#pragma once
#include "Bullet.h"

class CGameObject;

class CStickBullet : public CBullet {
public:
    /*create를 호출하면 CObjectManager에서 메모리를 받는다.
     받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
    static CStickBullet* create();
    
    virtual void CollisionWithPlayer() override;
    virtual void CollisionWithPlanet() override;
    virtual void CollisionWithBarrier() override;
    virtual void ChangeToCoinOrStar() override;
	virtual void Execute(float delta) override;
    
protected:
    virtual bool init() override;
    
private:
    CStickBullet();
    virtual ~CStickBullet(){};
    
private:
	float m_StayLimitTime;
    bool m_HitWithPlanet;
};