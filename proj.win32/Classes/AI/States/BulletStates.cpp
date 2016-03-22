#include "BulletStates.h"
#include "../../GameObject/Bullet.h"
#include "../../GameObject/Planet.h"
#include "../../GameObject/Player.h"
#include "../../GameObject/Bullet/PlayItem.h"
#include "../../GameObject/SteeringBehaviors.h"
#include "../../GameObject/ObjectManager.h"
CBulletNormal* CBulletNormal::Instance()
{
	static CBulletNormal instance;

	return &instance;
}

void CBulletNormal::Enter(CBullet* bullet)
{
	bullet->getSteeringBehavior()->setTargetVec(bullet->getPlanet()->getPosition());
	bullet->getSteeringBehavior()->SeekOn();
}

void CBulletNormal::Execute(CBullet* bullet, float delta)
{
	Vec2 SteeringForce = bullet->getSteeringBehavior()->CalculateBehaviorVector();
	SteeringForce.normalize();
	bullet->setVelocityVec(SteeringForce * (bullet->getBulletSpeed() * delta));

	bullet->setPosition(bullet->getPosition() + bullet->getVelocityVec());

	if (bullet->IsHit(bullet->getPlanet()))
	{
		bullet->CollisionWithPlanet();
	}
	else if (bullet->IsHit(bullet->getPlayer()))
	{
		bullet->CollisionWithPlayer();
	}

	switch (bullet->getItemEffect() & CObjectManager::Instance()->getCurrentItem())
	{
	case eITEM_TYPE_shield:
	{
		
	}break;
	case eITEM_TYPE_magnet:
	{
		bullet->getFSM()->ChangeState(CBulletMagnetItem::Instance());
	}break;
	case eITEM_TYPE_coin:
	{

	}break;
	case eITEM_TYPE_star:
	{

	}break;
	case eITEM_TYPE_giant:
	{

	}break;

	default:
	{

	}
	}
}

void CBulletNormal::Exit(CBullet* bullet)
{
	bullet->getSteeringBehavior()->SeekOff();
}


//------------------------------------------------------------------------

CBulletBonusTime* CBulletBonusTime::Instance()
{
	static CBulletBonusTime instance;

	return &instance;
}

void CBulletBonusTime::Enter(CBullet* bullet)
{

}

void CBulletBonusTime::Execute(CBullet* bullet, float delta)
{

}

void CBulletBonusTime::Exit(CBullet* bullet)
{

}


//------------------------------------------------------------------------

CBulletMagnetItem* CBulletMagnetItem::Instance()
{
	static CBulletMagnetItem instance;

	return &instance;
}

void CBulletMagnetItem::Enter(CBullet* bullet)
{
	bullet->getSteeringBehavior()->setTargetVec(bullet->getPlanet()->getPosition());
	bullet->getSteeringBehavior()->SeekOn();
}

void CBulletMagnetItem::Execute(CBullet* bullet, float delta)
{
	CCLOG("Magnet Item %f", delta);

	Vec2 SteeringForce = bullet->getSteeringBehavior()->CalculateBehaviorVector();
	SteeringForce.normalize();
	bullet->setVelocityVec(SteeringForce * (bullet->getBulletSpeed() * delta));

	bullet->setPosition(bullet->getPosition() + bullet->getVelocityVec());
	if (bullet->IsHit(bullet->getPlayer()))
	{
		bullet->CollisionWithPlayer();
	}
	else if (bullet->IsHit(bullet->getPlayer()->getPosition(), bullet->getPlayer()->getMagnetLimitRadius()))
	{
		bullet->getSteeringBehavior()->setTargetVec(bullet->getPlayer()->getPosition());
		bullet->setBulletSpeed(bullet->getBulletSpeed() + 50);
	}
	else if (bullet->IsHit(bullet->getPlanet()))
	{
		bullet->CollisionWithPlanet();
	}
	if (!(CObjectManager::Instance()->getCurrentItem() & eITEM_TYPE_magnet))
	{
		bullet->getFSM()->ChangeState(CBulletNormal::Instance());
	}
}

void CBulletMagnetItem::Exit(CBullet* bullet)
{
	bullet->getSteeringBehavior()->SeekOff();
}