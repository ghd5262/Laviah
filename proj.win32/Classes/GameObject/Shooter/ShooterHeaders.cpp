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
		speed));														//�ӵ�
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