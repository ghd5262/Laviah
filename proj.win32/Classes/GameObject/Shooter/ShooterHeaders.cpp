#pragma once
#include "ShooterHeaders.h"
#include "../ItemManager.h"
#include "../ObjectManager.h"
#include "../Bullet/BulletHeaders.h"
#include "../../Scene/GameScene.h"
#include "../../MyUI/UIManager.h"
#include "../../MyUI/BonusTimeUI.h"

//-------------------BonusLetter Shooter-------------------
void BonusLetterShoot(
	float speed/* = 100*/, 
	float interval/* = 0.1f*/, 
	int maxBulletCount /* = 1*/)
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
		speed));														//속도
}
//----------------------------------------------------


//-------------------BonusLetter Shooter-------------------
void PlayItemShoot(
	float speed/* = 100*/,
	float interval/* = 0.1f*/,
	int maxBulletCount /* = 1*/)
{

	float shotAngle = random<float>(0.f, 360.f);
	int itemNum = random<int>(0, static_cast<int>(eITEM_TYPE_MAX - 1));

	CGameScene::getGameScene()->addChild(CPlayItem::create(
		MakeString("playItem_%d.png", itemNum),							//이미지 이름
		20.f,															//충돌 범위
		shotAngle,														//초기 각도
		speed,															//속도
		itemNum,														//item type
		true));															//fly Item 여부
	CGameScene::getGameScene()->addChild(CPlayItem::create(
		MakeString("playItem_%d.png", itemNum),							//이미지 이름
		20.f,															//충돌 범위
		shotAngle,														//초기 각도
		speed,															//속도
		itemNum,														//item type
		false));														//fly Item 여부
}
//----------------------------------------------------