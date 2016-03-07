#pragma once
#include "Shooter.h"
#include "../Planet.h"
#include "../ObjectManager.h"
#include "../../Scene/GameScene.h"
#include "../../Task/PoolingManager.h"

//-------------------Random Shooter-------------------
CRandomShooter::CRandomShooter(float interval) 
	: CEnemy(interval)
	, m_fRandomInterval(0.0f)
	, m_fShotAngle(0.0f)
	, m_nBulletColor(1){};

CRandomShooter* CRandomShooter::create(float interval)				// Bullet 생성 간격
{
	CRandomShooter* pRet = (CRandomShooter*)new(std::nothrow)CRandomShooter(interval);
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

void CRandomShooter::Execute(float delta/* = 0.f*/) {
	m_fTime += delta;
	if (m_fTime >= m_fRandomInterval){
		for (int i = 0; i < 30; i++){
			m_fRandomInterval = random<float>(0.0f, m_fInterval);
			m_fShotAngle = random<float>(0.f, 360.f);
			m_nBulletColor = random<int>(1, 2);
			addChild(CBullet::create(
				MakeString("bullet_%d.png", m_nBulletColor),		//이미지 이름
				5.f,												//충돌 범위
				m_fShotAngle,										//초기 각도
				250.0f,												//속도
				CObjectManager::Instance()->getM_Planet()));		//타겟

			m_fTime = 0.f;
		}
	}
}	

void* CRandomShooter::operator new (size_t size, const std::nothrow_t)
{
	// ObjectManager에서 메모리를 할당 받는다.
	return CPoolingManager::Instance()->EnemyNew();
}

void RandomShoot() {
	CGameScene::getGameScene()->addChild(CRandomShooter::create(0.5f));
}
//----------------------------------------------------
