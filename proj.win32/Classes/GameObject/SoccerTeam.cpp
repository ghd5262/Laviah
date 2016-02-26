#include "SoccerTeam.h"

bool CSoccerTeam::init()
{
	if (!initVariable())
		return false;

	initPlayer();

	return true;
}

CSoccerTeam::~CSoccerTeam()
{
	//CC_SAFE_DELETE(m_pStateMachine);
}

bool CSoccerTeam::initVariable()
{
	try
	{
		//m_pStateMachine = new CStateMachine<CSoccerTeam>(this);
		//if (m_pStateMachine != nullptr)
		{
			/*m_pStateMachine->SetCurrentState(CDefending::Instance());
			m_pStateMachine->SetPreviousState(CDefending::Instance());
			m_pStateMachine->SetGlobalState(NULL);*/
		}

	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CSoccerTeam::initPlayer()
{

}

void CSoccerTeam::calculateClosestPlayerToBall()
{

}