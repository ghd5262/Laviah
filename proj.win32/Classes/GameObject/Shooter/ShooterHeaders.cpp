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
	// BonusTimeUI ������ ȹ��
	CBonusTimeUI* gameSceneUIManager 
		= static_cast<CBonusTimeUI*>(CUIManager::Instance()->FindUIWithName("BonusTime"));

	// ���� ���ĺ��� ���� ������
	Vec2 targetPos = gameSceneUIManager->NonCollectedLetterWorldPos(); 

	// ���� ���ĺ��� �ѹ�
	int letterNum = static_cast<int>(gameSceneUIManager->NonCollectedLetterNum()); 
	
	float shotAngle = random<float>(0.f, 360.f);
	CGameScene::getGameScene()->addChild(CBonusLetter::create(
		MakeString("bonusLetter_%d.png", letterNum),					//�̹��� �̸�
		20.f,															//�浹 ����
		shotAngle,														//�ʱ� ����
		param._fSpeed));												//�ӵ�
}
//----------------------------------------------------


//-------------------BonusLetter Shooter-------------------
void PlayItemShoot(sSHOOTER_PARAM param)
{
	float shotAngle = random<float>(0.f, 360.f);
	int itemNum = param._ShooterType - eSHOOTER_TYPE_item_0;

	CGameScene::getGameScene()->addChild(CPlayItem::create(
		MakeString("playItem_%d.png", itemNum),							//�̹��� �̸�
		20.f,															//�浹 ����
		shotAngle,														//�ʱ� ����
		param._fSpeed,													//�ӵ�
		itemNum,														//item type
		param._isFly));													//fly Item ����
}
//----------------------------------------------------