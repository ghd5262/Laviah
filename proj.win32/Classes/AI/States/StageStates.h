#pragma once
#include "State.h"
#include "../../GameObject/Stage/StageManager.h"

class CStageManager;
//------------------------------------------------------------------------
class CNormalStageState : public CState<CStageManager>
{
public:

	//this is a singleton
	static CNormalStageState* Instance();

	void Enter(CStageManager* stageMng) override;

	void Execute(CStageManager* stageMng, float delta) override;

	void Exit(CStageManager* stageMng) override;

private:
	CNormalStageState(){}
	virtual ~CNormalStageState(){}
};

//------------------------------------------------------------------------
class CBonusTimeStageState : public CState<CStageManager>
{
public:

	//this is a singleton
	static CBonusTimeStageState* Instance();

	void Enter(CStageManager* stageMng) override;

	void Execute(CStageManager* stageMng, float delta) override;

	void Exit(CStageManager* stageMng) override;

private:
	CBonusTimeStageState(){}
	virtual ~CBonusTimeStageState(){}
};

//------------------------------------------------------------------------
class CCrazyStageState : public CState<CStageManager>
{
public:

	//this is a singleton
	static CCrazyStageState* Instance();

	void Enter(CStageManager* stageMng) override;

	void Execute(CStageManager* stageMng, float delta) override;

	void Exit(CStageManager* stageMng) override;

private:
	CCrazyStageState(){}
	virtual ~CCrazyStageState(){}
};