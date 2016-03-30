#pragma once
#include "DoubleScrewShooter.h"
#include "ShooterHeaders.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"
#include "../../GameObject/Stage/StageManager.h"

CDoubleScrewShooter::CDoubleScrewShooter(sSHOOTER_PARAM param)
	: CShooter(param)
{
	if (m_ShooterParam._nDir == eSHOOTER_OPTION_right)
		m_ShooterParam._fAngle = 120.0f;
	else
		m_ShooterParam._fAngle = 60.0f;
}

CDoubleScrewShooter* CDoubleScrewShooter::create(sSHOOTER_PARAM param)
{
	CDoubleScrewShooter* pRet = (CDoubleScrewShooter*)new(std::nothrow)
		CDoubleScrewShooter(param);
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

	if (m_fTime >= m_ShooterParam._fInterval)
	{
		for (int count = 0; count < m_ShooterParam._nBulletCountAtOneShoot; count++)
		{
			if (m_ShooterParam._nDir == eSHOOTER_OPTION_right)
				m_ShooterParam._fAngle -= 9;
			else
				m_ShooterParam._fAngle += 9;

			CGameScene::getGameScene()->addChild(CNormalBullet::create(
				"bullet_1.png",					//이미지 이름
				5.f,							//충돌 범위
				m_ShooterParam._fAngle,			//초기 각도
				m_ShooterParam._fSpeed));		//속도

			CGameScene::getGameScene()->addChild(CNormalBullet::create(
				"bullet_1.png",					//이미지 이름
				5.f,							//충돌 범위
				m_ShooterParam._fAngle + 180,	//초기 각도
				m_ShooterParam._fSpeed));		//속도

			m_fTime = 0.f;
		}
	}
}
