#pragma once
#include "State.h"

class CPlayer;
//------------------------------------------------------------------------
class CPlayerNormal : public CState<CPlayer>
{
public:

	static CPlayerNormal* Instance();

	void Enter(CPlayer* player) override;

	void Execute(CPlayer* player, float delta) override;

	void Exit(CPlayer* player) override;

private:
	CPlayerNormal(){}
	virtual ~CPlayerNormal(){}
};

//------------------------------------------------------------------------
class CPlayerGiant : public CState<CPlayer>
{
public:

	static CPlayerGiant* Instance();

	void Enter(CPlayer* player) override;

	void Execute(CPlayer* player, float delta) override;

	void Exit(CPlayer* player) override;

private:
	CPlayerGiant(){}
	virtual ~CPlayerGiant(){}
};