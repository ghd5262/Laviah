#include "Planet.h"

CPlanet* CPlanet::create(
	std::string textureName,
	float boundingRadius,
	float rotate,
	float rotateSpeed)
{
	CPlanet *pRet = new(std::nothrow) CPlanet(
		 textureName
		, boundingRadius
		, rotate
		, rotateSpeed);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

CPlanet::CPlanet(std::string textureName, float boundingRadius, float rotate, float rotateSpeed)
	: CGameObject(boundingRadius)
	, m_TextureName(textureName)
	, m_fRotate(rotate)
	, m_fRotateSpeed(rotateSpeed)
{
}

bool CPlanet::init()
{
	this->DrawDebugBinding();
	if (!initVariable())
		return false;
	return true;
}

bool CPlanet::initVariable()
{
	try{
		m_pTexture = Sprite::create(m_TextureName);
		if (m_pTexture != nullptr){
			m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
			addChild(m_pTexture);
		}
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CPlanet::Execute(float delta)
{
	// 青己雀傈
	//CCLOG("青己雀傈\n");
}