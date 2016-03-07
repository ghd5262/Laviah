#include "Mover.h"
#include "../Task/PoolingManager.h"

CMover::CMover(float boundingRadius)
	: CGameObject(boundingRadius)
{
	this->setVisible(true);
	this->setAlive(true);
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

void CMover::ReturnToMemoryBlock()
{
	this->removeFromParentAndCleanup(false);
	this->setVisible(false);
	this->setAlive(false);
	CPoolingManager::Instance()->ObjectDelete(this);
}