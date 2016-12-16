#include "RocketStates.h"
#include "../../GameObject/Rocket.h"
#include "../../GameObject/Bullet/Bullet.h"
using namespace cocos2d;

CFlyAround* CFlyAround::Instance()
{
	static CFlyAround instance;
	return &instance;
}

void CFlyAround::Enter(CRocket* rocket)
{
	if (rocket->getTargetPos().x < Director::getInstance()->getVisibleSize().width / 2)
		rocket->setDirection(1);
	else
		rocket->setDirection(-1);
}

void CFlyAround::Execute(CRocket* rocket, float delta)
{
    rocket->FlyAround(delta);
}

void CFlyAround::Exit(CRocket* rocket){}

CFlyAway* CFlyAway::Instance()
{
    static CFlyAway instance;
    return &instance;
}

void CFlyAway::Enter(CRocket* rocket)
{
	rocket->setAwayAngle(random<int>(0, 360));
}

void CFlyAway::Execute(CRocket* rocket, float delta)
{
	rocket->FlyAway(delta);
}

void CFlyAway::Exit(CRocket* rocket){}

CFlyToTouchArea* CFlyToTouchArea::Instance()
{
	static CFlyToTouchArea instance;
	return &instance;
}

void CFlyToTouchArea::Enter(CRocket* rocket){}

void CFlyToTouchArea::Execute(CRocket* rocket, float delta)
{
	rocket->FlyToTouchArea(delta);
	rocket->CollisionCheckAtHome();
	if (rocket->getArrive())
		rocket->ChangeState(CFlyAround::Instance());
}

void CFlyToTouchArea::Exit(CRocket* rocket){}

