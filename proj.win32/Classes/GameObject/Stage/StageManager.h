#pragma once
#include "../Shooter/ShooterHeaders.h"
#include "../../AI/StateMachine.h"
#include <vector>
#include <map>

class CStageManager
{
public:
	static CStageManager* Instance();
	void Execute(float delta = 0.f);
	/*	bool AddShooter(std::string patternName, CShooter* shooter);
	CShooter* FindShooterWithName(std::string patternName) const;*/

	//getter & setter
	CStateMachine<CStageManager>* getFSM(){ return m_FSM; }

private:
	CStageManager();
	virtual ~CStageManager();

private:
	CStateMachine<CStageManager>* m_FSM;
	std::vector<sSHOOTER_PARAM> m_StageParamList;
	float m_fTime;
};

