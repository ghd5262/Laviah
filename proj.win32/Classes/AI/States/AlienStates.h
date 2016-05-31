#pragma once
#include "State.h"

class CAlien;
//------------------------------------------------------------------------
class CWanderingState : public CState<CAlien>
{
public:
    
    //this is a singleton
    static CWanderingState* Instance();
    
    void Enter(CAlien* alien) override;
    
    void Execute(CAlien* alien, float delta) override;
    
    void Exit(CAlien* alien) override;
    
private:
    CWanderingState(){}
    virtual ~CWanderingState(){}
};