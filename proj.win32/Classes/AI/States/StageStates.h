#pragma once
#include "State.h"
#include "../../GameObject/ObjectManager.h"

class CObjectManager;
//------------------------------------------------------------------------
class CNormalStageState : public CState<CObjectManager>
{
public:

	//this is a singleton
	static CNormalStageState* Instance();

	void Enter(CObjectManager* objectMng) override;

	void Execute(CObjectManager* objectMng, float delta) override;

	void Exit(CObjectManager* objectMng) override;

private:
	CNormalStageState(){}
	virtual ~CNormalStageState(){}
};

//------------------------------------------------------------------------
class CBonusTimeStageState : public CState<CObjectManager>
{
public:

	//this is a singleton
	static CBonusTimeStageState* Instance();

	void Enter(CObjectManager* objectMng) override;

	void Execute(CObjectManager* objectMng, float delta) override;

	void Exit(CObjectManager* objectMng) override;

private:
	CBonusTimeStageState(){}
	virtual ~CBonusTimeStageState(){}
};

//------------------------------------------------------------------------
class CCrazyStageState : public CState<CObjectManager>
{
public:

	//this is a singleton
	static CCrazyStageState* Instance();

	void Enter(CObjectManager* objectMng) override;

	void Execute(CObjectManager* objectMng, float delta) override;

	void Exit(CObjectManager* objectMng) override;

private:
	CCrazyStageState(){}
	virtual ~CCrazyStageState(){}
};

//------------------------------------------------------------------------
class CGameCountDownState : public CState<CObjectManager>
{
public:

	//this is a singleton
	static CGameCountDownState* Instance();

	void Enter(CObjectManager* objectMng) override;

	void Execute(CObjectManager* objectMng, float delta) override;

	void Exit(CObjectManager* objectMng) override;

private:
	CGameCountDownState() : m_fTime(0.f){}
	virtual ~CGameCountDownState(){}

private:
	float m_fTime;
};