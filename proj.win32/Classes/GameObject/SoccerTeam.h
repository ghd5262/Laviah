#pragma once

#include <vector>
#include "GameObject.h"
#include "../AI/StateMachine.h"

class CSoccerPitch;
class CPlayerBase;
struct sGOALPOST;

typedef enum eTEAM{
	eTEAM_blue,
	eTEAM_red,
};

class CSoccerTeam : public CGameObject
{
public:
	CSoccerTeam(sGOALPOST* homeGoal,
		sGOALPOST* otherGoal,
		CSoccerPitch* pitch,
		eTEAM teamColor)
		: CGameObject(CGameObject::getM_NextValidID())
		, m_pHomeGoalPost(homeGoal)
		, m_pOtherGoalPost(otherGoal)
		, m_pSoccerPitch(pitch)
		, m_teamColor(teamColor){};

	virtual ~CSoccerTeam();

public:
	//* 생성함수 */
	static CSoccerTeam* create(sGOALPOST* homeGoal, sGOALPOST* otherGoal, CSoccerPitch* pitch, eTEAM teamColor);


protected:
	virtual bool init();

	///getter & setter


private:
	bool initVariable();
	void initPlayer();
	void calculateClosestPlayerToBall();

private:
	std::vector<CPlayerBase*> m_vecPlayers;

	//CStateMachine<CSoccerTeam>* m_pStateMachine;

	CSoccerPitch* m_pSoccerPitch;

	eTEAM m_teamColor;

	sGOALPOST* m_pHomeGoalPost;
	sGOALPOST* m_pOtherGoalPost;

	CPlayerBase* m_pReceivingPlayer;
	CPlayerBase* m_pPlayerClosestToBall;
	CPlayerBase* m_pControllingPlayer;
	CPlayerBase* m_pSupportingPlayer;

	double m_dDistSqToBallOfClosestPlayer;
};

inline CSoccerTeam* CSoccerTeam::create(sGOALPOST* homeGoal, sGOALPOST* otherGoal, CSoccerPitch* pitch, eTEAM teamColor)
{
	CSoccerTeam *pRet = new(std::nothrow) CSoccerTeam(homeGoal, otherGoal, pitch, teamColor);
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