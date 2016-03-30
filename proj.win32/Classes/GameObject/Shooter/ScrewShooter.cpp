#pragma once
#include "ScrewShooter.h"
#include "ShooterHeaders.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"
#include "../../GameObject/Stage/StageManager.h"

//-------------------Screw Shooter--------------------
CScrewShooter::CScrewShooter(sSHOOTER_PARAM param)
	: CShooter(param)
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
	if (m_fTime >= m_ShooterParam._fInterval){
		for (int count = 0; count < m_ShooterParam._nBulletCountAtOneShoot; count++){
			if (m_ShooterParam._nDir == eSHOOTER_OPTION_right)
				m_ShooterParam._fAngle -= 9;
			else
				m_ShooterParam._fAngle += 9;
			CGameScene::getGameScene()->addChild(CNormalBullet::create(
				"bullet_1.png",						//�̹��� �̸�
				5.f,								//�浹 ����
				m_ShooterParam._fAngle,				//�ʱ� ����
				m_ShooterParam._fSpeed));			//�ӵ�

			m_ShooterParam._fAngle = static_cast<int>(m_ShooterParam._fAngle) % 360;
			m_fTime = 0.f;
		}
	}
}