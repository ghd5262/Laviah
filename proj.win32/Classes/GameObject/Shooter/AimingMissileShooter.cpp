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
			"missile_2.png",					//�̹��� �̸�
			5.f,								//�浹 ����
			90.f,								//�ʱ� ����
			m_ShooterParam._fSpeed,				//�ӵ�
			true);								//���ع̻������� ���� true = CrushShake ȣ��
		CGameScene::getGameScene()->addChild(missile);
		m_fTime = 0.f;
	}
}