#include "AlienStates.h"
#include "../../GameObject/Alien.h"

CWanderingState* CWanderingState::Instance()
{
    static CWanderingState instance;
    
    return &instance;
}

void CWanderingState::Enter(CAlien* alien)
{
    alien->setDirection(random<bool>(0, 1) == 0 ? -1 : 1);
}

void CWanderingState::Execute(CAlien* alien, float delta)
{
    alien->Walk(delta);
}

void CWanderingState::Exit(CAlien* alien)
{
}

