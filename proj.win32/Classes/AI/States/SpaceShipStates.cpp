#include "SpaceShipStates.h"
#include "../../GameObject/SpaceShip.h"

CSpaceShipFlyingState* CSpaceShipFlyingState::Instance()
{
	static CSpaceShipFlyingState instance;

	return &instance;
}

void CSpaceShipFlyingState::Enter(CSpaceShip* spaceship)
{
//	spaceship->setDirection(random<int>(0, 1) == 0 ? -1 : 1);
//	spaceship->setCurrentAction(random<int>(0, 1));
}

void CSpaceShipFlyingState::Execute(CSpaceShip* spaceship, float delta)
{
//	spaceship->setActionTime(spaceship->getActionTime() + delta);
//	if (spaceship->getActionTime() > 2.f)
//	{
//		spaceship->setActionTime(0.f);
//		if (random<int>(0, 100) > 70 ? true : false)
//			spaceship->getFSM()->ChangeState(CSpaceShipFlyingState::Instance());
//	}
//
//	switch (spaceship->getCurrentAction()) {
//	case 0:
		spaceship->FlyAround(delta);
//		break;
//
//	case 1:
//		spaceship->Break();
//		break;
//	}
}

void CSpaceShipFlyingState::Exit(CSpaceShip* spaceship)
{
}

