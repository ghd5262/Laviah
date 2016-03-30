#pragma once
#include "../Shooter/ShooterHeaders.h"
#include "../../AI/StateMachine.h"
#include <vector>

class CStageManager
{
public:
	static CStageManager* Instance();
	void Execute(float delta = 0.f);
	/*	bool AddShooter(std::string patternName, CShooter* shooter);
	CShooter* FindShooterWithName(std::string patternName) const;*/

	//getter & setter
	CStateMachine<CStageManager>* getFSM(){ return m_FSM; }
	sPATTERN_SHOOTER_PARAM getPatternInfo(int patternNum) const {
		return m_PatternList[patternNum];
	}

private:
	CStageManager();
	virtual ~CStageManager();

private:
	CStateMachine<CStageManager>* m_FSM;
	std::vector<sSHOOTER_PARAM> m_StageParamList;
	sPATTERN_SHOOTER_PARAM m_PatternList[10];
	float m_fTime;
};

