#include "SpaceShipStates.h"
#include "../../GameObject/SpaceShip.h"
#include "../../GameObject/Bullet/Bullet.h"
using namespace cocos2d;

CFlyAround* CFlyAround::Instance()
{
	static CFlyAround instance;
	return &instance;
}

void CFlyAround::Enter(CSpaceShip* spaceship)
{
	if (spaceship->getTargetPos().x < Director::getInstance()->getVisibleSize().width / 2)
		spaceship->setDirection(1);
	else
		spaceship->setDirection(-1);
}

void CFlyAround::Execute(CSpaceShip* spaceship, float delta)
{
    spaceship->FlyAround(delta);
}

void CFlyAround::Exit(CSpaceShip* spaceship)
{}

CFlyAway* CFlyAway::Instance()
{
    static CFlyAway instance;
    return &instance;
}

void CFlyAway::Enter(CSpaceShip* spaceship)
{
	spaceship->setAwayAngle(random<int>(0, 360));
}

void CFlyAway::Execute(CSpaceShip* spaceship, float delta)
{
	spaceship->FlyAway(delta);
}

void CFlyAway::Exit(CSpaceShip* spaceship)
{}

CFlyToTouchArea* CFlyToTouchArea::Instance()
{
	static CFlyToTouchArea instance;
	return &instance;
}

void CFlyToTouchArea::Enter(CSpaceShip* spaceship)
{}

void CFlyToTouchArea::Execute(CSpaceShip* spaceship, float delta)
{
	spaceship->FlyToTouchArea(delta);
	spaceship->Collision();
	if (spaceship->getArrive())
		spaceship->ChangeState(CFlyAround::Instance());
}

void CFlyToTouchArea::Exit(CSpaceShip* spaceship)
{}
