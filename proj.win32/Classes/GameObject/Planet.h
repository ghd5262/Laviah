#pragma once
#include "Mover.h"

class CPlanet : public CMover {
public:
	static CPlanet* create(std::string textureName, float boundingRadius, float rotate, float rotateSpeed);
	virtual void Execute(float delta = 0.f) override;

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

	//getter & setter
	CC_SYNTHESIZE(float, m_fRotate, Rotate);
	CC_SYNTHESIZE(float, m_fRotateSpeed, RotateSpeed);

private:
	CPlanet(std::string textureName, float boundingRadius, float rotate, float rotateSpeed);
	virtual ~CPlanet(){}

private:
	std::string m_TextureName;
	Sprite* m_pTexture;
};
