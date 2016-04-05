#pragma once
#include "PatternShooter.h"
#include "../Bullet/BulletHeaders.h"
#include "../../DataManager/BulletPatternDataManager.h"
#include "../../DataManager/BulletDataManager.h"
#include "../../Scene/GameScene.h"

CPatternShooter::CPatternShooter(sSHOOTER_PARAM param)
	: CShooter(param)
{}

CPatternShooter* CPatternShooter::create(sSHOOTER_PARAM param)				// Bullet 생성 간격
{
	CPatternShooter* pRet = (CPatternShooter*)new(std::nothrow)
		CPatternShooter(param);
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

void CPatternShooter::ShootWithPosition(sSHOOTER_PARAM param, float angle, float distance /*= 0.0f*/)
{
	sPATTERN_SHOOTER_PARAM pattern = CBulletPatternDataManager::Instance()->getPatternInfo(param._PatternName);

	for (int i = 0; i < pattern._height; i++) {

		for (int j = 0; j < pattern._width; j++){
			char p = *(pattern._pattern + (pattern._width * i) + j);
			if (p != ' ') {
				sBULLET_PARAM bulletParam = CBulletDataManager::Instance()->getBulletInfo(p);
				float initDistance = 0.f;
				if (distance == 0.0f)
					initDistance = bulletParam._fDistance;
				else
					initDistance = distance;
				float bulletAngle = (j * -pattern._widthAngleDistance) + angle + 20.f;
				bulletParam._fDistance = initDistance + ((pattern._height - i) * pattern._heightDistance);
				bulletParam._isFly = param._isFly;
				
				if (p >= '1' && p <= '3')
				{
					CGameScene::getGameScene()->addChild(CNormalBullet::create(
						bulletParam,
						bulletAngle,
						param._fSpeed));
				}
				else if (p >= '4' && p <= '5')
				{
					CGameScene::getGameScene()->addChild(CNormalMissile::create(
						bulletParam,
						bulletAngle,
						param._fSpeed));
				}
				else if (p >= 'A' && p <= 'G')
				{
					CGameScene::getGameScene()->addChild(CPlayItem::create(
						bulletParam,
						bulletAngle,
						250.f));
				}
				else if (p >= 'P' && p <= 'T')
				{
					CGameScene::getGameScene()->addChild(CPlayCoin::create(
						bulletParam,
						bulletAngle,
						param._fSpeed));
				}
				else if (p >= 'U' && p <= 'Y')
				{
					CGameScene::getGameScene()->addChild(CPlayStar::create(
						bulletParam,
						bulletAngle,
						param._fSpeed));
				}
				else if ('Z')
				{
					CGameScene::getGameScene()->addChild(CBonusLetter::create(
						bulletParam,
						bulletAngle,
						param._fSpeed));
				}
			}
		}
	}
}

void CPatternShooter::ShootOnce()
{
	ShootWithPosition(m_ShooterParam, m_ShooterParam._fAngle);
}

void CPatternShooter::Execute(float delta) {

	m_fTime += delta;
	if (m_fTime >= m_ShooterParam._fInterval)
	{
		ShootOnce();
		m_fTime = 0.f;
	}
}