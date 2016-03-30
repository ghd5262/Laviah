#pragma once
#include "RandomShooter.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"
#include "../../GameObject/Stage/StageManager.h"

CRandomShooter::CRandomShooter(sSHOOTER_PARAM param)
	: CShooter(param)
{}

CRandomShooter* CRandomShooter::create(sSHOOTER_PARAM param)				// Bullet ���� ����
{
	CRandomShooter* pRet = (CRandomShooter*)new(std::nothrow)
		CRandomShooter(param);
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

void CRandomShooter::Execute(float delta) {

	m_fTime += delta;
	if (m_fTime >= m_fIntervalRandom)
	{
		for (int count = 0; count < m_nBulletCountAtOnceRandom; count++)
		{
			m_fIntervalRandom = random<float>(0.0f, m_ShooterParam._fInterval);
			m_ShooterParam._fAngle = random<float>(0.f, 360.f);
			m_nBulletCountAtOnceRandom = random<int>(1, m_ShooterParam._nBulletCountAtOneShoot);
			CGameScene::getGameScene()->addChild(CNormalBullet::create(
				"bullet_1.png",					//�̹��� �̸�
				5.f,							//�浹 ����
				m_ShooterParam._fAngle,			//�ʱ� ����
				m_ShooterParam._fSpeed));		//�ӵ�

			m_fTime = 0.f;
		}
	}
}
