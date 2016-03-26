#pragma once
#include "AimingMissileShooter.h"
#include "ShooterHeaders.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"

CAimingMissileShooter::CAimingMissileShooter(float speed, float interval)
	: CShooter(interval, speed, 1)
{};

CAimingMissileShooter* CAimingMissileShooter::create(float speed, float interval)				// Bullet ���� ����
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
			"missile_2.png",									//�̹��� �̸�
			5.f,												//�浹 ����
			90.f,												//�ʱ� ����
			m_fShotSpeed,										//�ӵ�
			true);												//���ع̻������� ���� true = CrushShake ȣ��
		CGameScene::getGameScene()->addChild(missile);

		//CGameScene::getGameScene()->addChild(CTargetMark::create(
		//	"missile_target_2.png",								//�̹��� �̸�
		//	0.f,												//�浹 ����
		//	90.f,												//�ʱ� ����
		//	0.f,												//�ӵ�
		//	missile,											//������
		//	true));

		m_fTime = 0.f;
	}
}

void AimingMissileShoot(float speed/* = 1200*/, float interval/* = 0.1f*/) {
	CGameScene::getGameScene()->addChild(CAimingMissileShooter::create(speed, interval));
}