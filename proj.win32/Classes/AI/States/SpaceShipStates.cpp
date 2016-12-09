#include "SpaceShipStates.h"
#include "../../GameObject/SpaceShip.h"

using namespace cocos2d;

CFlyAround* CFlyAround::Instance()
{
	static CFlyAround instance;

	return &instance;
}

void CFlyAround::Enter(CSpaceShip* spaceship)
{
	spaceship->setDirection(random<int>(0, 1) == 0 ? -1 : 1);
//	spaceship->setCurrentAction(random<int>(0, 1));
}

void CFlyAround::Execute(CSpaceShip* spaceship, float delta)
{
    spaceship->FlyAround(delta);
}

void CFlyAround::Exit(CSpaceShip* spaceship)
{
}

CFlyAway* CFlyAway::Instance()
{
    static CFlyAway instance;
    
    return &instance;
}

void CFlyAway::Enter(CSpaceShip* spaceship)
{
    spaceship->setTargetPos(Vec2(50, 50));
}

void CFlyAway::Execute(CSpaceShip* spaceship, float delta)
{
}

void CFlyAway::Exit(CSpaceShip* spaceship)
{
}
