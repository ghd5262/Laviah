#pragma once
#include "RandomShooter.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/BulletDataManager.h"

CRandomShooter::CRandomShooter(sSHOOTER_PARAM param, float distance)
	: CShooter(param, distance)
{}

CRandomShooter* CRandomShooter::create(sSHOOTER_PARAM param, float distance/* = -1.f */)				// Bullet 생성 간격
{
	CRandomShooter* pRet = (CRandomShooter*)new(std::nothrow)
		CRandomShooter(param, distance);
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

void CRandomShooter::ShootOnce()
{
	m_fIntervalRandom = random<float>
		(m_ShooterParam._fInterval - (m_ShooterParam._fInterval * 0.2f), 
		m_ShooterParam._fInterval + (m_ShooterParam._fInterval * 0.2f));

	m_nBulletCountAtOnceRandom = random<int>(1, m_ShooterParam._nBulletCountAtOneShoot);

	// angle이 마이너스 이면 랜덤으로 사용한다. 
	if (m_ShooterParam._isAngleRandom){
		if (random<int>(0, 100) < 95)
			m_ShooterParam._fAngle = random<float>(0.f, 180.f);
		else
			m_ShooterParam._fAngle = random<float>(180.f, 360.f);
		
	}

	char p = m_ShooterParam._randomShootSymbol;
	sBULLET_PARAM bulletParam = CBulletDataManager::Instance()->getBulletInfo(p);
	bulletParam._isFly = m_ShooterParam._isFly;

	if (p >= '1' && p <= '3') //일반 bullet
	{
		CGameScene::getGridWorld()->addChild(CNormalBullet::create(
			bulletParam,
			m_ShooterParam._fAngle,
			m_ShooterParam._fSpeed));
	}
	else if (p >= '4' && p <= '5')//미사일과 조준미사일
	{
		CGameScene::getGridWorld()->addChild(CNormalMissile::create(
			bulletParam,
			m_ShooterParam._fAngle,
			m_ShooterParam._fSpeed), 101);
	}
    else if (p == '6')
    {
        CGameScene::getGridWorld()->addChild(CStickBullet::create(
            bulletParam,
            m_ShooterParam._fAngle,
            m_ShooterParam._fSpeed));
    }
	else if (p >= 'A' && p <= 'G')//아이템
	{
		CGameScene::getGridWorld()->addChild(CPlayItem::create(
			bulletParam,
			m_ShooterParam._fAngle,
			m_ShooterParam._fSpeed));
	}
	else if (p >= 'P' && p <= 'T')//코인
	{
		CGameScene::getGridWorld()->addChild(CPlayCoin::create(
			bulletParam,
			m_ShooterParam._fAngle,
			m_ShooterParam._fSpeed));
	}
	else if (p >= 'U' && p <= 'Y')//별
	{
		CGameScene::getGridWorld()->addChild(CPlayStar::create(
			bulletParam,
			m_ShooterParam._fAngle,
			m_ShooterParam._fSpeed));
	}
	else if (p == 'Z')//보너스 아이템
	{
		CGameScene::getGridWorld()->addChild(CBonusLetter::create(
			bulletParam,
			m_ShooterParam._fAngle,
			m_ShooterParam._fSpeed));
	}
	else if (p == 'z')//랜덤 아이템
	{
		bulletParam._itemType = static_cast<eITEM_TYPE>(random<int>(eITEM_TYPE_health, eITEM_TYPE_MAX - 2));
        bulletParam._TextureName = MakeString("playItem_%d.png", bulletParam._itemType -1);
		bulletParam._isFly = random<int>(0, 1);
		CGameScene::getGridWorld()->addChild(CPlayItem::create(
			bulletParam,
			m_ShooterParam._fAngle,
			m_ShooterParam._fSpeed));
	}
}

void CRandomShooter::Execute(float delta) {

	if (m_isShooterPause)
		return;

	m_fIntervalTimer += delta;
	m_fTime += delta;
	if (m_fIntervalTimer >= m_fIntervalRandom)
	{
		for (int count = 0; count < m_nBulletCountAtOnceRandom; count++)
		{
			ShootOnce();
			m_fIntervalTimer = 0.f;
		}
	}
	if (m_ShooterParam._isOneShoot || m_fTime >= m_ShooterParam._fEndTime)
	{
		ReturnToMemoryBlock();
	}
}

