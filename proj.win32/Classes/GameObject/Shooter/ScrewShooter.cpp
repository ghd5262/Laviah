#pragma once
#include "ScrewShooter.h"
#include "ShooterHeaders.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"

//-------------------Screw Shooter--------------------
CScrewShooter::CScrewShooter(
	float speed,
	float interval, 
	int bulletCountAtOneShoot,
	eSHOOTER_OPTION direction)

	: CShooter(interval, speed, bulletCountAtOneShoot)
	, m_Direction(direction)
{
	if (direction == eSHOOTER_OPTION_right)
		m_fShotAngle = 120.0f;
	else
		m_fShotAngle = 60.0f;
};

CScrewShooter* CScrewShooter::create(
	float speed, 
	float interval, 
	int bulletCountAtOneShoot,
	eSHOOTER_OPTION direction)
{
	CScrewShooter* pRet = (CScrewShooter*)new(std::nothrow)
		CScrewShooter(speed, interval, bulletCountAtOneShoot, direction);
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

void CScrewShooter::Execute(float delta) {
	m_fTime += delta;
	if (m_fTime >= m_fInterval){
		for (int count = 0; count < m_nBulletCountAtOneShoot; count++){
			if (m_Direction == eSHOOTER_OPTION_right)
				m_fShotAngle -= 9;
			else
				m_fShotAngle += 9;
			CGameScene::getGameScene()->addChild(CNormalBullet::create(
				"bullet_1.png",										//이미지 이름
				5.f,												//충돌 범위
				m_fShotAngle,										//초기 각도
				m_fShotSpeed));										//속도

			m_fShotAngle = static_cast<int>(m_fShotAngle) % 360;
			m_fTime = 0.f;
		}
	}
}

void ScrewShoot(
	float speed/* = 250*/, 
	float interval/* = 0.1f*/,
	int bulletCountAtOneShoot/* = 1*/,
	eSHOOTER_OPTION direction /*= eSHOOTER_OPTION_right*/)
{
	CGameScene::getGameScene()->addChild(CScrewShooter::create(speed, interval, bulletCountAtOneShoot, direction));
}
//----------------------------------------------------

