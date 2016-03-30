#include "BulletStates.h"
#include "../../GameObject/Planet.h"
#include "../../GameObject/Player.h"
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
	if (true == bullet->getIsFlyItem())
		bullet->Seek(delta);

	if (bullet->IsHit(bullet->getPlayer()))
	{
		bullet->CollisionWithPlayer();
	}

	if (bullet->IsHit(bullet->getPlanet()))
	{
		bullet->CollisionWithPlanet();
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
		bullet->ChangeToCoin();
	}break;
	case eITEM_FLAG_star:
	{
		bullet->ChangeToStar();
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
	if (true == bullet->getIsFlyItem() || bullet->getIsPlayerGet())
		bullet->Seek(delta);

	if (bullet->IsHit(bullet->getPlayer()))
	{
		bullet->CollisionWithPlayer();
	}
	else if (bullet->IsHit(bullet->getPlayer()->getPosition(), bullet->getPlayer()->getMagnetLimitRadius()))
	{
		bullet->setIsPlayerGet(true);
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