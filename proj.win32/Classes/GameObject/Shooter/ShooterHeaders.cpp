#pragma once
#include "ShooterHeaders.h"
#include "../ItemManager.h"
#include "../ObjectManager.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"
#include "../../MyUI/UIManager.h"
#include "../../MyUI/BonusTimeUI.h"

//-------------------BonusLetter Shooter-------------------
void BonusLetterShoot(sSHOOTER_PARAM param)
{
	// BonusTimeUI 포인터 획득
	CBonusTimeUI* gameSceneUIManager 
		= static_cast<CBonusTimeUI*>(CUIManager::Instance()->FindUIWithName("BonusTime"));

	// 다음 알파벳의 월드 포지션
	Vec2 targetPos = gameSceneUIManager->NonCollectedLetterWorldPos(); 

	// 다음 알파벳의 넘버
	int letterNum = static_cast<int>(gameSceneUIManager->NonCollectedLetterNum()); 
	
	float shotAngle = random<float>(0.f, 360.f);
	CGameScene::getGameScene()->addChild(CBonusLetter::create(
		MakeString("bonusLetter_%d.png", letterNum),					//이미지 이름
		20.f,															//충돌 범위
		shotAngle,														//초기 각도
		param._fSpeed));												//속도
}
//----------------------------------------------------


//-------------------BonusLetter Shooter-------------------
void PlayItemShoot(sSHOOTER_PARAM param)
{
	float shotAngle = random<float>(0.f, 360.f);
	int itemNum = param._ShooterType - eSHOOTER_TYPE_item_0;

	CGameScene::getGameScene()->addChild(CPlayItem::create(
		MakeString("playItem_%d.png", itemNum),							//이미지 이름
		20.f,															//충돌 범위
		shotAngle,														//초기 각도
		param._fSpeed,													//속도
		itemNum,														//item type
		param._isFly));													//fly Item 여부
}
//----------------------------------------------------