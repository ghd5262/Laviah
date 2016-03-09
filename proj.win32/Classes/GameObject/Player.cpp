#include "Player.h"

CPlayer* CPlayer::create(
	float boundingRadius,
	float runSpeed,
	float scale)
{
	CPlayer *pRet = new(std::nothrow) CPlayer(
		boundingRadius
		, runSpeed
		, scale);
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

CPlayer::CPlayer(float boundingRadius, float runSpeed, float scale)
	: CGameObject(boundingRadius)
	, m_fRunSpeed(runSpeed)
	, m_fScale(scale)
{
}

CPlayer::~CPlayer()
{
}

bool CPlayer::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CPlayer::initVariable()
{
	try{
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
	//플레이어 런! 런!
}