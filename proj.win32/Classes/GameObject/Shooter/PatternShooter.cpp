#pragma once
#include "PatternShooter.h"
#include "../Bullet/BulletHeaders.h"
#include "../Stage/StageManager.h"
#include "../../Scene/GameScene.h"

CPatternShooter::CPatternShooter(sSHOOTER_PARAM param)
	: CShooter(param)
	, m_ShapeHeight(0)
	, m_ShapeAngle(0.f)
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

void CPatternShooter::Execute(float delta) {

	m_fTime += delta;
	if (m_fTime >= m_ShooterParam._fInterval)
	{
		sPATTERN_SHOOTER_PARAM shape = CStageManager::Instance()->getPatternInfo(m_ShooterParam._ShooterType);

		for (int i = 0; i < shape._height; i++) {

			for (int j = 0; j < shape._width; j++){
				int p = *(shape._pattern + (shape._width * i) + j);
				if (p > 0) {
					CGameScene::getGameScene()->addChild(CPlayStar::create(
						static_cast<eSTAR_TYPE>(p),
						10.f,
						1200.f + ((shape._height - i) * 50.f),
						(j * -3.0f) + m_ShapeAngle,
						250.0f,
						true));
				}
			}
		}
		m_fTime = 0.f;
		m_ShapeAngle = random<float>(0.0f, 180.f);
	}
}
