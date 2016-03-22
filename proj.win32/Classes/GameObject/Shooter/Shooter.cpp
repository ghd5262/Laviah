#pragma once
#include "Shooter.h"
#include "../Planet.h"
#include "../Player.h"
#include "../ObjectManager.h"
#include "../Bullet/Bullets.h"
#include "../../Scene/GameScene.h"
#include "../../Task/PoolingManager.h"
#include "../../MyUI/UIManager.h"
#include "../../MyUI/BonusTimeUI.h"
using namespace Shooter;

//-------------------Random Shooter-------------------
CRandomShooter::CRandomShooter(float speed, float interval, int maxBulletCount)
	: CEnemy(interval, speed)
	, m_fRandomInterval(0.0f)
	, m_nBulletColor(1)
	, m_nMax(maxBulletCount)
	, m_nBulletCount(m_nMax){};

CRandomShooter* CRandomShooter::create(float speed, float interval, int maxBulletCount)				// Bullet ���� ����
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
			CGameScene::getGameScene()->addChild(CNormalBullet::create(
				"bullet_1.png",										//�̹��� �̸�
				5.f,												//�浹 ����
				m_fShotAngle,										//�ʱ� ����
				m_fShotSpeed));										//�ӵ�

			m_fTime = 0.f;
		}
	}
}	

void Shooter::RandomShoot(float speed/* = 250*/, float interval/* = 0.1f*/, int maxBulletCount /* = 1*/) {
	CGameScene::getGameScene()->addChild(CRandomShooter::create(speed, interval, maxBulletCount));
}
//----------------------------------------------------


//-------------------Screw Shooter--------------------
CScrewShooter::CScrewShooter(float speed, float interval, int bulletCount, eSHOOTER_OPTION direction)
	: CEnemy(interval, speed)
	, m_Direction(direction)
	, m_nBulletColor(random<int>(1, 2))
	, m_nBulletCount(bulletCount){
	if (direction == eSHOOTER_OPTION_right)
		m_fShotAngle = 120.0f;
	else
		m_fShotAngle = 60.0f;
};

CScrewShooter* CScrewShooter::create(float speed, float interval, int bulletCount, eSHOOTER_OPTION direction)
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
			if (m_Direction == eSHOOTER_OPTION_right)
				m_fShotAngle -= 9;
			else
				m_fShotAngle += 9;
			CGameScene::getGameScene()->addChild(CNormalBullet::create(
				"bullet_1.png",										//�̹��� �̸�
				5.f,												//�浹 ����
				m_fShotAngle,										//�ʱ� ����
				m_fShotSpeed));										//�ӵ�

			m_fShotAngle = static_cast<int>(m_fShotAngle) % 360;
			m_fTime = 0.f;
		}
	}
}

void Shooter::ScrewShoot(float speed/* = 250*/, float interval/* = 0.1f*/, int bulletCount/* = 1*/, eSHOOTER_OPTION direction /*= eSHOOTER_OPTION_right*/) {
	CGameScene::getGameScene()->addChild(CScrewShooter::create(speed, interval, bulletCount, direction));
}
//----------------------------------------------------


//-------------------DoubleScrew Shooter--------------------
CDoubleScrewShooter::CDoubleScrewShooter(float speed, float interval, int bulletCount, eSHOOTER_OPTION direction)
	: CEnemy(interval, speed)
	, m_Direction(direction)
	, m_nBulletColor(random<int>(1, 2))
	, m_nBulletCount(bulletCount){
	if (direction == eSHOOTER_OPTION_right)
		m_fShotAngle = 120.0f;
	else 
		m_fShotAngle = 60.0f;
};

CDoubleScrewShooter* CDoubleScrewShooter::create(float speed, float interval, int bulletCount, eSHOOTER_OPTION direction)
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
			if (m_Direction == eSHOOTER_OPTION_right)
				m_fShotAngle -= 9;
			else
				m_fShotAngle += 9;
			CGameScene::getGameScene()->addChild(CNormalBullet::create(
				"bullet_1.png",										//�̹��� �̸�
				5.f,												//�浹 ����
				m_fShotAngle,										//�ʱ� ����
				m_fShotSpeed));										//�ӵ�

			CGameScene::getGameScene()->addChild(CNormalBullet::create(
				"bullet_1.png",										//�̹��� �̸�
				5.f,												//�浹 ����
				m_fShotAngle + 180,									//�ʱ� ����
				m_fShotSpeed));										//�ӵ�

			m_fTime = 0.f;
		}
	}
}

void Shooter::DoubleScrewShoot(float speed/* = 250*/, float interval/* = 0.1f*/, int bulletCount/* = 1*/, eSHOOTER_OPTION direction /*= eSHOOTER_OPTION_right*/) {
	CGameScene::getGameScene()->addChild(CDoubleScrewShooter::create(speed, interval, bulletCount, direction));
}
//----------------------------------------------------


//-------------------Barrier Shooter--------------------
CBarrierShooter::CBarrierShooter(float speed, float interval, int waySize)
	: CEnemy(interval, speed)
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
		/*if (m_Direction == eSHOOTER_OPTION_right)
			m_fShotAngle -= 9;
		else
			m_fShotAngle += 9;*/
		CGameScene::getGameScene()->addChild(CNormalBullet::create(
			MakeString("bullet_%d.png", m_nBulletColor),		//�̹��� �̸�
			5.f,												//�浹 ����
			m_fShotAngle,										//�ʱ� ����
			m_fShotSpeed));										//�ӵ�

		m_fTime = 0.f;
	}
}

