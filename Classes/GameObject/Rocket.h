#pragma once
#include "GameObject.h"
#include "../AI/StateMachine.h"
#include "../DataManager/RocketDataManager.hpp"
#include <vector>

namespace ROCKET_DEFINE{
	static float			BOUNDING_RADIUS = 80.f;
    static float            FLYAROUND_DISTANCE = 500.f;
	static float			FLYAWAY_DISTANCE = 3000.f;
	static float			SPEED_MAX = 1500.f;
    static float            SPEED = 350.f;
    static float			MASS = 0.5f;
	static float			ARRIVE_RADIUS = 10.f;
};

class CBullet;
class CPlayer;
class CMyButton;
class CRocket : public CGameObject {
	typedef std::function<void(cocos2d::Node*)> ARRIVE_CALLBACK;
public:
	static CRocket* create();

	virtual void Execute(float delta = 0.f) override;
    void Fly(float speed);
    void FlyAround(float delta);
	void FlyAway(float delta);
	void FlyToTouchArea(float delta);
	void FlyToTarget(float delta);
    void ComebackHome();
	void CollisionCheckAtHome();
    void BonusTimeBegan();
    void BonusTimeEnd();
	void ChangeState(CState<CRocket>* newState){ m_FSM->ChangeState(newState); }
	void Gift();

	void setArriveCallback(const ARRIVE_CALLBACK& callback){ m_ArriveCallback = callback; }
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
	CC_SYNTHESIZE(const sROCKET_PARAM*, m_RocketParam, RocketParam);
	CC_SYNTHESIZE(cocos2d::Vec2, m_Velocity, Velocity);
    CC_SYNTHESIZE(CPlayer*, m_Player, Player);
private:
	virtual bool init() override;
	void arrive(float delta);
	void seek(float delta);
    void rotateToHead();
    void arriveCheck();
    void createFlameParticle();
    
	CRocket();
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
	CMyButton* m_Gift;
    float m_FlyLimitMax;
    float m_FlyLimitMin;
    float m_Time;
};