#pragma once
#include "Mover.h"

class CPlayer : public CMover {
public:
	CPlayer(std::string textureName, float boundingRadius, float runSpeed, float scale);
	virtual ~CPlayer();

	virtual void Execute(float delta = 0.f) override;

protected:
	//getter & setter
	CC_SYNTHESIZE(float, m_fRunSpeed, RunSpeed);
	CC_SYNTHESIZE(float, m_fScale, Scale);
};
