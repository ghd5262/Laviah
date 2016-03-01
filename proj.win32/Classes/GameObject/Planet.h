#pragma once
#include "Mover.h"

class CPlanet : public CMover {
public:
	CPlanet(std::string textureName, float boundingRadius, float rotate, float rotateSpeed);
	virtual ~CPlanet();

	virtual void Execute(float delta = 0.f) override;

protected:
	//getter & setter
	CC_SYNTHESIZE(float, m_fRotate, Rotate);
	CC_SYNTHESIZE(float, m_fRotateSpeed, RotateSpeed);
};
