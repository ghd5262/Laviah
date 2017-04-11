#include "BulletStates.h"
#include "../../GameObject/Planet.h"
#include "../../GameObject/Player.h"
#include "../../GameObject/ObjectManager.h"
#include "../../GameObject/ItemManager.h"
#include "../../GameObject/Bullet/Bullet.h"
#include "../../GameObject/ItemRange.h"
#include "../../DataManager/ChallengeDataManager.hpp"
#include "../../DataManager/ChallengeChecker/ChallengeClearChecker.h"

CBulletNormal* CBulletNormal::Instance()
{
	static CBulletNormal instance;

	return &instance;
}

void CBulletNormal::Enter(CBullet* bullet)
{}

void CBulletNormal::Execute(CBullet* bullet, float delta)
{
	if (bullet->getIsFly())
		bullet->Seek(delta);

	if (bullet->IsHit(bullet->getPlayer()))
	{
		bullet->CollisionWithPlayer();
		return;
	}

	if (bullet->IsEffectWithItem(eITEM_FLAG_shield) && 
		bullet->IsHit(bullet->getPlanet()->getPosition(),
		CObjectManager::Instance()->getBarrierItemRange()->getBoundingRadius()))
	{
		bullet->CollisionWithBarrier();
		return;
	}

	if (bullet->IsEffectWithItem(static_cast<eITEM_FLAG>
		(eITEM_FLAG_magnet & CItemManager::Instance()->getCurrentItem())))
	{
		bullet->ChangeState(CBulletMagnetItem::Instance());
	}

	if (bullet->IsEffectWithItem(eITEM_FLAG_coin) &&
		bullet->IsHit(bullet->getPlayer()->getPosition(),
		CObjectManager::Instance()->getCoinItemRange()->getBoundingRadius()))
	{
		bullet->ChangeToCoin();
		return;
	}

	if (bullet->IsEffectWithItem(eITEM_FLAG_star) &&
		bullet->IsHit(bullet->getPlayer()->getPosition(),
		CObjectManager::Instance()->getStarItemRange()->getBoundingRadius()))
	{
		bullet->ChangeToStar();
		return;
	}

	if (bullet->IsHit(bullet->getPlanet()))
	{
		bullet->CollisionWithPlanet();
		return;
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

    GLOBAL->MAGNET_COUNT += 1;
    GLOBAL->MAGNET_SCORE += (GLOBAL->COMBO_LEVEL * 2);
}

void CBulletMagnetItem::Execute(CBullet* bullet, float delta)
{
	if (bullet->getIsFly() || bullet->getIsPlayerGet())
		bullet->Seek(delta);

	auto player = bullet->getPlayer();
	float magnetSize = player->getMagnetLimitRadius();

	if (bullet->IsHit(bullet->getPlayer()))
	{
		bullet->CollisionWithPlayer();
		return;
	}
	else if (bullet->IsHit(player->getPosition(), magnetSize))
	{
		bullet->setIsPlayerGet(true);
		bullet->setTargetVec(bullet->getPlayer()->getPosition());
		bullet->setSpeed(bullet->getSpeed() + 50);
	}
	else if (bullet->IsHit(bullet->getPlanet()))
	{
		bullet->CollisionWithPlanet();
		return;
	}
	if (!(CItemManager::Instance()->getCurrentItem() & eITEM_FLAG_magnet))
	{
		bullet->ChangeState(CBulletNormal::Instance());
	}
}

void CBulletMagnetItem::Exit(CBullet* bullet)
{
}