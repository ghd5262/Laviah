#pragma once
#include "State.h"

class CSpaceShip;
//------------------------------------------------------------------------
class CFlyAround : public CState<CSpaceShip>
{
public:

	//this is a singleton
	static CFlyAround* Instance();

	void Enter(CSpaceShip* spaceship) override;

	void Execute(CSpaceShip* spaceship, float delta) override;

	void Exit(CSpaceShip* spaceship) override;

private:
	CFlyAround(){}
	virtual ~CFlyAround(){}
};

class CFlyAway : public CState<CSpaceShip>
{
public:
    
    //this is a singleton
    static CFlyAway* Instance();
    
    void Enter(CSpaceShip* spaceship) override;
    
    void Execute(CSpaceShip* spaceship, float delta) override;
    
    void Exit(CSpaceShip* spaceship) override;
    
private:
    CFlyAway(){}
    virtual ~CFlyAway(){}
};