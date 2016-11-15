#pragma once
#include "Bullet.h"

class CGameObject;

class CStickBullet : public CBullet {
public:
    /*create를 호출하면 CObjectManager에서 메모리를 받는다.
     받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
    static CStickBullet* create(sBULLET_PARAM bulletParam, float angle);
    
    virtual void Execute(float delta = 0.f) override;
    virtual void CollisionWithPlayer() override;
    virtual void CollisionWithPlanet() override;
    virtual void CollisionWithBarrier() override;
    virtual void ChangeToCoinOrStar() override;
    
protected:
    virtual bool init() override;
    
private:
    CStickBullet(sBULLET_PARAM bulletParam, float angle);
    virtual ~CStickBullet(){};
    
private:
    bool m_HitWithPlanet;
};