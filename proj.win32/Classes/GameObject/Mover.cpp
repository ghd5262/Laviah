#include "Mover.h"


CMover::CMover(const CTaskList* list, std::string textureName, float boundingRadius)
	: CGameObject(textureName, boundingRadius)
	, CTask(list)
{
}

CMover::CMover(std::string textureName, float boundingRadius)
	: CGameObject(textureName, boundingRadius)
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

bool CMover::IsHit(CTaskList* list) {
	for (CTaskIter i(list); i.HasNext();) {
		CMover* mover = static_cast<CMover*>(i.Next());
		if (IsHit(mover)) return true;
	}
	return false;
}
