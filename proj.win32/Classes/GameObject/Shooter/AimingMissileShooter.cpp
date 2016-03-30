#pragma once
#include "AimingMissileShooter.h"
#include "ShooterHeaders.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"
#include "../../GameObject/Stage/StageManager.h"

CAimingMissileShooter::CAimingMissileShooter(sSHOOTER_PARAM param)
	: CShooter(param)
{};

CAimingMissileShooter* CAimingMissileShooter::create(sSHOOTER_PARAM param)
{
	CAimingMissileShooter* pRet = (CAimingMissileShooter*)new(std::nothrow)
		CAimingMissileShooter(param);
	if (pRet)
	{
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

void CAimingMissileShooter::Execute(float delta) {
	m_fTime += delta;

	if (m_fTime >= m_ShooterParam._fInterval)
	{
		//BonusLetterShoot();
		//PlayItemShoot();
		m_ShooterParam._fAngle = random<float>(60.f, 120.f);

		auto missile = CNormalMissile::create(
			"missile_2.png",					//이미지 이름
			5.f,								//충돌 범위
			90.f,								//초기 각도
			m_ShooterParam._fSpeed,				//속도
			true);								//조준미사일인지 여부 true = CrushShake 호출
		CGameScene::getGameScene()->addChild(missile);
		m_fTime = 0.f;
	}
}