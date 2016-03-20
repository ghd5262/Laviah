#include "Mover.h"
#include "../Task/PoolingManager.h"

CMover::CMover(float boundingRadius)
	: CGameObject(boundingRadius)
{
	this->setVisible(true);
	this->m_bAlive = true;
	this->m_bHasPointer = true; // false를 만들어 주는 부분은 없다. 종료시 Delete의 플래그로 쓰기 때문
}

CMover::~CMover()
{
	
}

void CMover::Delete()
{
	this->removeFromParent();
	delete this; // operator delete호출 실제 메모리는 PoolingManager에서 제거된다.
}

bool CMover::IsHit(CGameObject* object) {
	float
		dx = object->getPosition().x - getPosition().x,
		dy = object->getPosition().y - getPosition().y,
		hit = object->getBRadius() + m_fBoundingRadius;
	return dx * dx + dy * dy < hit * hit;
}