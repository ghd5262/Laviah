#include "Planet.h"


CPlanet::CPlanet(std::string textureName, float boundingRadius, float rotate, float rotateSpeed)
	: CMover(textureName, boundingRadius)
	, m_fRotate(rotate)
	, m_fRotateSpeed(rotateSpeed)
{
}

CPlanet::~CPlanet()
{
}

void CPlanet::Execute(float delta)
{
	// 青己雀傈
	//CCLOG("青己雀傈\n");
}