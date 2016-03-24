#pragma once
#include "RandomShooter.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"

CRandomShooter::CRandomShooter(
	float speed, 
	float interval,
	int bulletCountAtOneShoot)

	: CShooter(interval, speed, bulletCountAtOneShoot)
{}

CRandomShooter* CRandomShooter::create(float speed, float interval, int bulletCountAtOneShoot)				// Bullet 생성 간격
{
	CRandomShooter* pRet = (CRandomShooter*)new(std::nothrow)
		CRandomShooter(speed, interval, bulletCountAtOneShoot);
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
			m_fIntervalRandom = random<float>(0.0f, m_fInterval);
			m_fShotAngle = random<float>(0.f, 360.f);
			m_nBulletCountAtOnceRandom = random<int>(1, m_nBulletCountAtOneShoot);
			CGameScene::getGameScene()->addChild(CNormalBullet::create(
				"bullet_1.png",										//이미지 이름
				5.f,												//충돌 범위
				m_fShotAngle,										//초기 각도
				m_fShotSpeed));										//속도

			m_fTime = 0.f;
		}
	}
}

void RandomShoot(
	float speed/* = 250*/, 
	float interval/* = 0.1f*/,
	int bulletCountAtOnceMax /* = 1*/) 
{
	CGameScene::getGameScene()->addChild(CRandomShooter::create(speed, interval, bulletCountAtOnceMax));
}
//----------------------------------------------------