#pragma once
#include "ScrewShooter.h"
#include "ShooterHeaders.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"
#include "../../GameObject/Stage/StageManager.h"

//-------------------Screw Shooter--------------------
CScrewShooter::CScrewShooter(sSHOOTER_PARAM param)
	: CShooter(param)
	, m_fHeightInterval(0.f)
{

	if (m_ShooterParam._nDir == eSHOOTER_OPTION_right)
		m_ShooterParam._fAngle = 120.0f;
	else
		m_ShooterParam._fAngle = 60.0f;
};

CScrewShooter* CScrewShooter::create(sSHOOTER_PARAM param)
{
	CScrewShooter* pRet = (CScrewShooter*)new(std::nothrow)
		CScrewShooter(param);
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
	m_fHeightInterval += delta;
	if (m_fTime <= m_ShooterParam._fEndTime){
		if (m_fHeightInterval >= 0.1f){
			for (int count = 0; count < m_ShooterParam._nBulletCountAtOneShoot; count++){
				if (m_ShooterParam._nDir == eSHOOTER_OPTION_right)
					m_ShooterParam._fAngle -= 6;
				else
					m_ShooterParam._fAngle += 6;
				CGameScene::getGameScene()->addChild(CNormalBullet::create(
					"bullet_2.png",						//이미지 이름
					5.f,								//충돌 범위
					m_ShooterParam._fAngle,				//초기 각도
					m_ShooterParam._fSpeed));			//속도

				m_ShooterParam._fAngle = static_cast<int>(m_ShooterParam._fAngle) % 360;
				m_fHeightInterval = 0.f;
			}
		}
	}
	else
	{
		if (m_fTime >= m_ShooterParam._fInterval + m_ShooterParam._fEndTime){
			m_fTime = 0.f;
			m_ShooterParam._nDir = random<int>(0, 1);
		}
	}


	
}