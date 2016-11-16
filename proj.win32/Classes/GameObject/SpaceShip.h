#pragma once

#include "GameObject.h"
#include "../AI/StateMachine.h"

struct sSPACESHIP_PARAM{
	sSPACESHIP_PARAM(){}
};

class CPlanet;

class CSpaceShip : public CGameObject {
public:
	static CSpaceShip* create(sSPACESHIP_PARAM SpaceshipParam,
		float FlySpeed,
		float distance);

	virtual void Execute(float delta = 0.f) override;

	void FlyAround(float delta);

	void Break(){};
protected:
	virtual bool init() override;
	virtual bool initVariable() override;

	CSpaceShip(sSPACESHIP_PARAM SpaceshipParam, float walkingSpeed, float distance);
	virtual ~CSpaceShip();

	CC_SYNTHESIZE(float, m_fFlySpeed, FlySpeed);
	CC_SYNTHESIZE(float, m_fDistance, Distance);   
	CC_SYNTHESIZE(float, m_fActionTime, ActionTime);
	CC_SYNTHESIZE(int, m_Direction, Direction);    
	CC_SYNTHESIZE(int, m_CurrentAction, CurrentAction);
	CC_SYNTHESIZE(cocos2d::Vec2, m_RotationVec, RotationVec);
	CC_SYNTHESIZE(sSPACESHIP_PARAM, m_SpaceshipParam, SpaceshipParam);
	CC_SYNTHESIZE(CStateMachine<CSpaceShip>*, m_FSM, FSM);
	CC_SYNTHESIZE(CPlanet*, m_pPlanet, Planet);

private:
	cocos2d::ParticleSystemQuad* m_pParticleFlame;
};