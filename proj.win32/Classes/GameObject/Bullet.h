#pragma once

#include "Mover.h"
#include "../Common/HSHUtility.h"


class CPlanet;
class CPlayer;
class CBullet : public CMover {
public:
	CBullet(std::string textureName, float boundingRadius, float angle, float speed, Vec2 targetVec);
	virtual ~CBullet();

	void* operator new (size_t n);
	void operator delete (void* p);
	virtual void Execute(float delta = 0.f) override;

protected:
	//getter & setter
	CC_SYNTHESIZE(float, m_fAngle, Angle);
	CC_SYNTHESIZE(float, m_fBulletSpeed, BulletSpeed);
	CC_SYNTHESIZE(Vec2, m_bulletTargetVec, BulletTargetVec);
private:
	const CPlanet* m_pPlanet;
	const CPlayer* m_pPlayer;
};

