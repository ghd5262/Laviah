#pragma once
#include "DoubleScrewShooter.h"
#include "ShooterHeaders.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"

CDoubleScrewShooter::CDoubleScrewShooter(
	float speed,
	float interval,
	int bulletCountAtOneShoot,
	eSHOOTER_OPTION direction)

	: CShooter(interval, speed, bulletCountAtOneShoot)
{
	if (direction == eSHOOTER_OPTION_right)
		m_fShotAngle = 120.0f;
	else
		m_fShotAngle = 60.0f;
}

CDoubleScrewShooter* CDoubleScrewShooter::create(
	float speed, 
	float interval, 
	int bulletCountAtOneShoot,
	eSHOOTER_OPTION direction)
{
	CDoubleScrewShooter* pRet = (CDoubleScrewShooter*)new(std::nothrow)
		CDoubleScrewShooter(speed, interval, bulletCountAtOneShoot, direction);
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

void CDoubleScrewShooter::Execute(float delta) {
	m_fTime += delta;

	if (m_fTime >= m_fInterval)
	{
		for (int count = 0; count < m_nBulletCountAtOneShoot; count++)
		{
			if (m_Direction == eSHOOTER_OPTION_right)
				m_fShotAngle -= 9;
			else
				m_fShotAngle += 9;

			CGameScene::getGameScene()->addChild(CNormalBullet::create(
				"bullet_1.png",										//이미지 이름
				5.f,												//충돌 범위
				m_fShotAngle,										//초기 각도
				m_fShotSpeed));										//속도

			CGameScene::getGameScene()->addChild(CNormalBullet::create(
				"bullet_1.png",										//이미지 이름
				5.f,												//충돌 범위
				m_fShotAngle + 180,									//초기 각도
				m_fShotSpeed));										//속도

			m_fTime = 0.f;
		}
	}
}

void DoubleScrewShoot(
	float speed/* = 250*/, 
	float interval/* = 0.1f*/, 
	int bulletCountAtOneShoot/* = 1*/,
	eSHOOTER_OPTION direction /*= eSHOOTER_OPTION_right*/)
{
	CGameScene::getGameScene()->addChild(CDoubleScrewShooter::create(speed, interval, bulletCountAtOneShoot, direction));
}