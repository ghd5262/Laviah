#pragma once
#include "State.h"

class CRocket;
//------------------------------------------------------------------------

class CFlyToTarget : public CState<CRocket>
{
public:

	//this is a singleton
	static CFlyToTarget* Instance();

	void Enter(CRocket* rocket) override;

	void Execute(CRocket* rocket, float delta) override;

	void Exit(CRocket* rocket) override;

private:
	CFlyToTarget(){}
	virtual ~CFlyToTarget(){}
};

class CFlyAround : public CState<CRocket>
{
public:

	//this is a singleton
	static CFlyAround* Instance();

	void Enter(CRocket* rocket) override;

	void Execute(CRocket* rocket, float delta) override;

	void Exit(CRocket* rocket) override;

private:
	CFlyAround(){}
	virtual ~CFlyAround(){}
};

class CFlyAway : public CState<CRocket>
{
public:
    
    //this is a singleton
    static CFlyAway* Instance();
    
    void Enter(CRocket* rocket) override;
    
    void Execute(CRocket* rocket, float delta) override;
    
    void Exit(CRocket* rocket) override;
    
private:
    CFlyAway(){}
    virtual ~CFlyAway(){}
};

class CFlyToTouchArea : public CState<CRocket>
{
public:

	//this is a singleton
	static CFlyToTouchArea* Instance();

	void Enter(CRocket* rocket) override;

	void Execute(CRocket* rocket, float delta) override;

	void Exit(CRocket* rocket) override;

private:
	CFlyToTouchArea(){}
	virtual ~CFlyToTouchArea(){}
};