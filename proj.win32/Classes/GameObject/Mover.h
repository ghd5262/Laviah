#pragma once
#include "GameObject.h"

class CMover : public CGameObject
{
public:
	CMover(){}
	CMover(float boundingRadius);
	virtual ~CMover() = 0;

	bool IsHit(CMover* mover);
};

