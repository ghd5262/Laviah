#pragma once
#include "PatternShooter.h"
#include "../Bullet/BulletHeaders.h"
#include "../../DataManager/BulletPatternDataManager.h"
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

void CPatternShooter::ShootWithPosition(std::string patternName, float angle, float distance)
{
	sPATTERN_SHOOTER_PARAM pattern = CBulletPatternDataManager::Instance()->getPatternInfo(patternName);

	for (int i = 0; i < pattern._height; i++) {

		for (int j = 0; j < pattern._width; j++){
			int p = *(pattern._pattern + (pattern._width * i) + j);
			if (p > 0) {
				CGameScene::getGameScene()->addChild(CPlayCoin::create(
					static_cast<eCOIN_TYPE>(p),
					10.f,
					distance + ((pattern._height - i) * pattern._heightDistance),
					(j * -pattern._widthAngleDistance) + angle + 20.f,
					m_ShooterParam._fSpeed,
					true));
			}
		}
	}
}

void CPatternShooter::ShootOnce()
{
	ShootWithPosition(m_ShooterParam._PatternName, m_ShooterParam._fAngle, 1200.f);
}

void CPatternShooter::Execute(float delta) {

	m_fTime += delta;
	if (m_fTime >= m_ShooterParam._fInterval)
	{
		ShootOnce();
		m_fTime = 0.f;
	}
}
