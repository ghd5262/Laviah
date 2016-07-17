#pragma once
#include "State.h"

class CSpaceShip;
//------------------------------------------------------------------------
class CSpaceShipFlyingState : public CState<CSpaceShip>
{
public:

	//this is a singleton
	static CSpaceShipFlyingState* Instance();

	void Enter(CSpaceShip* spaceship) override;

	void Execute(CSpaceShip* spaceship, float delta) override;

	void Exit(CSpaceShip* spaceship) override;

private:
	CSpaceShipFlyingState(){}
	virtual ~CSpaceShipFlyingState(){}
};