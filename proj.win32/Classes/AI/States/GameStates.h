#pragma once
#include "State.h"
#include "../../GameObject/ObjectManager.h"

class CObjectManager;

class CNormalState : public CState<CObjectManager>
{
public:

	//this is a singleton
    static CNormalState* Instance(){
        static CNormalState instance;
        return &instance;
    }
    void Enter(CObjectManager* objectMng) override;

	void Execute(CObjectManager* objectMng, float delta) override;

	void Exit(CObjectManager* objectMng) override;

private:
	CNormalState(){}
	virtual ~CNormalState(){}
};


class CBonusTimeState : public CState<CObjectManager>
{
public:
    
    //this is a singleton
    static CBonusTimeState* Instance(){
        static CBonusTimeState instance;
        return &instance;
    }
    
    void Enter(CObjectManager* objectMng) override;
    
    void Execute(CObjectManager* objectMng, float delta) override;
    
    void Exit(CObjectManager* objectMng) override;
    
private:
    CBonusTimeState(){}
    virtual ~CBonusTimeState(){}
};