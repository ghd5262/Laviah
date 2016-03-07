#pragma once
#include "Shooter.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../../Scene/GameScene.h"

//-------------------Random Shooter-------------------
CRandomShooter* CRandomShooter::create(float interval)				// Bullet ���� ����
{
	CRandomShooter* pRet = (CRandomShooter*)CObjectManager::Instance()->EnemyNew();
	if (pRet && pRet->initVariable(interval))
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

// �̰��� Enemy�� ������Ʈ Ǯ���� ���������� ȣ���ϴ� �κ��̴� addChild�� �������ؼ��� �ȵȴ�.
bool CRandomShooter::initVariable(float interval)
{
	m_fBoundingRadius = 0.0f;
	m_fInterval = interval;
	return true;
}

void CRandomShooter::Execute(float delta/* = 0.f*/) {
	m_fTime += delta;
	if (m_fTime >= m_fRandomInterval){
		for (int i = 0; i < 30; i++){
			m_fRandomInterval = random<float>(0.0f, m_fInterval);
			m_fShotAngle = random<float>(0.f, 360.f);
			m_nBulletColor = random<int>(1, 2);
			CBullet::create(
				MakeString("bullet_%d.png", m_nBulletColor),		//�̹��� �̸�
				5.f,												//�浹 ����
				m_fShotAngle,										//�ʱ� ����
				250.0f,												//�ӵ�
				CObjectManager::Instance()->getM_Planet());			//Ÿ��

			m_fTime = 0.f;
		}
	}
}	

void* CRandomShooter::operator new (size_t size, const std::nothrow_t)
{
	// ObjectManager���� �޸𸮸� �Ҵ� �޴´�.
	return CObjectManager::Instance()->EnemyNew();
}

void RandomShoot() {
	CRandomShooter::create(0.5f);
}
//----------------------------------------------------
