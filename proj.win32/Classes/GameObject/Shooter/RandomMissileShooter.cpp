#pragma once
#include "RandomMissileShooter.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"

CRandomMissileShooter::CRandomMissileShooter(
	float speed, 
	float interval,
	int bulletCountAtOneShoot)

	: CShooter(interval, speed, bulletCountAtOneShoot){};

CRandomMissileShooter* CRandomMissileShooter::create(
	float speed, 
	float interval,		// Bullet ���� ����
	int bulletCountAtOneShoot)
{
	CRandomMissileShooter* pRet = (CRandomMissileShooter*)new(std::nothrow)
		CRandomMissileShooter(speed, interval, bulletCountAtOneShoot);
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

void CRandomMissileShooter::Execute(float delta) 
{
	m_fTime += delta;

	if (m_fTime >= m_fInterval)
	{
		for (int count = 0; count < m_nBulletCountAtOnceRandom; count++)
		{
			m_fShotAngle = random<float>(0.f, 360.f);
			m_nBulletCountAtOnceRandom = random<int>(1, m_nBulletCountAtOneShoot);

			auto missile = CNormalMissile::create(
				"missile_1.png",									//�̹��� �̸�
				5.f,												//�浹 ����
				m_fShotAngle,										//�ʱ� ����
				m_fShotSpeed);										//�ӵ�
			CGameScene::getGameScene()->addChild(missile);

			CGameScene::getGameScene()->addChild(CTargetMark::create(
				"missile_target_1.png",								//�̹��� �̸�
				0.f,												//�浹 ����
				m_fShotAngle,										//�ʱ� ����
				0.f,												//�ӵ�
				missile));											//������

			m_fTime = 0.f;
		}
	}
}

void RandomMissileShoot(
	float speed/* = 600*/, 
	float interval/* = 0.1f*/, 
	int bulletCountAtOneShoot /* = 1*/)
{
	CGameScene::getGameScene()->addChild(CRandomMissileShooter::create(speed, interval, bulletCountAtOneShoot));
}
