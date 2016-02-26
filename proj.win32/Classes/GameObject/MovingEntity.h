#pragma once

#include "GameObject.h"

class CMovingEntity : public CGameObject
{
public:
	CMovingEntity(Vec2 position,
		double radius,
		Vec2 velocityVec,
		double max_speed,
		Vec2 headingVec,
		double mass,
		double turn_rate,
		double max_force) 
		: CGameObject(CGameObject::getM_NextValidID())
		, m_velocityVec(velocityVec)
		, m_headingVec(headingVec)
		, m_sideVec(m_headingVec.getPerp())
		, m_dMass(mass)
		, m_dMaxForce(max_force)
		, m_dMaxSpeed(max_speed)
		, m_dMaxTurnRate(turn_rate)
	{
		setPosition(position);
		m_dBoundingRadius = radius;
	}
	virtual ~CMovingEntity();

public:
	bool rotateHeadingToFacePosition(Vec2 target);
	bool isSpeedMaxedOut() const { return (m_dMaxSpeed * m_dMaxSpeed) >= m_velocityVec.getLengthSq(); }
	double speed() const { return m_velocityVec.getLength(); }
	double speedSq() const { return m_velocityVec.getLengthSq(); }
	
protected:
	///getter & setter
	CC_SYNTHESIZE(Vec2, m_velocityVec, VelocityVec);
	CC_SYNTHESIZE(Vec2, m_sideVec, SideVec);
	CC_SYNTHESIZE(Vec2, m_headingVec, HeadingVec);
	CC_SYNTHESIZE(double, m_dMaxTurnRate, MaxTurnRate);
	CC_SYNTHESIZE(double, m_dMaxForce, MaxForce);
	CC_SYNTHESIZE(double, m_dMaxSpeed, MaxSpeed);
	CC_SYNTHESIZE(double, m_dMass, Mass);

};

