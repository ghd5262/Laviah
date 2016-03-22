#pragma once
#include "State.h"
#include "../../GameObject/ObjectManager.h"

class CStateManager;
//------------------------------------------------------------------------
class CNormalState : public CState<CObjectManager>
{
public:

	//this is a singleton
	static CNormalState* Instance();

	void Enter(CObjectManager* objectMng) override;

	void Execute(CObjectManager* objectMng, float delta) override;

	void Exit(CObjectManager* objectMng) override;

private:
	CNormalState(){}
	virtual ~CNormalState(){}
};

//------------------------------------------------------------------------
class CBonusTimeState : public CState<CObjectManager>
{
public:

	//this is a singleton
	static CBonusTimeState* Instance();

	void Enter(CObjectManager* objectMng) override;

	void Execute(CObjectManager* objectMng, float delta) override;

	void Exit(CObjectManager* objectMng) override;

private:
	CBonusTimeState(){}
	virtual ~CBonusTimeState(){}
};

//------------------------------------------------------------------------
class CCrazyState : public CState<CObjectManager>
{
public:

	//this is a singleton
	static CCrazyState* Instance();

	void Enter(CObjectManager* objectMng) override;

	void Execute(CObjectManager* objectMng, float delta) override;

	void Exit(CObjectManager* objectMng) override;

private:
	CCrazyState(){}
	virtual ~CCrazyState(){}
};


//------------------------------------------------------------------------
class CGiantItemState : public CState<CObjectManager>
{
public:

	//this is a singleton
	static CGiantItemState* Instance();

	void Enter(CObjectManager* objectMng) override;

	void Execute(CObjectManager* objectMng, float delta) override;

	void Exit(CObjectManager* objectMng) override;

private:
	CGiantItemState(){}
	virtual ~CGiantItemState(){}
};


//------------------------------------------------------------------------
class CMagnetItemState : public CState<CObjectManager>
{
public:

	//this is a singleton
	static CMagnetItemState* Instance();

	void Enter(CObjectManager* objectMng) override;

	void Execute(CObjectManager* objectMng, float delta) override;

	void Exit(CObjectManager* objectMng) override;

private:
	CMagnetItemState(){}
	virtual ~CMagnetItemState(){}
};


//------------------------------------------------------------------------
class CStarItemState : public CState<CObjectManager>
{
public:

	//this is a singleton
	static CStarItemState* Instance();

	void Enter(CObjectManager* objectMng) override;

	void Execute(CObjectManager* objectMng, float delta) override;

	void Exit(CObjectManager* objectMng) override;

private:
	CStarItemState(){}
	virtual ~CStarItemState(){}
};


//------------------------------------------------------------------------
class CCoinItemState : public CState<CObjectManager>
{
public:

	//this is a singleton
	static CCoinItemState* Instance();

	void Enter(CObjectManager* objectMng) override;

	void Execute(CObjectManager* objectMng, float delta) override;

	void Exit(CObjectManager* objectMng) override;

private:
	CCoinItemState(){}
	virtual ~CCoinItemState(){}
};


//------------------------------------------------------------------------
class CShieldItemState : public CState<CObjectManager>
{
public:

	//this is a singleton
	static CShieldItemState* Instance();

	void Enter(CObjectManager* objectMng) override;

	void Execute(CObjectManager* objectMng, float delta) override;

	void Exit(CObjectManager* objectMng) override;

private:
	CShieldItemState(){}
	virtual ~CShieldItemState(){}
};