#pragma once

#include "GameObject.h"
#include "../AI/StateMachine.h"
#include <vector>

namespace SPACESHIP{
	static float			BOUNDING_RADIUS = 50.f;
	static float			FLYAWAY_DISTANCE = 3000.f;
	static float			SPEED_MAX = 1500.f;
	static float			MASS = 0.5f;
	static float			ARRIVE_RADIUS = 50.f;
};

struct sSPACESHIP_PARAM{
	sSPACESHIP_PARAM(){}
};

class CBullet;
class CSpaceShip : public CGameObject {
public:
	static CSpaceShip* create(sSPACESHIP_PARAM SpaceshipParam);

	virtual void Execute(float delta = 0.f) override;
	void FlyAround(float delta);
	void FlyAway(float delta);
	void FlyToTouchArea(float delta);
	void Collision();
	void ChangeState(CState<CSpaceShip>* newState)
    { m_FSM->ChangeState(newState); };
    
	CC_SYNTHESIZE(float, m_Speed, Speed);
	CC_SYNTHESIZE(float, m_ActionTime, ActionTime);
	CC_SYNTHESIZE(float, m_Distance, Distance);
	CC_SYNTHESIZE(float, m_AwayAngle, AwayAngle);
	CC_SYNTHESIZE(int, m_Direction, Direction);    
	CC_SYNTHESIZE(bool, m_Arrive, Arrive);
    CC_SYNTHESIZE(cocos2d::Vec2, m_TargetPos, TargetPos);
	CC_SYNTHESIZE(sSPACESHIP_PARAM, m_SpaceshipParam, SpaceshipParam);
	CC_SYNTHESIZE(CStateMachine<CSpaceShip>*, m_FSM, FSM);

private:
	virtual bool init() override;
	void arrive(float delta);
	void seek(float delta);
    void rotateToHead();
    void arriveCheck();
    void createFlameParticle();
    
	CSpaceShip(sSPACESHIP_PARAM SpaceshipParam);
	virtual ~CSpaceShip();

private:
	std::vector<CBullet*>* m_BulletList;
	cocos2d::ParticleSystemQuad* m_ParticleFlame;
    cocos2d::Sprite* m_Texture;
    cocos2d::Vec2 m_CenterPos;
    cocos2d::Vec2 m_Velocity;
    float m_Time;
};