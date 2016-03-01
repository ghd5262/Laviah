#pragma once
#include "GameObject.h"
#include "../Task/Task.h"

class CMover : public CGameObject, public CTask
{
public:
	CMover(const CTaskList* list, std::string textureName, float boundingRadius);
	CMover(std::string textureName, float boundingRadius);
	virtual ~CMover() = 0;

	bool IsHit(CMover* mover);
	bool IsHit(CTaskList* list);
};

