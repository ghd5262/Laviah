#pragma once
#include "Shooter.h"
#include "../Planet.h"
#include "../ObjectManager.h"
#include "../../Scene/GameScene.h"
#include "../../Task/PoolingManager.h"

using namespace Shooter;

//-------------------Random Shooter-------------------
CRandomShooter::CRandomShooter(float speed, float interval, int maxBulletCount)
	: CEnemy(interval)
	, m_fRandomInterval(0.0f)
	, m_nBulletColor(1)
	, m_nMax(maxBulletCount)
	, m_nBulletCount(m_nMax){};

CRandomShooter* CRandomShooter::create(float speed, float interval, int maxBulletCount)				// Bullet 생성 간격
{
	CRandomShooter* pRet = (CRandomShooter*)new(std::nothrow)CRandomShooter(speed, interval, maxBulletCount);
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
	if (m_fTime >= m_fRandomInterval){
		for (int count = 0; count < m_nBulletCount; count++){
			m_fRandomInterval = random<float>(0.0f, m_fInterval);
			m_fShotAngle = random<float>(0.f, 360.f);
			m_nBulletColor = random<int>(1, 2);
			m_nBulletCount = random<int>(1, m_nMax);
			addChild(CBullet::create(
				MakeString("bullet_%d.png", m_nBulletColor),		//이미지 이름
				5.f,												//충돌 범위
				m_fShotAngle,										//초기 각도
				m_fShotSpeed,										//속도
				CObjectManager::Instance()->getM_Planet()));		//타겟

			m_fTime = 0.f;
		}
	}
}	

void Shooter::RandomShoot(float speed/* = 250*/, float interval/* = 0.1f*/, int maxBulletCount /* = 1*/) {
	CGameScene::getGameScene()->addChild(CRandomShooter::create(speed, interval, maxBulletCount));
}
//----------------------------------------------------


//-------------------Screw Shooter--------------------
CScrewShooter::CScrewShooter(float speed, float interval, int bulletCount, SHOOTER_OPTION direction)
	: CEnemy(interval)
	, m_Direction(direction)
	, m_nBulletColor(random<int>(1, 2))
	, m_nBulletCount(bulletCount){
	if (direction == RIGHT)
		m_fShotAngle = 120.0f;
	else
		m_fShotAngle = 60.0f;
};

CScrewShooter* CScrewShooter::create(float speed, float interval, int bulletCount, SHOOTER_OPTION direction)
{
	CScrewShooter* pRet = (CScrewShooter*)new(std::nothrow)CScrewShooter(speed, interval, bulletCount, direction);
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
	if (m_fTime >= m_fInterval){
		for (int count = 0; count < m_nBulletCount; count++){
			if (m_Direction == RIGHT)
				m_fShotAngle -= 9;
			else
				m_fShotAngle += 9;
			addChild(CBullet::create(
				MakeString("bullet_%d.png", m_nBulletColor),		//이미지 이름
				5.f,												//충돌 범위
				m_fShotAngle,										//초기 각도
				m_fShotSpeed,										//속도
				CObjectManager::Instance()->getM_Planet()));		//타겟

			m_fTime = 0.f;
		}
	}
}

void Shooter::ScrewShoot(float speed/* = 250*/, float interval/* = 0.1f*/, int bulletCount/* = 1*/, SHOOTER_OPTION direction /*= RIGHT*/) {
	CGameScene::getGameScene()->addChild(CScrewShooter::create(speed, interval, bulletCount, direction));
}
//----------------------------------------------------


//-------------------DoubleScrew Shooter--------------------
CDoubleScrewShooter::CDoubleScrewShooter(float speed, float interval, int bulletCount, SHOOTER_OPTION direction)
	: CEnemy(interval)
	, m_Direction(direction)
	, m_nBulletColor(random<int>(1, 2))
	, m_nBulletCount(bulletCount){
	if (direction == RIGHT)
		m_fShotAngle = 120.0f;
	else 
		m_fShotAngle = 60.0f;
};

CDoubleScrewShooter* CDoubleScrewShooter::create(float speed, float interval, int bulletCount, SHOOTER_OPTION direction)
{
	CDoubleScrewShooter* pRet = (CDoubleScrewShooter*)new(std::nothrow)CDoubleScrewShooter(speed, interval, bulletCount, direction);
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
	if (m_fTime >= m_fInterval){
		for (int count = 0; count < m_nBulletCount; count++){
			if (m_Direction == RIGHT)
				m_fShotAngle -= 9;
			else
				m_fShotAngle += 9;
			addChild(CBullet::create(
				MakeString("bullet_%d.png", m_nBulletColor),		//이미지 이름
				5.f,												//충돌 범위
				m_fShotAngle,										//초기 각도
				m_fShotSpeed,										//속도
				CObjectManager::Instance()->getM_Planet()));		//타겟

			addChild(CBullet::create(
				MakeString("bullet_%d.png", 3 - m_nBulletColor),	//이미지 이름
				5.f,												//충돌 범위
				m_fShotAngle + 180,									//초기 각도
				m_fShotSpeed,										//속도
				CObjectManager::Instance()->getM_Planet()));		//타겟

			m_fTime = 0.f;
		}
	}
}

void Shooter::DoubleScrewShoot(float speed/* = 250*/, float interval/* = 0.1f*/, int bulletCount/* = 1*/, SHOOTER_OPTION direction /*= RIGHT*/) {
	CGameScene::getGameScene()->addChild(CDoubleScrewShooter::create(speed, interval, bulletCount, direction));
}
//----------------------------------------------------


//-------------------Barrier Shooter--------------------
CBarrierShooter::CBarrierShooter(float speed, float interval, int waySize)
	: CEnemy(interval)
	, m_nWaySize(waySize)
	, m_nBulletColor(random<int>(1, 2)){};

CBarrierShooter* CBarrierShooter::create(float speed, float interval, int waySize)
{
	CBarrierShooter* pRet = (CBarrierShooter*)new(std::nothrow)CBarrierShooter(speed, interval, waySize);
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

void CBarrierShooter::Execute(float delta) {
	m_fTime += delta;
	if (m_fTime >= m_fInterval){
		/*if (m_Direction == RIGHT)
			m_fShotAngle -= 9;
		else
			m_fShotAngle += 9;*/
		addChild(CBullet::create(
			MakeString("bullet_%d.png", m_nBulletColor),		//이미지 이름
			5.f,												//충돌 범위
			m_fShotAngle,										//초기 각도
			m_fShotSpeed,										//속도
			CObjectManager::Instance()->getM_Planet()));		//타겟

		m_fTime = 0.f;
	}
}

void Shooter::BarrierShoot(float speed/* = 250*/, float interval/* = 0.1f*/, int waySize/* = 10*/) {
	CGameScene::getGameScene()->addChild(CBarrierShooter::create(speed, interval, waySize));
}
//----------------------------------------------------
