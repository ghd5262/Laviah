#include "Mover.h"
#include "../Task/PoolingManager.h"

CMover::CMover(float boundingRadius)
	: CGameObject(boundingRadius)
{
	this->setVisible(true);
	this->m_bAlive = true;
	this->m_bHasPointer = true; // false�� ����� �ִ� �κ��� ����. ����� Delete�� �÷��׷� ���� ����
}

CMover::~CMover()
{
	
}

void CMover::Delete()
{
	this->removeFromParent();
	delete this; // operator deleteȣ�� ���� �޸𸮴� PoolingManager���� ���ŵȴ�.
}

bool CMover::IsHit(CGameObject* object) {
	float
		dx = object->getPosition().x - getPosition().x,
		dy = object->getPosition().y - getPosition().y,
		hit = object->getBRadius() + m_fBoundingRadius;
	return dx * dx + dy * dy < hit * hit;
}