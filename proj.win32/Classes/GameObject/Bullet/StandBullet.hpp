#pragma once
#include "Bullet.h"

class CGameObject;

class CStandBullet : public CBullet {
public:
    /*create를 호출하면 CObjectManager에서 메모리를 받는다.
     받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
    static CStandBullet* create();
    void StandUp(float delta);
    
    virtual void CollisionWithPlayer() override;
    virtual void CollisionWithBarrier() override;
    virtual void ChangeToCoin() override;
	virtual void ChangeToStar() override;
    virtual void Execute(float delta) override;
    virtual CBullet* build() override;
    
protected:
    virtual bool init() override;
    
private:
    CStandBullet();
    virtual ~CStandBullet(){};
    
private:
    float m_StayLimitTime;
    bool m_StandUpComplete;
    cocos2d::Vec2 m_PlanetPos;
};