#pragma once

#include "GameObject.h"
#include "../AI/StateMachine.h"

struct sSPACESHIP_PARAM{
	sSPACESHIP_PARAM(){}
};

class CPlanet;

class CSpaceShip : public CGameObject {
public:
	static CSpaceShip* create(sSPACESHIP_PARAM SpaceshipParam);

	virtual void Execute(float delta = 0.f) override;
    void seek(float delta);
	void FlyAround(float delta);

	void Break(){};
protected:
	virtual bool init() override;
    
	CSpaceShip(sSPACESHIP_PARAM SpaceshipParam);
	virtual ~CSpaceShip();

	CC_SYNTHESIZE(float, m_Speed, Speed);
	CC_SYNTHESIZE(float, m_Distance, Distance);   
	CC_SYNTHESIZE(float, m_ActionTime, ActionTime);
	CC_SYNTHESIZE(int, m_Direction, Direction);    
	CC_SYNTHESIZE(int, m_CurrentAction, CurrentAction);
    CC_SYNTHESIZE(cocos2d::Vec2, m_TargetPos, TargetPos);
	CC_SYNTHESIZE(cocos2d::Vec2, m_RotationVec, RotationVec);
	CC_SYNTHESIZE(sSPACESHIP_PARAM, m_SpaceshipParam, SpaceshipParam);
	CC_SYNTHESIZE(CStateMachine<CSpaceShip>*, m_FSM, FSM);
	CC_SYNTHESIZE(CPlanet*, m_Planet, Planet);

private:
    void createFlameParticle();
    
private:
	cocos2d::ParticleSystemQuad* m_ParticleFlame;
    cocos2d::Sprite* m_Texture;
    cocos2d::Vec2 m_CenterPos;
    cocos2d::Vec2 m_Velocity;
    float m_Time;
};