void Shooter::BarrierShoot(float speed/* = 250*/, float interval/* = 0.1f*/, int waySize/* = 10*/) {
	CGameScene::getGameScene()->addChild(CBarrierShooter::create(speed, interval, waySize));
}
//----------------------------------------------------



//-------------------RandomMissile Shooter-------------------
CRandomMissileShooter::CRandomMissileShooter(float speed, float interval, int maxBulletCount)
	: CEnemy(interval, speed)
	, m_nMax(maxBulletCount)
	, m_nBulletCount(m_nMax){};

CRandomMissileShooter* CRandomMissileShooter::create(float speed, float interval, int maxBulletCount)				// Bullet ���� ����
{
	CRandomMissileShooter* pRet = (CRandomMissileShooter*)new(std::nothrow)CRandomMissileShooter(speed, interval, maxBulletCount);
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

void CRandomMissileShooter::Execute(float delta) {
	m_fTime += delta;
	if (m_fTime >= m_fInterval){
		for (int count = 0; count < m_nBulletCount; count++){
			m_fShotAngle = random<float>(0.f, 360.f);
			m_nBulletCount = random<int>(1, m_nMax);
			auto missile = CNormalMissile::create(
				"missile_1.png",									//�̹��� �̸�
				5.f,												//�浹 ����
				m_fShotAngle,										//�ʱ� ����
				m_fShotSpeed);										//�ӵ�
			CGameScene::getGameScene()->addChild(missile);


			CGameScene::getGameScene()->addChild(CTargetMark::create(
				"missile_target_1.png",								//�̹��� �̸�
				0.f,												//�浹 ����
				m_fShotAngle,										//�ʱ� ����
				0.f,												//�ӵ�
				missile));											//������

			m_fTime = 0.f;
		}
	}
}

void Shooter::RandomMissileShoot(float speed/* = 600*/, float interval/* = 0.1f*/, int maxBulletCount /* = 1*/) {
	CGameScene::getGameScene()->addChild(CRandomMissileShooter::create(speed, interval, maxBulletCount));
}
//----------------------------------------------------



//-------------------AimingMissile Shooter-------------------
CAimingMissileShooter::CAimingMissileShooter(float speed, float interval)
	: CEnemy(interval, speed)
{};

CAimingMissileShooter* CAimingMissileShooter::create(float speed, float interval)				// Bullet ���� ����
{
	CAimingMissileShooter* pRet = (CAimingMissileShooter*)new(std::nothrow)CAimingMissileShooter(speed, interval);
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

void CAimingMissileShooter::Execute(float delta) {
	m_fTime += delta;
	if (m_fTime >= m_fInterval){
		BonusLetterShoot();
		PlayItemShoot();
		m_fShotAngle = random<float>(60.f, 120.f);
		auto missile = CNormalMissile::create(
			"missile_2.png",									//�̹��� �̸�
			5.f,												//�浹 ����
			90.f,												//�ʱ� ����
			m_fShotSpeed,										//�ӵ�
			true);												//���ع̻������� ���� true = CrushShake ȣ��
		CGameScene::getGameScene()->addChild(missile);


		CGameScene::getGameScene()->addChild(CTargetMark::create(
			"missile_target_2.png",								//�̹��� �̸�
			0.f,												//�浹 ����
			90.f,												//�ʱ� ����
			0.f,												//�ӵ�
			missile,											//������
			true));
		m_fTime = 0.f;
	}
}

void Shooter::AimingMissileShoot(float speed/* = 600*/, float interval/* = 0.1f*/) {
	CGameScene::getGameScene()->addChild(CAimingMissileShooter::create(speed, interval));
}
//----------------------------------------------------


//-------------------BonusLetter Shooter-------------------
void Shooter::BonusLetterShoot(float speed/* = 100*/, float interval/* = 0.1f*/, int maxBulletCount /* = 1*/) {
	CBonusTimeUI* gameSceneUIManager = static_cast<CBonusTimeUI*>(CUIManager::Instance()->FindUIWithName("BonusTime"));
	Vec2 targetPos = gameSceneUIManager->NonCollectedLetterWorldPos(); // ���� ���ĺ��� ���� ������
	int letterNum = static_cast<int>(gameSceneUIManager->NonCollectedLetterNum()); // ���� ���ĺ��� �ѹ�
	
	float shotAngle = random<float>(0.f, 360.f);
	CGameScene::getGameScene()->addChild(CBonusLetter::create(
		MakeString("bonusLetter_%d.png", letterNum),					//�̹��� �̸�
		20.f,															//�浹 ����
		shotAngle,														//�ʱ� ����
		speed));														//�ӵ�
}
//----------------------------------------------------


//-------------------BonusLetter Shooter-------------------
void Shooter::PlayItemShoot(float speed/* = 100*/, float interval/* = 0.1f*/, int maxBulletCount /* = 1*/) {

	float shotAngle = random<float>(0.f, 360.f);
	int itemNum = random<int>(0, static_cast<int>(eITEM_TYPE_MAX - 1));

	CGameScene::getGameScene()->addChild(CPlayItem::create(
		MakeString("playItem_%d.png", itemNum),							//�̹��� �̸�
		20.f,															//�浹 ����
		shotAngle,														//�ʱ� ����
		speed,															//�ӵ�
		itemNum,														//item type
		true));															//fly Item ����
	CGameScene::getGameScene()->addChild(CPlayItem::create(
		MakeString("playItem_%d.png", itemNum),							//�̹��� �̸�
		20.f,															//�浹 ����
		shotAngle,														//�ʱ� ����
		speed,															//�ӵ�
		itemNum,														//item type
		false));														//fly Item ����
}
//----------------------------------------------------