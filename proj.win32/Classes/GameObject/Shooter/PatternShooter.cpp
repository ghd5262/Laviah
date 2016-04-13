#pragma once
#include "PatternShooter.h"
#include "../Bullet/BulletHeaders.h"
#include "../../DataManager/BulletPatternDataManager.h"
#include "../../DataManager/BulletDataManager.h"
#include "../../Scene/GameScene.h"

CPatternShooter::CPatternShooter(sSHOOTER_PARAM param, float distance)
	: CShooter(param, distance)
{}

CPatternShooter* CPatternShooter::create(sSHOOTER_PARAM param, float distance/* = -1*/)
{
	CPatternShooter* pRet = (CPatternShooter*)new(std::nothrow)
		CPatternShooter(param, distance);
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

void CPatternShooter::ShootOnce()
{
	sPATTERN_SHOOTER_PARAM pattern = 
		CBulletPatternDataManager::Instance()->getPatternInfo(m_ShooterParam._PatternName);
    
    if (m_ShooterParam._isAngleRandom)
        m_ShooterParam._fAngle = random<float>(0.f, 360.f);
	
    for (int i = 0; i < pattern._height; i++) {

		for (int j = 0; j < pattern._width; j++){

			char p = *(pattern._pattern + (pattern._width * i) + j);
			if (p != ' ') {

				sBULLET_PARAM bulletParam = CBulletDataManager::Instance()->getBulletInfo(p);

				float initDistance = 0.f;

				// 0보다 작으면(default -1) bullet에 설정된 distance로 사용한다.
				if (m_fDistance < 0.0f)
					initDistance = bulletParam._fDistance;
				else
					initDistance = m_fDistance;

                
                
				bulletParam._fDistance = initDistance + ((pattern._height - i) * pattern._heightDistance);
				bulletParam._isFly = m_ShooterParam._isFly;

				float bulletAngle = (j * -pattern._widthAngleDistance) + m_ShooterParam._fAngle + 18.f;

				if (p >= '1' && p <= '3') //일반 bullet
				{
					CGameScene::getGameScene()->addChild(CNormalBullet::create(
						bulletParam,
						bulletAngle,
						m_ShooterParam._fSpeed));
				}
				else if (p >= '4' && p <= '5')//미사일과 조준미사일
				{
					CGameScene::getGameScene()->addChild(CNormalMissile::create(
						bulletParam,
						bulletAngle,
						m_ShooterParam._fSpeed));
				}
				else if (p >= 'A' && p <= 'G')//아이템
				{
					CGameScene::getGameScene()->addChild(CPlayItem::create(
						bulletParam,
						bulletAngle,
						m_ShooterParam._fSpeed));
				}
				else if (p >= 'P' && p <= 'T')//코인
				{
					CGameScene::getGameScene()->addChild(CPlayCoin::create(
						bulletParam,
						bulletAngle,
						m_ShooterParam._fSpeed));
				}
				else if (p >= 'U' && p <= 'Y')//별
				{
					CGameScene::getGameScene()->addChild(CPlayStar::create(
						bulletParam,
						bulletAngle,
						m_ShooterParam._fSpeed));
				}
				else if (p == 'Z')//보너스 아이템
				{
					CGameScene::getGameScene()->addChild(CBonusLetter::create(
						bulletParam,
						bulletAngle,
						m_ShooterParam._fSpeed));
				}
				else if (p == 'z')//랜덤 아이템
				{
					bulletParam._itemType = static_cast<eITEM_TYPE>(random<int>(eITEM_TYPE_health, eITEM_TYPE_MAX - 2));
                    bulletParam._TextureName = MakeString("playItem_%d.png", bulletParam._itemType - 1);
					CGameScene::getGameScene()->addChild(CPlayItem::create(
						bulletParam,
						bulletAngle,
						m_ShooterParam._fSpeed));
				}
			}
		}
	}
}

void CPatternShooter::Execute(float delta) {

	m_fIntervalTimer += delta;
	m_fTime += delta;
	if (m_fIntervalTimer >= m_ShooterParam._fInterval)
	{
		ShootOnce();
		m_fIntervalTimer = 0.f;
	}
	if (m_ShooterParam._isOneShoot || m_fTime >= m_ShooterParam._fEndTime)
	{
		ReturnToMemoryBlock();
	}
}