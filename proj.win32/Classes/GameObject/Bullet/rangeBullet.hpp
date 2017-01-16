#pragma once
#include "Bullet.h"

class CGameObject;

class CRangeBullet : public CBullet {
public:
    /*create를 호출하면 CObjectManager에서 메모리를 받는다.
     받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
    static CRangeBullet* create();
    CBullet* setRange(float range);
    CBullet* setTarget(float target);
    CBullet* setStayLimitTime(float stayLimitTime);
    virtual void Execute(float delta) override;

protected:
    virtual bool init() override;
    
private:
    void sizeUP(float delta);

    CRangeBullet();
    virtual ~CRangeBullet(){};
    
private:
    float m_Range;
    float m_TargetDistance;
    float m_StayTime;
    float m_StayLimitTime;
    bool m_Movable;
    cocos2d::Vec2 m_PlanetPos;
};