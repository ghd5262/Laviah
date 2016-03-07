#pragma once
#include "GameObject.h"

class CMover : public CGameObject
{
public:
	CMover(float boundingRadius);
	virtual ~CMover() = 0;

	bool IsHit(CMover* mover);
	void ReturnToMemoryBlock();				// 메모리 블럭으로 되돌리는 함수 이다. 
											// Alive = false, parent로부터 제거, visible = false를 수행한다.  
};

