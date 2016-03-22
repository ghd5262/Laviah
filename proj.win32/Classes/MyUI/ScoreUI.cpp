#include "ScoreUI.h"

CScoreUI* CScoreUI::create()
{
	CScoreUI *pRet = new(std::nothrow) CScoreUI();
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

bool CScoreUI::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CScoreUI::initVariable()
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

void CScoreUI::Execute(float delta)
{
}