#include "Player.h"

CPlayer* CPlayer::create(
	std::string textureName,
	float boundingRadius,
	float rotate,
	float rotateSpeed)
{
	CPlayer *pRet = new(std::nothrow) CPlayer(
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

CPlayer::CPlayer(std::string textureName, float boundingRadius, float rotate, float rotateSpeed)
	: CGameObject(boundingRadius)
	, m_TextureName(textureName)
	, m_fRotate(rotate)
	, m_fRotateSpeed(rotateSpeed)
{
}

bool CPlayer::init()
{
	//this->DrawDebugBinding();
	if (!initVariable())
		return false;
	return true;
}

bool CPlayer::initVariable()
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

void CPlayer::Execute(float delta)
{
	// 행성회전
	//CCLOG("행성회전\n");
}