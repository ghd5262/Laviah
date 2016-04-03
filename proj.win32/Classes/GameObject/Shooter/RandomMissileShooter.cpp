#pragma once
#include "RandomMissileShooter.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"
#include "../../GameObject/Stage/StageManager.h"

CRandomMissileShooter::CRandomMissileShooter(sSHOOTER_PARAM param)
	: CShooter(param){};

CRandomMissileShooter* CRandomMissileShooter::create(sSHOOTER_PARAM param)
{
	CRandomMissileShooter* pRet = (CRandomMissileShooter*)new(std::nothrow)
		CRandomMissileShooter(param);
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

void CRandomMissileShooter::ShootOnce()
{
	m_ShooterParam._fAngle = random<float>(0.f, 360.f);
	CGameScene::getGameScene()->addChild(CNormalMissile::create(
		"missile_1.png",				//이미지 이름
		5.f,							//충돌 범위
		m_ShooterParam._fAngle,			//초기 각도
		m_ShooterParam._fSpeed));		//속도
}

void CRandomMissileShooter::Execute(float delta) 
{
	m_fTime += delta;

	if (m_fTime >= m_ShooterParam._fInterval)
	{
		for (int count = 0; count < m_nBulletCountAtOnceRandom; count++)
		{
			m_nBulletCountAtOnceRandom = random<int>(1, m_ShooterParam._nBulletCountAtOneShoot);
			ShootOnce();
			m_fTime = 0.f;
		}
	}
}
