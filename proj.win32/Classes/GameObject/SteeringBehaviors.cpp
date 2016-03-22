#include "SteeringBehaviors.h"
#include "Bullet.h"
#include <WinDef.h>

CSteeringBehaviors::CSteeringBehaviors(CBullet* bullet)
	: m_pOwner(bullet)
	, m_nFlags(0)
	, m_dInterposeDist(0.0f)
{
}

Vec2 CSteeringBehaviors::CalculateBehaviorVector()
{
	// 현재의 조종힘을 기반으로 계산하여 벡터값을 반환해줌
	m_steeringVec = Vec2(0, 0);
	m_steeringVec = sumForces();
	if (m_steeringVec.length() > m_pOwner->getMaxForce())
		m_steeringVec.normalize();
	return m_steeringVec;
}

Vec2 CSteeringBehaviors::seek(Vec2 target)
{
	// 조종행동 찾기
	Vec2 velocity = target - m_pOwner->getPosition();
	Vec2 desiredVelocity = velocity.getNormalized()
		* m_pOwner->getBulletSpeed();

	return (desiredVelocity - m_pOwner->getVelocityVec());
}

Vec2 CSteeringBehaviors::arrive(Vec2 target, eDECELERATION deceleration)
{
	// 조종행동 도착하기
	Vec2 toTarget = target - m_pOwner->getPosition();

	double dist = toTarget.length();

	if (dist > 0)
	{
		//because Deceleration is enumerated as an int, this value is required
		//to provide fine tweaking of the deceleration..
		const double DecelerationTweaker = 0.3;

		//calculate the speed required to reach the target given the desired
		//deceleration
		double speed = dist / (static_cast<double>(deceleration)* DecelerationTweaker);

		//make sure the velocity does not exceed the max
		speed = speed < m_pOwner->getBulletSpeed() ? speed : m_pOwner->getBulletSpeed();

		//from here proceed just like Seek except we don't need to normalize 
		//the ToTarget vector because we have already gone to the trouble
		//of calculating its length: dist. 
		Vec2 DesiredVelocity = toTarget * speed / dist;

		return (DesiredVelocity - m_pOwner->getVelocityVec());
	}

	return Vec2(0, 0);
}

Vec2 CSteeringBehaviors::interpose(Vec2 targetPos, double distFromTarget)
{
	Vec2 opponentVelocity = targetPos - m_pOwner->getPosition();

	return arrive(targetPos + opponentVelocity.getNormalized() * distFromTarget, eDECELERATION_normal);
}

bool CSteeringBehaviors::accumulateForce(Vec2 &steeringForce, Vec2 forceToAdd)
{
	// 조종힘 계산 함수
	double magnitudeSoFar = steeringForce.length();
	double magnitudeRemaining = m_pOwner->getMaxForce() - magnitudeSoFar; // 남은힘

	if (magnitudeRemaining <= 0.0f) return false;

	double magnitudeToAdd = forceToAdd.length();

	if (magnitudeToAdd > magnitudeRemaining)
		magnitudeToAdd = magnitudeRemaining;

	steeringForce += (forceToAdd.getNormalized() * magnitudeToAdd);

	return true;
}

Vec2 CSteeringBehaviors::sumForces()
{
	Vec2 force;

	if (on(eBEHAVIOR_TYPE_seek))
	{
		force += seek(m_targetVec);
		if (!accumulateForce(m_steeringVec, force)) return m_steeringVec;
	}

	if (on(eBEHAVIOR_TYPE_arrive))
	{
		force += arrive(m_targetVec, eDECELERATION::eDECELERATION_fast);
		if (!accumulateForce(m_steeringVec, force)) return m_steeringVec;
	}

	if (on(eBEHAVIOR_TYPE_interpose))
	{
		force += interpose(m_targetVec, m_dInterposeDist);
		if (!accumulateForce(m_steeringVec, force)) return m_steeringVec;
	}

	return m_steeringVec;
}