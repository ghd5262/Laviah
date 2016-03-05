#include "Mover.h"


CMover::CMover(float boundingRadius)
	: CGameObject(boundingRadius)
{
}

CMover::~CMover()
{
}

bool CMover::IsHit(CMover* mover) {
	float
		dx = mover->getPosition().x - getPosition().x,
		dy = mover->getPosition().y - getPosition().y,
		hit = mover->m_fBoundingRadius + m_fBoundingRadius;
	return (dx * dx < hit * hit) && (dy * dy < hit * hit);
}