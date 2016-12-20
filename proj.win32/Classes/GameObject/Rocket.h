#pragma once

#include "GameObject.h"
#include "../AI/StateMachine.h"
#include <vector>

namespace ROCKET{
	static float			BOUNDING_RADIUS = 80.f;
    static float            FLYAROUND_DISTANCE = 500.f;
	static float			FLYAWAY_DISTANCE = 3000.f;
	static float			SPEED_MAX = 1500.f;
    static float            SPEED = 350.f;
    static float			MASS = 0.5f;
	static float			ARRIVE_RADIUS = 30.f;
};

struct sROCKET_PARAM{
	sROCKET_PARAM(){}
};

class CBullet;
class CRocket : public CGameObject {
	typedef std::function<void(cocos2d::Node*)> ARRIVE_CALLBACK;
public:
	static CRocket* create(sROCKET_PARAM RocketParam);

	virtual void Execute(float delta = 0.f) override;
    void Fly(float speed, float delta);
    void FlyAround(float delta);
	void FlyAway(float delta);
	void FlyToTouchArea(float delta);
	void FlyToTarget(float delta);
	void CollisionCheckAtHome();
    void BonusTimeBegan();
    void BonusTimeEnd();
	void ChangeState(CState<CRocket>* newState){ m_FSM->ChangeState(newState); }
    
	void setArriveCallback(const ARRIVE_CALLBACK& callback){ m_ArriveCallback = callback;
    }
    cocos2d::Vec2 getTargetPos() const { return m_TargetPos; }
    void setTargetPos(cocos2d::Vec2 pos) {
        if(pos == m_TargetPos) return;
        m_TargetPos = pos;
        m_Arrive = false;
    }
    
	CC_SYNTHESIZE(float, m_Speed, Speed);
	CC_SYNTHESIZE(float, m_Distance, Distance);
	CC_SYNTHESIZE(float, m_AwayAngle, AwayAngle);
	CC_SYNTHESIZE(int, m_Direction, Direction);    
	CC_SYNTHESIZE(bool, m_Arrive, Arrive);
	CC_SYNTHESIZE(sROCKET_PARAM, m_RocketParam, RocketParam);
	CC_SYNTHESIZE(cocos2d::Vec2, m_Velocity, Velocity);

private:
	virtual bool init() override;
	void arrive(float delta);
	void seek(float delta);
    void rotateToHead();
    void arriveCheck();
    void createFlameParticle();
    
	CRocket(sROCKET_PARAM RocketParam);
	virtual ~CRocket();

private:
    std::shared_ptr<CStateMachine<CRocket>> m_FSM;
    ARRIVE_CALLBACK m_ArriveCallback;
	std::vector<CBullet*>* m_BulletList;
	cocos2d::ParticleSystemQuad* m_ParticleFlame;
    cocos2d::Sprite* m_Texture;
    cocos2d::Vec2 m_CenterPos;
    cocos2d::Vec2 m_PlayerPos;
    cocos2d::Vec2 m_TargetPos;
    float m_FlyLimitMax;
    float m_FlyLimitMin;
    float m_Time;
};