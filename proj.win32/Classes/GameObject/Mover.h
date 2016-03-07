#pragma once
#include "GameObject.h"

class CMover : public CGameObject
{
public:
	CMover(float boundingRadius);
	virtual ~CMover() = 0;

	bool IsHit(CMover* mover);
	void ReturnToMemoryBlock();				// �޸� ������ �ǵ����� �Լ� �̴�. 
											// Alive = false, parent�κ��� ����, visible = false�� �����Ѵ�.  
};

