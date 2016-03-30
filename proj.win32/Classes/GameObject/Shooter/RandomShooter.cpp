#pragma once
#include "RandomShooter.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"
#include "../../GameObject/Stage/StageManager.h"

CRandomShooter::CRandomShooter(sSHOOTER_PARAM param)
	: CShooter(param)
{}

CRandomShooter* CRandomShooter::create(sSHOOTER_PARAM param)				// Bullet 생성 간격
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
				"bullet_1.png",					//이미지 이름
				5.f,							//충돌 범위
				m_ShooterParam._fAngle,			//초기 각도
				m_ShooterParam._fSpeed));		//속도

			m_fTime = 0.f;
		}
	}
}
