#pragma once
#include "AimingMissileShooter.h"
#include "ShooterHeaders.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"

CAimingMissileShooter::CAimingMissileShooter(float speed, float interval)
	: CShooter(interval, speed, 1)
{};

CAimingMissileShooter* CAimingMissileShooter::create(float speed, float interval)				// Bullet 생성 간격
{
	CAimingMissileShooter* pRet = (CAimingMissileShooter*)new(std::nothrow)
		CAimingMissileShooter(speed, interval);
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

	if (m_fTime >= m_fInterval)
	{
		BonusLetterShoot();
		PlayItemShoot();
		m_fShotAngle = random<float>(60.f, 120.f);

		auto missile = CNormalMissile::create(
			"missile_2.png",									//이미지 이름
			5.f,												//충돌 범위
			90.f,												//초기 각도
			m_fShotSpeed,										//속도
			true);												//조준미사일인지 여부 true = CrushShake 호출
		CGameScene::getGameScene()->addChild(missile);

		//CGameScene::getGameScene()->addChild(CTargetMark::create(
		//	"missile_target_2.png",								//이미지 이름
		//	0.f,												//충돌 범위
		//	90.f,												//초기 각도
		//	0.f,												//속도
		//	missile,											//소유자
		//	true));

		m_fTime = 0.f;
	}
}

void AimingMissileShoot(float speed/* = 1200*/, float interval/* = 0.1f*/) {
	CGameScene::getGameScene()->addChild(CAimingMissileShooter::create(speed, interval));
}