#pragma once
#include "RandomShooter.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"
#include "../../GameObject/Stage/StageManager.h"
#include "../../DataManager/BulletDataManager.h"

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

void CRandomShooter::ShootWithPosition(sSHOOTER_PARAM param, float angle, float distance/* = 0.0f*/)
{
	m_fIntervalRandom = random<float>(0.0f, m_ShooterParam._fInterval);
	m_ShooterParam._fAngle = random<float>(0.f, 360.f);

	m_nBulletCountAtOnceRandom = random<int>(1, m_ShooterParam._nBulletCountAtOneShoot);

	sBULLET_PARAM bulletParam = CBulletDataManager::Instance()->getBulletInfo(param._randomShootSymbol);
	bulletParam._isFly = param._isFly;

	CGameScene::getGameScene()->addChild(CNormalBullet::create(
		bulletParam,
		m_ShooterParam._fAngle,			//초기 각도
		m_ShooterParam._fSpeed));		//속도
}

void CRandomShooter::ShootOnce()
{
	ShootWithPosition(m_ShooterParam, 0.f);
}

void CRandomShooter::Execute(float delta) {

	m_fTime += delta;
	if (m_fTime >= m_fIntervalRandom)
	{
		for (int count = 0; count < m_nBulletCountAtOnceRandom; count++)
		{
			
			ShootOnce();
			m_fTime = 0.f;
		}
	}
}
