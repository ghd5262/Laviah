#pragma once

#include "../Common/HSHUtility.h"

class CBullet;

typedef enum eBEHAVIOR_TYPE{
	eBEHAVIOR_TYPE_none = 0x0000,
	eBEHAVIOR_TYPE_seek = 0x0001,
	eBEHAVIOR_TYPE_arrive = 0x0002,
	eBEHAVIOR_TYPE_interpose = 0x0004
};

typedef enum eDECELERATION{
	eDECELERATION_fast = 1,
	eDECELERATION_normal = 2,
	eDECELERATION_slow = 3,
};

class CSteeringBehaviors
{
public:
	CSteeringBehaviors(CBullet* bullet);
	virtual ~CSteeringBehaviors(){};

public:
	// 모든 조종힘들을 합치는 연산 함수
	Vec2 CalculateBehaviorVector();

	void SeekOn(){ m_nFlags |= eBEHAVIOR_TYPE_seek; }
	void ArriveOn(){ m_nFlags |= eBEHAVIOR_TYPE_arrive; }
	void InterposeOn(double d){ m_nFlags |= eBEHAVIOR_TYPE_interpose; m_dInterposeDist = d; }

	void SeekOff()  { if (on(eBEHAVIOR_TYPE_seek))   m_nFlags ^= eBEHAVIOR_TYPE_seek; }
	void ArriveOff(){ if (on(eBEHAVIOR_TYPE_arrive)) m_nFlags ^= eBEHAVIOR_TYPE_arrive; }
	void InterposeOff(){ if (on(eBEHAVIOR_TYPE_interpose)) m_nFlags ^= eBEHAVIOR_TYPE_interpose; }

	bool SeekIsOn(){ return on(eBEHAVIOR_TYPE_seek); }
	bool ArriveIsOn(){ return on(eBEHAVIOR_TYPE_arrive); }
	bool InterposeIsOn(){ return on(eBEHAVIOR_TYPE_interpose); }

protected:

	//getter & setter
	CC_SYNTHESIZE(Vec2, m_targetVec, TargetVec);
	CC_SYNTHESIZE(Vec2, m_steeringVec, SteeringVec);
	CC_SYNTHESIZE(double, m_dInterposeDist, InterPoseDist);
	CC_SYNTHESIZE(double, m_dMultSeparation, MultSeparation);
	CC_SYNTHESIZE(double, m_dViewDistance, ViewDistance);

private:
	Vec2 seek(Vec2 target);
	Vec2 arrive(Vec2 target, eDECELERATION deceleration);
	Vec2 interpose(Vec2 targetPos, double distFromTarget);
	bool on(eBEHAVIOR_TYPE behaviorType){ return (m_nFlags & behaviorType) == behaviorType; }
	bool accumulateForce(Vec2 &steeringForce, Vec2 forceToAdd);
	Vec2 sumForces();

private:
	CBullet* m_pOwner;
	int m_nFlags;
};

