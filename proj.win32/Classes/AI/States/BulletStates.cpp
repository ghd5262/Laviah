#include "BulletStates.h"
#include "../../GameObject/Planet.h"
#include "../../GameObject/Player.h"
#include "../../GameObject/SteeringBehaviors.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/ItemManager.h"
#include "../../GameObject/Bullet/Bullet.h"

CBulletNormal* CBulletNormal::Instance()
{
	static CBulletNormal instance;

	return &instance;
}

void CBulletNormal::Enter(CBullet* bullet)
{
	bullet->setTargetVec(bullet->getPlanet()->getPosition());
}

void CBulletNormal::Execute(CBullet* bullet, float delta)
{
	/*Vec2 SteeringForce = bullet->getSteeringBehavior()->CalculateBehaviorVector();
	SteeringForce.normalize();
	bullet->setVelocityVec(SteeringForce * (bullet->getBulletSpeed() * delta));

	bullet->setPosition(bullet->getPosition() + bullet->getVelocityVec());*/

	bullet->Seek(delta);

	if (bullet->IsHit(bullet->getPlanet()))
	{
		bullet->CollisionWithPlanet();
	}
	else if (bullet->IsHit(bullet->getPlayer()))
	{
		bullet->CollisionWithPlayer();
	}

	switch (bullet->getItemEffect() & CItemManager::Instance()->getCurrentItem())
	{
	case eITEM_FLAG_shield:
	{
		
	}break;
	case eITEM_FLAG_magnet:
	{
		bullet->getFSM()->ChangeState(CBulletMagnetItem::Instance());
	}break;
	case eITEM_FLAG_coin:
	{

	}break;
	case eITEM_FLAG_star:
	{

	}break;
	case eITEM_FLAG_giant:
	{

	}break;

	default:
	{

	}
	}
}

void CBulletNormal::Exit(CBullet* bullet)
{
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
	bullet->setTargetVec(bullet->getPlanet()->getPosition());
}

void CBulletMagnetItem::Execute(CBullet* bullet, float delta)
{
	//CCLOG("Magnet Item %f", delta);

	/*Vec2 SteeringForce = bullet->getSteeringBehavior()->CalculateBehaviorVector();
	SteeringForce.normalize();
	bullet->setVelocityVec(SteeringForce * (bullet->getBulletSpeed() * delta));

	bullet->setPosition(bullet->getPosition() + bullet->getVelocityVec());*/
	
	bullet->Seek(delta);

	if (bullet->IsHit(bullet->getPlayer()))
	{
		bullet->CollisionWithPlayer();
	}
	else if (bullet->IsHit(bullet->getPlayer()->getPosition(), bullet->getPlayer()->getMagnetLimitRadius()))
	{
		bullet->setTargetVec(bullet->getPlayer()->getPosition());
		bullet->setBulletSpeed(bullet->getBulletSpeed() + 50);
	}
	else if (bullet->IsHit(bullet->getPlanet()))
	{
		bullet->CollisionWithPlanet();
	}
	if (!(CItemManager::Instance()->getCurrentItem() & eITEM_FLAG_magnet))
	{
		bullet->getFSM()->ChangeState(CBulletNormal::Instance());
	}
}

void CBulletMagnetItem::Exit(CBullet* bullet)
{
}