#include "RocketStates.h"
#include "../../GameObject/Rocket.h"
#include "../../GameObject/Bullet/Bullet.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/Player.h"
#include "../../GameObject/ItemManager.h"

using namespace cocos2d;

CFlyAtBonusTime* CFlyAtBonusTime::Instance()
{
	static CFlyAtBonusTime instance;
	return &instance;
}

void CFlyAtBonusTime::Enter(CRocket* rocket)
{
	auto visible = Director::getInstance()->getVisibleSize();
	rocket->setPosition(Vec2(-50, -50));
	rocket->setRotation(90);
	auto pos = CObjectManager::Instance()->getPlayer()->getOriginPos();
	rocket->setTargetPos(Vec2(pos.x, pos.y + 10));
	rocket->setVelocity(Vec2(800, 300));
	rocket->setSpeed(600.f);
	rocket->setArriveCallback([=](cocos2d::Node* sender){

		rocket->scheduleOnce([=](float delta){
			rocket->setTargetPos(Vec2(visible.width * 0.5f, visible.height + 500));
			rocket->setArrive(false);
			rocket->setVelocity(Vec2(0, 1500));
		}, 1.f, "DELAY");
	});
}

void CFlyAtBonusTime::Execute(CRocket* rocket, float delta)
{
	if (!rocket->getArrive())
		rocket->FlyToTarget(delta);
}

void CFlyAtBonusTime::Exit(CRocket* rocket){}


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

	if ((CItemManager::Instance()->isCurrentItem(eITEM_FLAG_bonustime)))
		rocket->ChangeState(CFlyAtBonusTime::Instance());
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

